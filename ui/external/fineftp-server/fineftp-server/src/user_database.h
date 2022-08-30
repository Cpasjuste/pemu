#pragma once

#include <mutex>
#include <map>
#include <string>
#include <memory>

#include "ftp_user.h"

namespace fineftp
{
  class UserDatabase
  {
  public:
    UserDatabase();
    ~UserDatabase();

    bool addUser(const std::string& username, const std::string& password, const std::string& local_root_path, Permission permissions);

    std::shared_ptr<FtpUser> getUser(const std::string& username, const std::string& password) const;

  private:
    bool isUsernameAnonymousUser(const std::string& username) const;

    mutable std::mutex                              database_mutex_;
    std::map<std::string, std::shared_ptr<FtpUser>> database_;
    std::shared_ptr<FtpUser>                        anonymous_user_;
  };
}