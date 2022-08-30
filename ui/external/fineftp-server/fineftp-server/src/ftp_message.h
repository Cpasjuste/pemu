#pragma once

#include <string>

namespace fineftp
{
  enum class FtpReplyCode : int
  {
    // Reply codes from RFC 959 (plain FTP)
    // https://tools.ietf.org/html/rfc959

    COMMAND_OK                                  = 200,
    SYNTAX_ERROR_UNRECOGNIZED_COMMAND           = 500,
    SYNTAX_ERROR_PARAMETERS                     = 501,
    COMMAND_NOT_IMPLEMENTED_SUPERFLOUS          = 202,
    COMMAND_NOT_IMPLEMENTED                     = 502,
    COMMANDS_BAD_SEQUENCE                       = 503,
    COMMAND_NOT_IMPLEMENTED_FOR_PARAMETER       = 504,

    REPLY_RESTART_MARKER                        = 110,
    REPLY_SYSTEM_STATUS                         = 211,
    DIRECTORY_STATUS                            = 212,
    FILE_STATUS                                 = 213,
    HELP_MESSAGE                                = 214,
    NAME_SYSTEM_TYPE                            = 215,

    SERVICE_READY_IN_MINUTES                    = 120,
    SERVICE_READY_FOR_NEW_USER                  = 220,
    SERVICE_CLOSING_CONTROL_CONNECTION          = 221,
    SERVICE_NOT_AVAILABLE                       = 421,
    DATA_CONNECTION_OPEN_TRANSFER_STARTING      = 125,
    DATA_CONNECTION_OPEN_NO_TRANSFER            = 225,
    ERROR_OPENING_DATA_CONNECTION               = 425,
    CLOSING_DATA_CONNECTION                     = 226,
    TRANSFER_ABORTED                            = 426,
    ENTERING_PASSIVE_MODE                       = 227,

    USER_LOGGED_IN                              = 230,
    NOT_LOGGED_IN                               = 530,
    USER_NAME_OK                                = 331,
    NEED_ACCOUNT_FOR_LOGIN                      = 332,
    NEED_ACCOUNT_FOR_STORING_FILES              = 532,
    FILE_STATUS_OK_OPENING_DATA_CONNECTION      = 150,
    FILE_ACTION_COMPLETED                       = 250,
    PATHNAME_CREATED                            = 257,
    FILE_ACTION_NEEDS_FURTHER_INFO              = 350,
    FILE_ACTION_NOT_TAKEN                       = 450,
    ACTION_NOT_TAKEN                            = 550,
    ACTION_ABORTED_LOCAL_ERROR                  = 451,
    ACTION_ABORTED_PAGE_TYPE_UNKNOWN            = 551,
    ACTION_NOT_TAKEN_INSUFFICIENT_STORAGE_SPACE = 452,
    FILE_ACTION_ABORTED                         = 552,
    ACTION_NOT_TAKEN_FILENAME_NOT_ALLOWED       = 553,
  };

  class FtpMessage
  {
  public:
    FtpMessage(FtpReplyCode code, const std::string& message)
      : code_(code)
      , message_(message)
    {}

    ~FtpMessage()
    {}

    inline FtpReplyCode replyCode() const
    {
      return code_;
    }

    inline std::string message() const
    {
      return message_;
    }

    inline std::string str() const
    {
      return std::to_string(static_cast<int>(code_)) + " " + message_ + "\r\n";
    }

  private:
    FtpReplyCode code_;
    std::string  message_;
  };
}