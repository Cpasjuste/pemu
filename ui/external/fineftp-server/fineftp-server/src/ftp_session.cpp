#include "ftp_session.h"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <map>
#include <functional>
#include <fstream>


#include "filesystem.h"

#ifdef WIN32
#include <direct.h>
#include "win_str_convert.h"
#endif // WIN32


namespace fineftp
{

  FtpSession::FtpSession(asio::io_service& io_service, const UserDatabase& user_database, const std::function<void()>& completion_handler)
    : completion_handler_   (completion_handler)
    , user_database_        (user_database)
    , io_service_           (io_service)
    , command_socket_       (io_service)
    , command_write_strand_ (io_service)
    , data_type_binary_     (false)
    , data_acceptor_        (io_service)
    , data_buffer_strand_   (io_service)
    , file_rw_strand_       (io_service)
    , ftp_working_directory_("/")
  {
  }

  FtpSession::~FtpSession()
  {
#ifndef NDEBUG
    std::cout << "Ftp Session shutting down" << std::endl;
#endif // !NDEBUG
    completion_handler_();
  }

  void FtpSession::start()
  {
    asio::error_code ec;
    command_socket_.set_option(asio::ip::tcp::no_delay(true), ec);
    if (ec) std::cerr << "Unable to set socket option tcp::no_delay: " << ec.message() << std::endl;

    sendFtpMessage(FtpMessage(FtpReplyCode::SERVICE_READY_FOR_NEW_USER, "Welcome to fineFTP Server"));
    readFtpCommand();
  }

  asio::ip::tcp::socket& FtpSession::getSocket()
  {
    return command_socket_;
  }

  void FtpSession::sendFtpMessage(const FtpMessage& message)
  {
    sendRawFtpMessage(message.str());
  }
  void FtpSession::sendFtpMessage(FtpReplyCode code, const std::string& message)
  {
    sendFtpMessage(FtpMessage(code, message));
  }

  void FtpSession::sendRawFtpMessage(const std::string& raw_message)
  {
    command_write_strand_.post([me = shared_from_this(), raw_message]()
                                {
                                  bool write_in_progress = !me->command_output_queue_.empty();
                                  me->command_output_queue_.push_back(raw_message);
                                  if (!write_in_progress)
                                  {
                                    me->startSendingMessages();
                                  }
                                });
  }

  void FtpSession::startSendingMessages()
  {
#ifndef NDEBUG
    std::cout << "FTP >> " << command_output_queue_.front() << std::endl;
#endif

    asio::async_write(command_socket_
                    , asio::buffer(command_output_queue_.front())
                    , command_write_strand_.wrap(
                      [me = shared_from_this()](asio::error_code ec, std::size_t /*bytes_to_transfer*/)
                      {
                        if (!ec)
                        {
                          me->command_output_queue_.pop_front();
              
                          if (!me->command_output_queue_.empty())
                          {
                            me->startSendingMessages();
                          }
                        }
                        else
                        {
                          std::cerr << "Command write error: " << ec.message() << std::endl;
                        }
                      }
                    ));
  }

  void FtpSession::readFtpCommand()
  {
    asio::async_read_until(command_socket_, command_input_stream_, "\r\n",
                        [me = shared_from_this()](asio::error_code ec, std::size_t length)
                        {
                          if (ec)
                          {
                            if (ec != asio::error::eof)
                            {
                              std::cerr << "read_until error: " << ec.message() << std::endl;
                            }
#ifndef NDEBUG
                            else
                            {
                              std::cout << "Control connection closed by client." << std::endl;
                            }
#endif // !NDEBUG
                            return;
                          }

                          std::istream stream (&(me->command_input_stream_));
                          std::string packet_string(length - 2, ' ');
                          stream.read(&packet_string[0], length - 2);

                          stream.ignore(2); // Remove the "\r\n"
#ifndef NDEBUG
                          std::cout << "FTP << " << packet_string << std::endl;
#endif

                          me->handleFtpCommand(packet_string);
                        });
  }

  void FtpSession::handleFtpCommand(const std::string& command)
  {
    std::string ftp_command;
    std::string parameters;

    size_t space_index = command.find_first_of(' ');

    ftp_command = command.substr(0, space_index);
    std::transform(ftp_command.begin(), ftp_command.end(), ftp_command.begin(), [](char c) { return static_cast<char>(std::toupper(static_cast<unsigned char>(c))); });

    if (space_index != std::string::npos)
    {
      parameters = command.substr(space_index + 1, std::string::npos);
    }


    const std::map<std::string, std::function<void(std::string)>> command_map {
      // Access control commands
      { "USER", std::bind(&FtpSession::handleFtpCommandUSER, this, std::placeholders::_1) },
      { "PASS", std::bind(&FtpSession::handleFtpCommandPASS, this, std::placeholders::_1) },
      { "ACCT", std::bind(&FtpSession::handleFtpCommandACCT, this, std::placeholders::_1) },
      { "CWD",  std::bind(&FtpSession::handleFtpCommandCWD,  this, std::placeholders::_1) },
      { "CDUP", std::bind(&FtpSession::handleFtpCommandCDUP, this, std::placeholders::_1) },
      { "REIN", std::bind(&FtpSession::handleFtpCommandREIN, this, std::placeholders::_1) },
      { "QUIT", std::bind(&FtpSession::handleFtpCommandQUIT, this, std::placeholders::_1) },

      // Transfer parameter commands
      { "PORT", std::bind(&FtpSession::handleFtpCommandPORT, this, std::placeholders::_1) },
      { "PASV", std::bind(&FtpSession::handleFtpCommandPASV, this, std::placeholders::_1) },
      { "TYPE", std::bind(&FtpSession::handleFtpCommandTYPE, this, std::placeholders::_1) },
      { "STRU", std::bind(&FtpSession::handleFtpCommandSTRU, this, std::placeholders::_1) },
      { "MODE", std::bind(&FtpSession::handleFtpCommandMODE, this, std::placeholders::_1) },

      // Ftp service commands
      { "RETR", std::bind(&FtpSession::handleFtpCommandRETR, this, std::placeholders::_1) },
      { "STOR", std::bind(&FtpSession::handleFtpCommandSTOR, this, std::placeholders::_1) },
      { "STOU", std::bind(&FtpSession::handleFtpCommandSTOU, this, std::placeholders::_1) },
      { "APPE", std::bind(&FtpSession::handleFtpCommandAPPE, this, std::placeholders::_1) },
      { "ALLO", std::bind(&FtpSession::handleFtpCommandALLO, this, std::placeholders::_1) },
      { "REST", std::bind(&FtpSession::handleFtpCommandREST, this, std::placeholders::_1) },
      { "RNFR", std::bind(&FtpSession::handleFtpCommandRNFR, this, std::placeholders::_1) },
      { "RNTO", std::bind(&FtpSession::handleFtpCommandRNTO, this, std::placeholders::_1) },
      { "ABOR", std::bind(&FtpSession::handleFtpCommandABOR, this, std::placeholders::_1) },
      { "DELE", std::bind(&FtpSession::handleFtpCommandDELE, this, std::placeholders::_1) },
      { "RMD",  std::bind(&FtpSession::handleFtpCommandRMD,  this, std::placeholders::_1) },
      { "MKD",  std::bind(&FtpSession::handleFtpCommandMKD,  this, std::placeholders::_1) },
      { "PWD",  std::bind(&FtpSession::handleFtpCommandPWD,  this, std::placeholders::_1) },
      { "LIST", std::bind(&FtpSession::handleFtpCommandLIST, this, std::placeholders::_1) },
      { "NLST", std::bind(&FtpSession::handleFtpCommandNLST, this, std::placeholders::_1) },
      { "SITE", std::bind(&FtpSession::handleFtpCommandSITE, this, std::placeholders::_1) },
      { "SYST", std::bind(&FtpSession::handleFtpCommandSYST, this, std::placeholders::_1) },
      { "STAT", std::bind(&FtpSession::handleFtpCommandSTAT, this, std::placeholders::_1) },
      { "HELP", std::bind(&FtpSession::handleFtpCommandHELP, this, std::placeholders::_1) },
      { "NOOP", std::bind(&FtpSession::handleFtpCommandNOOP, this, std::placeholders::_1) },

      // Modern FTP Commands
      { "FEAT", std::bind(&FtpSession::handleFtpCommandFEAT, this, std::placeholders::_1) },
      { "OPTS", std::bind(&FtpSession::handleFtpCommandOPTS, this, std::placeholders::_1) },
      { "SIZE", std::bind(&FtpSession::handleFtpCommandSIZE, this, std::placeholders::_1) },
    };

    auto command_it = command_map.find(ftp_command);
    if (command_it != command_map.end())
    {
      command_it->second(parameters);
      last_command_ = ftp_command;
    }
    else
    {
      sendFtpMessage(FtpReplyCode::SYNTAX_ERROR_UNRECOGNIZED_COMMAND, "Unrecognized command");
    }

    if (last_command_ == "QUIT")
    {
      // Close command socket
      command_write_strand_.wrap([me = shared_from_this()]() { me->command_socket_.close();  });
    }
    else
    {
      // Wait for next command
      readFtpCommand();
    }
  }

  ////////////////////////////////////////////////////////
  // FTP Commands
  ////////////////////////////////////////////////////////


  // Access control commands

  void FtpSession::handleFtpCommandUSER(const std::string& param)
  {
    logged_in_user_        = nullptr;
    username_for_login_    = param;
    ftp_working_directory_ = "/";

    if (param.empty())
    {
      sendFtpMessage(FtpReplyCode::SYNTAX_ERROR_PARAMETERS, "Please provide username");
      return;
    }
    else
    {
      sendFtpMessage(FtpReplyCode::USER_NAME_OK, "Please enter password");
      return;
    }
  }

  void FtpSession::handleFtpCommandPASS(const std::string& param)
  {
    if (last_command_ != "USER")
    {
      sendFtpMessage(FtpReplyCode::COMMANDS_BAD_SEQUENCE, "Please specify username first");
      return;
    }
    else
    {
      auto user = user_database_.getUser(username_for_login_, param);
      if (user)
      {
        logged_in_user_ = user;
        sendFtpMessage(FtpReplyCode::USER_LOGGED_IN, "Login successful");
        return;
      }
      else
      {
        sendFtpMessage(FtpReplyCode::NOT_LOGGED_IN, "Failed to log in");
        return;
      }
    }
  }

  void FtpSession::handleFtpCommandACCT(const std::string& /*param*/)
  {
    sendFtpMessage(FtpReplyCode::SYNTAX_ERROR_UNRECOGNIZED_COMMAND, "Unsupported command");
    return;
  }

  void FtpSession::handleFtpCommandCWD(const std::string& param)
  {
      sendFtpMessage(executeCWD(param));
      return;
  }

  void FtpSession::handleFtpCommandCDUP(const std::string& /*param*/)
  {
    if (!logged_in_user_)
    {
      sendFtpMessage(FtpReplyCode::NOT_LOGGED_IN,    "Not logged in");
      return;
    }
    if (static_cast<int>(logged_in_user_->permissions_ & Permission::DirList) == 0) 
    {
      sendFtpMessage(FtpReplyCode::ACTION_NOT_TAKEN, "Permission denied");
      return;
    }

    if (ftp_working_directory_ != "/")
    {
      // Only CDUP when we are not already at the root directory
      auto cwd_reply = executeCWD("..");
      if (cwd_reply.replyCode() == FtpReplyCode::FILE_ACTION_COMPLETED)
      {
        // The CWD returns FILE_ACTION_COMPLETED on success, while CDUP returns COMMAND_OK on success.
        sendFtpMessage(FtpReplyCode::COMMAND_OK, cwd_reply.message());
        return;
      }
      else
      {
        sendFtpMessage(cwd_reply);
        return;
      }
    }
    else
    {
      sendFtpMessage(FtpReplyCode::ACTION_NOT_TAKEN, "Already at root directory");
      return;
    }
  }
  
  void FtpSession::handleFtpCommandREIN(const std::string& /*param*/)
  {
    sendFtpMessage(FtpReplyCode::COMMAND_NOT_IMPLEMENTED, "Unsupported command");
    return;
  }

  void FtpSession::handleFtpCommandQUIT(const std::string& /*param*/)
  {
    logged_in_user_ = nullptr;
    sendFtpMessage(FtpReplyCode::SERVICE_CLOSING_CONTROL_CONNECTION, "Connection shutting down");
    return;
  }

  // Transfer parameter commands

  void FtpSession::handleFtpCommandPORT(const std::string& /*param*/)
  {
    sendFtpMessage(FtpReplyCode::SYNTAX_ERROR_UNRECOGNIZED_COMMAND, "FTP active mode is not supported by this server");
    return;
  }

  void FtpSession::handleFtpCommandPASV(const std::string& /*param*/)
  {
    if (!logged_in_user_)
    {
      sendFtpMessage(FtpReplyCode::NOT_LOGGED_IN,    "Not logged in");
      return;
    }

    if (data_acceptor_.is_open())
    {
      asio::error_code ec;
      data_acceptor_.close(ec);
      if (ec)
      {
        std::cerr << "Error closing data acceptor: " << ec.message() << std::endl;
      }
    }

    asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), 0);

    {
      asio::error_code ec;
      data_acceptor_.open(endpoint.protocol(), ec);
      if (ec)
      {
        std::cerr << "Error opening data acceptor: " << ec.message() << std::endl;
        sendFtpMessage(FtpReplyCode::SERVICE_NOT_AVAILABLE, "Failed to enter passive mode.");
        return;
      }
    }
    {
      asio::error_code ec;
      data_acceptor_.bind(endpoint);
      if (ec)
      {
        std::cerr << "Error binding data acceptor: " << ec.message() << std::endl;
        sendFtpMessage(FtpReplyCode::SERVICE_NOT_AVAILABLE, "Failed to enter passive mode.");
        return;
      }
    }
    {
      asio::error_code ec;
      data_acceptor_.listen(asio::socket_base::max_connections, ec);
      if (ec)
      {
        std::cerr << "Error listening on data acceptor: " << ec.message() << std::endl;
        sendFtpMessage(FtpReplyCode::SERVICE_NOT_AVAILABLE, "Failed to enter passive mode.");
        return;
      }
    }

    // Split address and port into bytes and get the port the OS chose for us
    auto ip_bytes = command_socket_.local_endpoint().address().to_v4().to_bytes();
    auto port     = data_acceptor_.local_endpoint().port();

    // Form reply string
    std::stringstream stream;
    stream << "(";
    for (size_t i = 0; i < 4; i++)
    {
      stream << static_cast<int>(ip_bytes[i]) << ",";
    }
    stream << ((port >> 8) & 0xff) << "," << (port & 0xff) << ")";

    sendFtpMessage(FtpReplyCode::ENTERING_PASSIVE_MODE, "Entering passive mode " + stream.str());
    return;
  }

  void FtpSession::handleFtpCommandTYPE(const std::string& param)
  {
    if (!logged_in_user_)
    {
      sendFtpMessage(FtpReplyCode::NOT_LOGGED_IN,    "Not logged in");
      return;
    }

    if (param == "A")
    {
      data_type_binary_ = false;
      // TODO: The ASCII mode currently does not work as RFC 959 demands it. It
      // should perform line ending conversion, which it doesn't. But as we are
      // living in the 21st centry, nobody should use ASCII mode anyways.
      sendFtpMessage(FtpReplyCode::COMMAND_OK, "Switching to ASCII mode");
      return;
    }
    else if (param == "I")
    {
      data_type_binary_ = true;
      sendFtpMessage(FtpReplyCode::COMMAND_OK, "Switching to binary mode");
      return;
    }
    else
    {
      sendFtpMessage(FtpReplyCode::COMMAND_NOT_IMPLEMENTED_FOR_PARAMETER, "Unknown or unsupported type");
      return;
    }
  }

  void FtpSession::handleFtpCommandSTRU(const std::string& /*param*/)
  {
    sendFtpMessage(FtpReplyCode::SYNTAX_ERROR_UNRECOGNIZED_COMMAND, "Unsupported command");
    return;
  }

  void FtpSession::handleFtpCommandMODE(const std::string& /*param*/)
  {
    sendFtpMessage(FtpReplyCode::SYNTAX_ERROR_UNRECOGNIZED_COMMAND, "Unsupported command");
    return;
  }

  // Ftp service commands
  void FtpSession::handleFtpCommandRETR(const std::string& param)
  {
    if (!logged_in_user_)
    {
      sendFtpMessage(FtpReplyCode::NOT_LOGGED_IN,    "Not logged in");
      return;
    }
    if (static_cast<int>(logged_in_user_->permissions_ & Permission::FileRead) == 0)
    {
      sendFtpMessage(FtpReplyCode::ACTION_NOT_TAKEN,              "Permission denied");
      return;
    }
    if (!data_acceptor_.is_open())
    {
      sendFtpMessage(FtpReplyCode::ERROR_OPENING_DATA_CONNECTION, "Error opening data connection");
      return;
    }

    std::string local_path = toLocalPath(param);
    
    std::ios::openmode open_mode = (data_type_binary_ ? (std::ios::in | std::ios::binary) : (std::ios::in));
    std::shared_ptr<IoFile> file = std::make_shared<IoFile>(local_path, open_mode);

    if (!file->file_stream_.good())
    {
      sendFtpMessage(FtpReplyCode::ACTION_ABORTED_LOCAL_ERROR, "Error opening file for transfer");
      return;
    }

    sendFtpMessage(FtpReplyCode::FILE_STATUS_OK_OPENING_DATA_CONNECTION, "Sending file");
    sendFile(file);
    return;
  }

  void FtpSession::handleFtpCommandSIZE(const std::string& param)
  {
    if (!logged_in_user_)
    {
      sendFtpMessage(FtpReplyCode::NOT_LOGGED_IN, "Not logged in");
      return;
    }
    if (static_cast<int>(logged_in_user_->permissions_ & Permission::FileRead) == 0)
    {
      sendFtpMessage(FtpReplyCode::ACTION_NOT_TAKEN, "Permission denied");
      return;
    }

    std::string local_path = toLocalPath(param);

    std::ios::openmode open_mode =
       std::ios::ate | (data_type_binary_ ? (std::ios::in | std::ios::binary) : (std::ios::in));
#if defined(WIN32) && !defined(__GNUG__)
    std::ifstream file(StrConvert::Utf8ToWide(local_path), open_mode);
#else
    std::ifstream file(local_path, open_mode);
#endif

    if (!file.good())
    {
      sendFtpMessage(FtpReplyCode::ACTION_ABORTED_LOCAL_ERROR, "Error opening file for size retrieval");
      return;
    }

    // RFC 3659 actually states that the returned size should depend on the STRU, MODE, and TYPE and that
    // the returned size should be exact. We don't comply with this here. The size returned is the
    // size for TYPE=I.
    auto file_size = file.tellg();
    if (std::fstream::pos_type(-1) == file_size)
    {
      sendFtpMessage(FtpReplyCode::ACTION_ABORTED_LOCAL_ERROR, "Error getting file size");
      return;
    }

    // Form reply string
    std::stringstream rep;
    rep << file_size;

    sendFtpMessage(FtpReplyCode::FILE_STATUS, rep.str());
  }

  void FtpSession::handleFtpCommandSTOR(const std::string& param)
  {
    if (!logged_in_user_)
    {
      sendFtpMessage(FtpReplyCode::NOT_LOGGED_IN,    "Not logged in");
      return;
    }

    // TODO: the ACTION_NOT_TAKEN reply is not RCF 959 conform. Apparently in
    // 1985 nobody anticipated that you might not want anybody uploading files
    // to your server. We use the return code anyways, as the popular FileZilla
    // Server also returns that code as "Permission denied"
    if (static_cast<int>(logged_in_user_->permissions_ & Permission::FileWrite) == 0)
    {
      sendFtpMessage(FtpReplyCode::ACTION_NOT_TAKEN,              "Permission denied");
      return;
    }
    if (!data_acceptor_.is_open())
    {
      sendFtpMessage(FtpReplyCode::ERROR_OPENING_DATA_CONNECTION, "Error opening data connection");
      return;
    }

    std::string local_path = toLocalPath(param);

    auto existing_file_filestatus = Filesystem::FileStatus(local_path);
    if (existing_file_filestatus.isOk())
    {
      if ((existing_file_filestatus.type() == Filesystem::FileType::RegularFile)
        && (static_cast<int>(logged_in_user_->permissions_ & Permission::FileDelete) == 0))
      {
        sendFtpMessage(FtpReplyCode::ACTION_NOT_TAKEN_FILENAME_NOT_ALLOWED, "File already exists. Permission denied to overwrite file.");
        return;
      }
      else if (existing_file_filestatus.type() == Filesystem::FileType::Dir)
      {
        sendFtpMessage(FtpReplyCode::ACTION_NOT_TAKEN_FILENAME_NOT_ALLOWED, "Cannot create file. A directory with that name already exists.");
        return;
      }
    }

    std::ios::openmode open_mode = (data_type_binary_ ? (std::ios::out | std::ios::binary) : (std::ios::out));    
    std::shared_ptr<IoFile> file = std::make_shared<IoFile>(local_path, open_mode);

    if (!file->file_stream_.good())
    {
      sendFtpMessage(FtpReplyCode::ACTION_ABORTED_LOCAL_ERROR, "Error opening file for transfer");
      return;
    }

    sendFtpMessage(FtpReplyCode::FILE_STATUS_OK_OPENING_DATA_CONNECTION, "Receiving file");
    receiveFile(file);
    return;
  }

  void FtpSession::handleFtpCommandSTOU(const std::string& /*param*/)
  {
    sendFtpMessage(FtpReplyCode::SYNTAX_ERROR_UNRECOGNIZED_COMMAND, "Command not implemented");
    return;
  }

  void FtpSession::handleFtpCommandAPPE(const std::string& param)
  {
    if (!logged_in_user_)
    {
      sendFtpMessage(FtpReplyCode::NOT_LOGGED_IN,    "Not logged in");
      return;
    }
    if (static_cast<int>(logged_in_user_->permissions_ & Permission::FileAppend) == 0)
    {
      sendFtpMessage(FtpReplyCode::ACTION_NOT_TAKEN, "Permission denied");
      return;
    }
    if (!data_acceptor_.is_open())
    {
      sendFtpMessage(FtpReplyCode::ERROR_OPENING_DATA_CONNECTION, "Error opening data connection");
      return;
    }

    std::string local_path = toLocalPath(param);

    auto existing_file_filestatus = Filesystem::FileStatus(local_path);
    if (!existing_file_filestatus.isOk()
      || (existing_file_filestatus.type() != Filesystem::FileType::RegularFile))
    {
      sendFtpMessage(FtpReplyCode::ACTION_NOT_TAKEN, "File does not exist.");
      return;
    }

    std::ios::openmode open_mode = (data_type_binary_ ? (std::ios::out | std::ios::app | std::ios::binary) : (std::ios::out | std::ios::app));
    std::shared_ptr<IoFile> file = std::make_shared<IoFile>(local_path, open_mode);

    if (!file->file_stream_.good())
    {
      sendFtpMessage(FtpReplyCode::ACTION_ABORTED_LOCAL_ERROR, "Error opening file for transfer");
      return;
    }

    sendFtpMessage(FtpReplyCode::FILE_STATUS_OK_OPENING_DATA_CONNECTION, "Receiving file");
    receiveFile(file);
    return;
  }

  void FtpSession::handleFtpCommandALLO(const std::string& /*param*/)
  {
    sendFtpMessage(FtpReplyCode::SYNTAX_ERROR_UNRECOGNIZED_COMMAND, "Command not implemented");
    return;
  }

  void FtpSession::handleFtpCommandREST(const std::string& /*param*/)
  {
    sendFtpMessage(FtpReplyCode::COMMAND_NOT_IMPLEMENTED, "Command not implemented");
    return;
  }

  void FtpSession::handleFtpCommandRNFR(const std::string& param)
  {
    rename_from_path_.clear();

    auto is_renamable_error = checkIfPathIsRenamable(param);

    if (is_renamable_error.replyCode() == FtpReplyCode::COMMAND_OK)
    {
      rename_from_path_ = param;
      sendFtpMessage(FtpReplyCode::FILE_ACTION_NEEDS_FURTHER_INFO, "Enter target name");
      return;
    }
    else
    {
      sendFtpMessage(is_renamable_error);
      return;
    }
  }

  void FtpSession::handleFtpCommandRNTO(const std::string& param)
  {
    if (!logged_in_user_)
    {
      sendFtpMessage(FtpReplyCode::NOT_LOGGED_IN,    "Not logged in");
      return;
    }

    if (last_command_ != "RNFR" || rename_from_path_.empty())
    {
      sendFtpMessage(FtpReplyCode::COMMANDS_BAD_SEQUENCE, "Please specify target file first");
      return;
    }

    if (param.empty())
    {
      sendFtpMessage(FtpReplyCode::SYNTAX_ERROR_PARAMETERS, "No target name given");
      return;
    }

    // TODO: returning neiher FILE_ACTION_NOT_TAKEN nor ACTION_NOT_TAKEN are
    // RFC 959 conform. Aoarently back in 1985 it was assumed that the RNTO
    //command will always succeed, as long as you enter a valid target file
    // name. Thus we use the two return codes anyways, the popular FileZilla
    // FTP Server uses those as well.
    auto is_renamable_error = checkIfPathIsRenamable(rename_from_path_);

    if (is_renamable_error.replyCode() == FtpReplyCode::COMMAND_OK)
    {
      std::string local_from_path = toLocalPath(rename_from_path_);
      std::string local_to_path   = toLocalPath(param);

      // Check if the source file exists already. We simple disallow overwriting a
      // file be renaming (the bahavior of the native rename command on Windows
      // and Linux differs; Windows will not overwrite files, Linux will).      
      if (Filesystem::FileStatus(local_to_path).isOk())
      {
        sendFtpMessage(FtpReplyCode::FILE_ACTION_NOT_TAKEN, "Target path exists already.");
        return;
      }

#ifdef WIN32

      if (MoveFileW(StrConvert::Utf8ToWide(local_from_path).c_str(), StrConvert::Utf8ToWide(local_to_path).c_str()) != 0)
      {
        sendFtpMessage(FtpReplyCode::FILE_ACTION_COMPLETED, "OK");
        return;
      }
      else
      {
        sendFtpMessage(FtpReplyCode::FILE_ACTION_NOT_TAKEN, "Error renaming file: " + GetLastErrorStr());
        return;
      }
#else // WIN32
      if (rename(local_from_path.c_str(), local_to_path.c_str()) == 0)
      {
        sendFtpMessage(FtpReplyCode::FILE_ACTION_COMPLETED, "OK");
        return;
      }
      else
      {
        sendFtpMessage(FtpReplyCode::FILE_ACTION_NOT_TAKEN, "Error renaming file");
        return;
      }
#endif // WIN32
    }
    else
    {
      sendFtpMessage(is_renamable_error);
      return;
    }
  }

  void FtpSession::handleFtpCommandABOR(const std::string& /*param*/)
  {
    sendFtpMessage(FtpReplyCode::COMMAND_NOT_IMPLEMENTED, "Command not implemented");
    return;
  }

  void FtpSession::handleFtpCommandDELE(const std::string& param)
  {
    if (!logged_in_user_)
    {
      sendFtpMessage(FtpReplyCode::NOT_LOGGED_IN,    "Not logged in");
      return;
    }
    std::string local_path = toLocalPath(param);

    auto file_status = Filesystem::FileStatus(local_path);

    if (!file_status.isOk())
    {
      sendFtpMessage(FtpReplyCode::ACTION_NOT_TAKEN, "Resource does not exist");
      return;
    }
    else if (file_status.type() != Filesystem::FileType::RegularFile)
    {
      sendFtpMessage(FtpReplyCode::ACTION_NOT_TAKEN, "Resource is not a file");
      return;
    }
    else
    {
      if (static_cast<int>(logged_in_user_->permissions_ & Permission::FileDelete) == 0)
      {
        sendFtpMessage(FtpReplyCode::ACTION_NOT_TAKEN, "Permission denied");
        return;
      }
      else
      {
#ifdef WIN32
        if (DeleteFileW(StrConvert::Utf8ToWide(local_path).c_str()) != 0)
        {
          sendFtpMessage(FtpReplyCode::FILE_ACTION_COMPLETED, "Successfully deleted file");
          return;
        }
        else
        {
          sendFtpMessage(FtpReplyCode::FILE_ACTION_NOT_TAKEN, "Unable to delete file: " + GetLastErrorStr());
          return;
        }
#else
        if (unlink(local_path.c_str()) == 0)
        {
          sendFtpMessage(FtpReplyCode::FILE_ACTION_COMPLETED, "Successfully deleted file");
          return;
        }
        else
        {
          sendFtpMessage(FtpReplyCode::FILE_ACTION_NOT_TAKEN, "Unable to delete file");
          return;
        }
#endif
      }
    }
  }

  void FtpSession::handleFtpCommandRMD(const std::string& param)
  {
    if (!logged_in_user_)
    {
      sendFtpMessage(FtpReplyCode::NOT_LOGGED_IN,    "Not logged in");
      return;
    }
    if (static_cast<int>(logged_in_user_->permissions_ & Permission::DirDelete) == 0)
    {
      sendFtpMessage(FtpReplyCode::ACTION_NOT_TAKEN, "Permission denied");
      return;
    }

    std::string local_path = toLocalPath(param);

#ifdef WIN32
    if (RemoveDirectoryW(StrConvert::Utf8ToWide(local_path).c_str()) != 0)
    {
      sendFtpMessage(FtpReplyCode::FILE_ACTION_COMPLETED, "Successfully removed directory");
      return;
    }
    else
    {
      // If would be a good idea to return a 4xx error code here (-> temp error)
      // (e.g. FILE_ACTION_NOT_TAKEN), but RFC 959 assumes that all directory
      // errors are permanent.
      sendFtpMessage(FtpReplyCode::ACTION_NOT_TAKEN, "Unable to remove directory: " + GetLastErrorStr());
      return;
    }
#else
    if (rmdir(local_path.c_str()) == 0)
    {
      sendFtpMessage(FtpReplyCode::FILE_ACTION_COMPLETED, "Successfully removed directory");
      return;
    }
    else
    {
      // If would be a good idea to return a 4xx error code here (-> temp error)
      // (e.g. FILE_ACTION_NOT_TAKEN), but RFC 959 assumes that all directory
      // errors are permanent.
      sendFtpMessage(FtpReplyCode::ACTION_NOT_TAKEN, "Unable to remove directory");
      return;
    }
#endif

  }

  void FtpSession::handleFtpCommandMKD(const std::string& param)
  {
    if (!logged_in_user_)
    {
      sendFtpMessage(FtpReplyCode::NOT_LOGGED_IN,    "Not logged in");
      return;
    }
    if (static_cast<int>(logged_in_user_->permissions_ & Permission::DirCreate) == 0)
    {
      sendFtpMessage(FtpReplyCode::ACTION_NOT_TAKEN, "Permission denied");
      return;
    }

    auto local_path = toLocalPath(param);

#ifdef WIN32
    LPSECURITY_ATTRIBUTES security_attributes = NULL; // => Default security attributes
    if (CreateDirectoryW(StrConvert::Utf8ToWide(local_path).c_str(), security_attributes) != 0)
    {
      sendFtpMessage(FtpReplyCode::PATHNAME_CREATED, createQuotedFtpPath(toAbsoluteFtpPath(param)) + " Successfully created");
      return;
    }
    else
    {
      // If would be a good idea to return a 4xx error code here (-> temp error)
      // (e.g. FILE_ACTION_NOT_TAKEN), but RFC 959 assumes that all directory
      // errors are permanent.
      sendFtpMessage(FtpReplyCode::ACTION_NOT_TAKEN, "Unable to create directory: " + GetLastErrorStr());
      return;
    }
#else
    mode_t mode = 0755;
    if (mkdir(local_path.c_str(), mode) == 0)
    {
      sendFtpMessage(FtpReplyCode::PATHNAME_CREATED, createQuotedFtpPath(toAbsoluteFtpPath(param)) + " Successfully created");
      return;
    }
    else
    {
      // If would be a good idea to return a 4xx error code here (-> temp error)
      // (e.g. FILE_ACTION_NOT_TAKEN), but RFC 959 assumes that all directory
      // errors are permanent.
      sendFtpMessage(FtpReplyCode::ACTION_NOT_TAKEN, "Unable to create directory");
      return;
    }
#endif
  }

  void FtpSession::handleFtpCommandPWD(const std::string& /*param*/)
  {
    // RFC 959 does not allow returning NOT_LOGGED_IN here, so we abuse ACTION_NOT_TAKEN for that.
    if (!logged_in_user_)
    {
      sendFtpMessage(FtpReplyCode::ACTION_NOT_TAKEN, "Not logged in");
      return;
    }

    sendFtpMessage(FtpReplyCode::PATHNAME_CREATED, createQuotedFtpPath(ftp_working_directory_));
    return;
  }

  void FtpSession::handleFtpCommandLIST(const std::string& param)
  {
    if (!logged_in_user_)
    {
      sendFtpMessage(FtpReplyCode::NOT_LOGGED_IN,    "Not logged in");
      return;
    }

    // RFC 959 does not allow ACTION_NOT_TAKEN (-> permanent error), so we return a temporary error (FILE_ACTION_NOT_TAKEN).
    if (static_cast<int>(logged_in_user_->permissions_ & Permission::DirList) == 0)
    {
      sendFtpMessage(FtpReplyCode::FILE_ACTION_NOT_TAKEN, "Permission denied");
      return;
    }

    // Deal with some unusual commands like "LIST -a", "LIST -a dirname".
    // Some FTP clients send those commands, as if they would call ls on unix.
    // 
    // We try to support those parameters (or rather ignore them), even though
    // this techniqually breaks listing directories that actually use "-a" etc.
    // as directory name. As most clients however first CWD into a directory and
    // call LIST without parameter afterwards and starting a directory name with
    // "-a " / "-l " / "-al " / "-la " is not that common, the compatibility
    // benefit should outperform te potential problems by a lot.
    // 
    // The RFC-violating LIST command now shall be:
    // 
    //   LIST [<SP> <arg>] [<SP> <pathname>] <CRLF>
    // 
    //   with:
    // 
    //     <arg> ::= -a
    //             | -l
    //             | -al
    //             | -la
    //

    std::string path2dst;
    if ((param == "-a") || (param == "-l") || (param == "-al") || (param == "-la"))
    {
      path2dst  = "";
    }
    else if ((param.substr(0, 3)=="-a "|| param.substr(0, 3)=="-l ") && (param.size() > 3))
    {
      path2dst = param.substr(3);
    }
    else if ((param.substr(0, 4) == "-al " || param.substr(0, 4) == "-la ") && (param.size() > 4))
    {
      path2dst = param.substr(4);
    }
    else
    {
      path2dst = param;
    }

    std::string local_path = toLocalPath(path2dst);
    auto dir_status = Filesystem::FileStatus(local_path);

    if (dir_status.isOk())
    {
      if (dir_status.type() == Filesystem::FileType::Dir)
      {
        if (dir_status.canOpenDir())
        {
          sendFtpMessage(FtpReplyCode::FILE_STATUS_OK_OPENING_DATA_CONNECTION, "Sending directory listing");
          sendDirectoryListing(Filesystem::dirContent(local_path));
          return;
        }
        else
        {
          sendFtpMessage(FtpReplyCode::FILE_ACTION_NOT_TAKEN, "Permission denied");
          return;
        }
      }
      else
      {
        // TODO: RFC959: If the pathname specifies a file then the server should send current information on the file.
        sendFtpMessage(FtpReplyCode::FILE_ACTION_NOT_TAKEN, "Path is not a directory");
        return;
      }
    }
    else
    {
      sendFtpMessage(FtpReplyCode::FILE_ACTION_NOT_TAKEN, "Path does not exist");
      return;
    }
  }

  void FtpSession::handleFtpCommandNLST(const std::string& param)
  {
    if (!logged_in_user_)
    {
      sendFtpMessage(FtpReplyCode::NOT_LOGGED_IN,    "Not logged in");
      return;
    }

    // RFC 959 does not allow ACTION_NOT_TAKEN (-> permanent error), so we return a temporary error (FILE_ACTION_NOT_TAKEN).
    if (static_cast<int>(logged_in_user_->permissions_ & Permission::DirList) == 0)
    {
      sendFtpMessage(FtpReplyCode::FILE_ACTION_NOT_TAKEN, "Permission denied");
      return;
    }

    std::string local_path = toLocalPath(param);
    auto dir_status = Filesystem::FileStatus(local_path);

    if (dir_status.isOk())
    {
      if (dir_status.type() == Filesystem::FileType::Dir)
      {
        if (dir_status.canOpenDir())
        {
          sendFtpMessage(FtpReplyCode::FILE_STATUS_OK_OPENING_DATA_CONNECTION, "Sending name list");
          sendNameList(Filesystem::dirContent(local_path));
          return;
        }
        else
        {
          sendFtpMessage(FtpReplyCode::FILE_ACTION_NOT_TAKEN, "Permission denied");
          return;
        }
      }
      else
      {
        // TODO: RFC959: If the pathname specifies a file then the server should send current information on the file.
        sendFtpMessage(FtpReplyCode::FILE_ACTION_NOT_TAKEN, "Path is not a directory");
        return;
      }
    }
    else
    {
      sendFtpMessage(FtpReplyCode::FILE_ACTION_NOT_TAKEN, "Path does not exist");
      return;
    }
  }

  void FtpSession::handleFtpCommandSITE(const std::string& /*param*/)
  {
    sendFtpMessage(FtpReplyCode::SYNTAX_ERROR_UNRECOGNIZED_COMMAND, "Command not implemented");
  }

  void FtpSession::handleFtpCommandSYST(const std::string& /*param*/)
  {
    // Always returning "UNIX" when being asked for the operating system.
    // Some clients (Mozilla Firefox for example) may disconnect, when we
    // return an unknown operating system here. As depending on the Server's
    // operating system is a horrible feature anyways, we simply fake it.
    //
    // Unix should be the best compatible value here, as we emulate Unix-like
    // outputs for other commands (-> LIST) on all operating systems.
    sendFtpMessage(FtpReplyCode::NAME_SYSTEM_TYPE, "UNIX");
  }

  void FtpSession::handleFtpCommandSTAT(const std::string& /*param*/)
  {
    sendFtpMessage(FtpReplyCode::COMMAND_NOT_IMPLEMENTED, "Command not implemented");
  }

  void FtpSession::handleFtpCommandHELP(const std::string& /*param*/)
  {
    sendFtpMessage(FtpReplyCode::COMMAND_NOT_IMPLEMENTED, "Command not implemented");
  }

  void FtpSession::handleFtpCommandNOOP(const std::string& /*param*/)
  {
    sendFtpMessage(FtpReplyCode::COMMAND_OK, "OK");
  }

  // Modern FTP Commands
  void FtpSession::handleFtpCommandFEAT(const std::string& /*param*/)
  {
    std::stringstream ss;
    ss << "211- Feature List:\r\n";
    ss << " UTF8\r\n";
    ss << " SIZE\r\n";
    ss << " LANG EN\r\n";
    ss << "211 END\r\n";

    sendRawFtpMessage(ss.str());
  }

  void FtpSession::handleFtpCommandOPTS(const std::string& param)
  {
    std::string param_upper = param;
    std::transform(param_upper.begin(), param_upper.end(), param_upper.begin(), [](char c) { return static_cast<char>(std::toupper(static_cast<unsigned char>(c))); });

    if (param_upper == "UTF8 ON")
    {
      sendFtpMessage(FtpReplyCode::COMMAND_OK, "OK");
      return;
    }

    sendFtpMessage(FtpReplyCode::COMMAND_NOT_IMPLEMENTED_FOR_PARAMETER, "Unrecognized parameter");
  }

  ////////////////////////////////////////////////////////
  // FTP data-socket send
  ////////////////////////////////////////////////////////

  void FtpSession::sendDirectoryListing(const std::map<std::string, Filesystem::FileStatus>& directory_content)
  {
    auto data_socket = std::make_shared<asio::ip::tcp::socket>(io_service_);

    data_acceptor_.async_accept(*data_socket
                              , [data_socket, directory_content, me = shared_from_this()](auto ec)
                                {
                                  if (ec)
                                  {
                                    me->sendFtpMessage(FtpReplyCode::TRANSFER_ABORTED, "Data transfer aborted");
                                    return;
                                  }
      // TODO: close acceptor after connect?
                                  // Create a Unix-like file list
                                  std::stringstream stream;
                                  for (const auto& entry : directory_content)
                                  {
                                    const std::string& filename(entry.first);
                                    const fineftp::Filesystem::FileStatus& file_status(entry.second);

                                    stream << ((file_status.type() == fineftp::Filesystem::FileType::Dir) ? 'd' : '-') << file_status.permissionString() << "   1 ";
                                    stream << std::setw(10) << file_status.ownerString() << " " << std::setw(10) << file_status.groupString() << " ";
                                    stream << std::setw(10) << file_status.fileSize() << " ";
                                    stream << file_status.timeString() << " ";
                                    stream << filename;
                                    stream << "\r\n";
                                  }

                                  // Copy the file list into a raw char vector
                                  std::string dir_listing_string = stream.str();
                                  std::shared_ptr<std::vector<char>> dir_listing_rawdata = std::make_shared<std::vector<char>>();
                                  dir_listing_rawdata->reserve(dir_listing_string.size());
                                  std::copy(dir_listing_string.begin(), dir_listing_string.end(), std::back_inserter(*dir_listing_rawdata));

                                  // Send the string out
                                  me->addDataToBufferAndSend(dir_listing_rawdata, data_socket);
                                  me->addDataToBufferAndSend(std::shared_ptr<std::vector<char>>(), data_socket);// Nullpointer indicates end of transmission
                                });
  }

  void FtpSession::sendNameList(const std::map<std::string, Filesystem::FileStatus>& directory_content)
  {
    auto data_socket = std::make_shared<asio::ip::tcp::socket>(io_service_);

    data_acceptor_.async_accept(*data_socket
                              , [data_socket, directory_content, me = shared_from_this()](auto ec)
                                {
                                  if (ec)
                                  {
                                    me->sendFtpMessage(FtpReplyCode::TRANSFER_ABORTED, "Data transfer aborted");
                                    return;
                                  }

                                  // Create a file list
                                  std::stringstream stream;
                                  for (const auto& entry : directory_content)
                                  {
                                    stream << entry.first;
                                    stream << "\r\n";
                                  }

                                  // Copy the file list into a raw char vector
                                  std::string dir_listing_string = stream.str();
                                  std::shared_ptr<std::vector<char>> dir_listing_rawdata = std::make_shared<std::vector<char>>();
                                  dir_listing_rawdata->reserve(dir_listing_string.size());
                                  std::copy(dir_listing_string.begin(), dir_listing_string.end(), std::back_inserter(*dir_listing_rawdata));

                                  // Send the string out
                                  me->addDataToBufferAndSend(dir_listing_rawdata, data_socket);
                                  me->addDataToBufferAndSend(std::shared_ptr<std::vector<char>>(), data_socket);// Nullpointer indicates end of transmission
                                });
  }

  void FtpSession::sendFile(std::shared_ptr<IoFile> file)
  {
    auto data_socket = std::make_shared<asio::ip::tcp::socket>(io_service_);

    data_acceptor_.async_accept(*data_socket
                              , [data_socket, file, me = shared_from_this()](auto ec)
                                {
                                  if (ec)
                                  {
                                    me->sendFtpMessage(FtpReplyCode::TRANSFER_ABORTED, "Data transfer aborted");
                                    return;
                                  }

                                  // Start sending multiple buffers at once
                                  me->readDataFromFileAndSend(file, data_socket);
                                  me->readDataFromFileAndSend(file, data_socket);
                                  me->readDataFromFileAndSend(file, data_socket);
                                });

  }

  void FtpSession::readDataFromFileAndSend(std::shared_ptr<IoFile> file, std::shared_ptr<asio::ip::tcp::socket> data_socket)
  {
    file_rw_strand_.post([me = shared_from_this(), file, data_socket]()
                        {
                          if(file->file_stream_.eof()) return;

                          std::shared_ptr<std::vector<char>>buffer = std::make_shared<std::vector<char>>(1024 * 1024 * 1);
                          file->file_stream_.read(buffer->data(), static_cast<std::streamsize>(buffer->size()));
                          auto bytes_read = file->file_stream_.gcount();
                          buffer->resize(static_cast<size_t>(bytes_read));

                          if (!file->file_stream_.eof())
                          {
                            me->addDataToBufferAndSend(buffer, data_socket, [me, file, data_socket]() {me->readDataFromFileAndSend(file, data_socket); });
                          }
                          else
                          {
                            me->addDataToBufferAndSend(buffer, data_socket);
                            me->addDataToBufferAndSend(std::shared_ptr<std::vector<char>>(nullptr), data_socket);
                          }
                        });
  }

  void FtpSession::addDataToBufferAndSend(std::shared_ptr<std::vector<char>> data, std::shared_ptr<asio::ip::tcp::socket> data_socket, std::function<void(void)> fetch_more)
  {
    data_buffer_strand_.post([me = shared_from_this(), data, data_socket, fetch_more]()
                            {
                              bool write_in_progress = (!me->data_buffer_.empty());

                              me->data_buffer_.push_back(data);

                              if (!write_in_progress)
                              {
                                me->writeDataToSocket(data_socket, fetch_more);
                              }
                            });
  }

  void FtpSession::writeDataToSocket(std::shared_ptr<asio::ip::tcp::socket> data_socket, std::function<void(void)> fetch_more)
  {
    data_buffer_strand_.post(
                    [me = shared_from_this(), data_socket, fetch_more]()
                    {
                      auto data = me->data_buffer_.front();

                      if (data)
                      {
                        // Send out the buffer
                        asio::async_write(*data_socket
                                          , asio::buffer(*data)
                                          , me->data_buffer_strand_.wrap([me, data_socket, data, fetch_more](asio::error_code ec, std::size_t /*bytes_to_transfer*/)
                                            {
                                              me->data_buffer_.pop_front();

                                              if (ec)
                                              {
                                                std::cerr << "Data write error: " << ec.message() << std::endl;
                                                return;
                                              }

                                              fetch_more();

                                              if (!me->data_buffer_.empty())
                                              {
                                                me->writeDataToSocket(data_socket, fetch_more);
                                              }
                                            }
                                            ));
                      }
                      else
                      {
                        // we got to the end of transmission
                        me->data_buffer_.pop_front();
                        me->sendFtpMessage(FtpReplyCode::CLOSING_DATA_CONNECTION, "Done");
                      }
                    }
                    );
  }

  ////////////////////////////////////////////////////////
  // FTP data-socket receive
  ////////////////////////////////////////////////////////

  void FtpSession::receiveFile(std::shared_ptr<IoFile> file)
  {
    auto data_socket = std::make_shared<asio::ip::tcp::socket>(io_service_);

    data_acceptor_.async_accept(*data_socket
                              , [data_socket, file, me = shared_from_this()](auto ec)
                                {
                                  if (ec)
                                  {
                                    me->sendFtpMessage(FtpReplyCode::TRANSFER_ABORTED, "Data transfer aborted");
                                    return;
                                  }

                                  me->receiveDataFromSocketAndWriteToFile(file, data_socket);
                                });
  }

  void FtpSession::receiveDataFromSocketAndWriteToFile(std::shared_ptr<IoFile> file, std::shared_ptr<asio::ip::tcp::socket> data_socket)
  {
    std::shared_ptr<std::vector<char>> buffer = std::make_shared<std::vector<char>>(1024 * 1024 * 1);
      
    asio::async_read(*data_socket
                    , asio::buffer(*buffer)
                    , asio::transfer_at_least(buffer->size())
                    , [me = shared_from_this(), file, data_socket, buffer](asio::error_code ec, std::size_t length)
                      {
                        buffer->resize(length);
                        if (ec)
                        {
                          if (length > 0)
                          {
                            me->writeDataToFile(buffer, file);
                          }
                          me->endDataReceiving(file);
                          return;
                        }
                        else if (length > 0)
                        {
                          me->writeDataToFile(buffer, file, [me, file, data_socket]() { me->receiveDataFromSocketAndWriteToFile(file, data_socket); });
                        }
                      });
  }


  void FtpSession::writeDataToFile(std::shared_ptr<std::vector<char>> data, std::shared_ptr<IoFile> file, std::function<void(void)> fetch_more)
  {
    file_rw_strand_.post([me = shared_from_this(), data, file, fetch_more]
                        {
                          fetch_more();
                          file->file_stream_.write(data->data(), static_cast<std::streamsize>(data->size()));
                        });
  }

  void FtpSession::endDataReceiving(std::shared_ptr<IoFile> file)
  {
    file_rw_strand_.post([me = shared_from_this(), file]
                        {
                          file->file_stream_.flush();
                          file->file_stream_.close();
                          me->sendFtpMessage(FtpReplyCode::CLOSING_DATA_CONNECTION, "Done");
                        });
  }

  ////////////////////////////////////////////////////////
  // Helpers
  ////////////////////////////////////////////////////////

  std::string FtpSession::toAbsoluteFtpPath(const std::string& rel_or_abs_ftp_path) const
  {
    std::string absolute_ftp_path;

    if (!rel_or_abs_ftp_path.empty() && (rel_or_abs_ftp_path[0] == '/'))
    {
      absolute_ftp_path = rel_or_abs_ftp_path;
    }
    else
    {
      absolute_ftp_path = fineftp::Filesystem::cleanPath(ftp_working_directory_ + "/" + rel_or_abs_ftp_path, false, '/');
    }

    return absolute_ftp_path;
  }

  std::string FtpSession::toLocalPath(const std::string& ftp_path) const
  {
    assert(logged_in_user_);

    // First make the ftp path absolute if it isn't already
    std::string absolute_ftp_path = toAbsoluteFtpPath(ftp_path);

    // Now map it to the local filesystem
    return fineftp::Filesystem::cleanPathNative(logged_in_user_->local_root_path_ + "/" + absolute_ftp_path);
  }

  std::string FtpSession::createQuotedFtpPath(const std::string& unquoted_ftp_path) const
  {
    std::string output;
    output.reserve(unquoted_ftp_path.size() * 2 + 2);
    output.push_back('\"');

    for (char c : unquoted_ftp_path)
    {
      output.push_back(c);
      if (c == '\"')            // Escape quote by double-quote
        output.push_back(c);
    }

    output.push_back('\"');

    return output;
  }

  FtpMessage FtpSession::checkIfPathIsRenamable(const std::string& ftp_path) const
  {
    if (!logged_in_user_) return FtpMessage(FtpReplyCode::NOT_LOGGED_IN, "Not logged in");

    if (!ftp_path.empty())
    {
      // Is the given path a file or a directory?
      auto file_status = Filesystem::FileStatus(toLocalPath(ftp_path));

      if (file_status.isOk())
      {
        // Calculate required permissions to rename the given resource
        Permission required_permissions;
        if (file_status.type() == Filesystem::FileType::Dir)
        {
          required_permissions = Permission::DirRename;
        }
        else
        {
          required_permissions = Permission::FileRename;
        }

        // Send error if the permisions are insufficient
        if ((logged_in_user_->permissions_ & required_permissions) != required_permissions)
        {
          return FtpMessage(FtpReplyCode::ACTION_NOT_TAKEN, "Permission denied");
        }

        return FtpMessage(FtpReplyCode::COMMAND_OK, "");
      }
      else
      {
        return FtpMessage(FtpReplyCode::ACTION_NOT_TAKEN, "File does not exist");
      }
    }
    else
    {
      return FtpMessage(FtpReplyCode::SYNTAX_ERROR_PARAMETERS, "Empty path");
    }
  }

  FtpMessage FtpSession::executeCWD(const std::string& param)
  {
    if (!logged_in_user_)
    {
      return FtpMessage(FtpReplyCode::NOT_LOGGED_IN,    "Not logged in");
    }
    if (static_cast<int>(logged_in_user_->permissions_ & Permission::DirList) == 0)
    {
      return FtpMessage(FtpReplyCode::ACTION_NOT_TAKEN, "Permission denied");
    }


    if (param.empty())
    {
      return FtpMessage(FtpReplyCode::SYNTAX_ERROR_PARAMETERS, "No path given");
    }

    std::string absolute_new_working_dir;

    if (param[0] == '/')
    {
      // Absolute path given
      absolute_new_working_dir = fineftp::Filesystem::cleanPath(param, false, '/');
    }
    else
    {
      // Make the path abolute
      absolute_new_working_dir = fineftp::Filesystem::cleanPath(ftp_working_directory_ + "/" + param, false, '/');
    }

    auto local_path = toLocalPath(absolute_new_working_dir);
    Filesystem::FileStatus file_status(local_path);

    if (!file_status.isOk())
    {
      return FtpMessage(FtpReplyCode::ACTION_NOT_TAKEN, "Failed ot change directory: The given resource does not exist or permission denied.");
    }
    if (file_status.type() != Filesystem::FileType::Dir)
    {
      return FtpMessage(FtpReplyCode::ACTION_NOT_TAKEN, "Failed ot change directory: The given resource is not a directory.");
    }
    if (!file_status.canOpenDir())
    {
      return FtpMessage(FtpReplyCode::ACTION_NOT_TAKEN, "Failed ot change directory: Permission denied.");
    }
    ftp_working_directory_ = absolute_new_working_dir;
    return FtpMessage(FtpReplyCode::FILE_ACTION_COMPLETED, "Working directory changed to " + ftp_working_directory_);
  }

#ifdef WIN32
  std::string FtpSession::GetLastErrorStr() const
  {
    DWORD error = GetLastError();
    if (error)
    {
      LPVOID lp_msg_buf;
      DWORD buf_len = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
                                    FORMAT_MESSAGE_FROM_SYSTEM |
                                    FORMAT_MESSAGE_IGNORE_INSERTS,
                                    NULL,
                                    error,
                                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                                    (LPTSTR) &lp_msg_buf,
                                    0, NULL );
      if (buf_len)
      {
        LPCSTR lp_msg_str = (LPCSTR)lp_msg_buf;
        std::string result(lp_msg_str, lp_msg_str + buf_len);
        result.erase(std::remove_if(result.begin(),
                                    result.end(),
                                    [](unsigned char x) {return std::iscntrl(x); }),
                     result.end()); //remove CRLF
        LocalFree(lp_msg_buf);

        return result;
      }
    }

    return ""; 
  }
#endif //WIN32
}
