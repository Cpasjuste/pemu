#include <fineftp/server.h>

#include "server_impl.h"
#include <memory>

namespace fineftp
{
  FtpServer::FtpServer(const std::string& address, uint16_t port)
    : ftp_server_(std::make_unique<FtpServerImpl>(address, port))
  {}

  FtpServer::FtpServer(uint16_t port)
    : FtpServer(std::string("0.0.0.0"), port)
  {}

  FtpServer::~FtpServer()
  {}

  bool FtpServer::addUser(const std::string& username, const std::string& password, const std::string& local_root_path, const Permission permissions)
  {
    return ftp_server_->addUser(username, password, local_root_path, permissions);
  }

  bool FtpServer::addUserAnonymous(const std::string& local_root_path, const Permission permissions)
  {
    return ftp_server_->addUserAnonymous(local_root_path, permissions);
  }

  bool FtpServer::start(size_t thread_count)
  {
    assert(thread_count > 0);
    return ftp_server_->start(thread_count);
  }

  void FtpServer::stop()
  {
    ftp_server_->stop();
  }

  int FtpServer::getOpenConnectionCount() const
  {
    return ftp_server_->getOpenConnectionCount();
  }

  uint16_t FtpServer::getPort() const
  {
    return ftp_server_->getPort();
  }

  std::string FtpServer::getAddress() const
  {
    return ftp_server_->getAddress();
  }
}
