#include "user_database.h"

#include <iostream>


namespace fineftp
{
  UserDatabase::UserDatabase()
  {}

  UserDatabase::~UserDatabase()
  {}

  bool UserDatabase::addUser(const std::string& username, const std::string& password, const std::string& local_root_path, Permission permissions)
  {
    std::lock_guard<decltype(database_mutex_)> database_lock(database_mutex_);

    if (isUsernameAnonymousUser(username))
    {
      if (anonymous_user_)
      {
        std::cerr << "Error adding user with username \"" << username << "\". The username denotes the anonymous user, which is already present." << std::endl;
        return false;
      }
      else
      {
        anonymous_user_ = std::shared_ptr<FtpUser>(new FtpUser(password, local_root_path, permissions));
#ifndef NDEBUG
        std::cout << "Successfully added anonymous user." << std::endl;
#endif // !NDEBUG
        return true;
      }
    }
    else
    {
      auto user_it = database_.find(username);
      if (user_it == database_.end())
      {
        database_.emplace(username, std::shared_ptr<FtpUser>(new FtpUser(password, local_root_path, permissions)));
#ifndef NDEBUG
        std::cout << "Successfully added user \"" << username << "\"." << std::endl;
#endif // !NDEBUG
        return true;
      }
      else
      {
        std::cerr << "Error adding user with username \"" << username << "\". The user already exists." << std::endl;
        return false;
      }
    }
  }

  std::shared_ptr<FtpUser> UserDatabase::getUser(const std::string& username, const std::string& password) const
  {
    std::lock_guard<decltype(database_mutex_)> database_lock(database_mutex_);

    if (isUsernameAnonymousUser(username))
    {
      return anonymous_user_;
    }
    else
    {
      auto user_it = database_.find(username);
      if (user_it == database_.end())
      {
        return nullptr;
      }
      else
      {
        if (user_it->second->password_ == password)
          return user_it->second;
        else
          return nullptr;
      }
    }
  }

  bool UserDatabase::isUsernameAnonymousUser(const std::string& username) const
  {
    return (username.empty()
      || username == "ftp"
      || username == "anonymous");
  }

}