#pragma once

#include <chrono>
#include <regex>
#include <iostream>
#include <map>

#include <sys/stat.h>

////////////////////////////////////////////////////////////////////////////////
/// Filesystem
////////////////////////////////////////////////////////////////////////////////

namespace fineftp
{
  namespace Filesystem
  {
    enum class FileType
    {
      Unknown,
      RegularFile,
      Dir,
      CharacterDevice,
      BlockDevice,
      Fifo,
      SymbolicLink,
      Socket
    };

    class FileStatus
    {
    public:
      FileStatus(const std::string& path);

      ~FileStatus();

      bool isOk() const;
      FileType type() const;

      int64_t fileSize() const;

  #ifdef WIN32
      bool permissionRootRead()     const;
      bool permissionRootWrite()    const;
      bool permissionRootExecute()  const;
      bool permissionGroupRead()    const;
      bool permissionGroupWrite()   const;
      bool permissionGroupExecute() const;
      bool permissionOwnerRead()    const;
      bool permissionOwnerWrite()   const;
      bool permissionOwnerExecute() const;
  #else // WIN32
      bool permissionRootRead()     const;
      bool permissionRootWrite()    const;
      bool permissionRootExecute()  const;
      bool permissionGroupRead()    const;
      bool permissionGroupWrite()   const;
      bool permissionGroupExecute() const;
      bool permissionOwnerRead()    const;
      bool permissionOwnerWrite()   const;
      bool permissionOwnerExecute() const;
  #endif // WIN32


      std::string permissionString() const;

      std::string ownerString() const;

      std::string groupString() const;

      std::string timeString() const;

      bool canOpenDir() const;



    private:
      std::string path_;
      bool is_ok_;
  #ifdef WIN32
      struct __stat64 file_status_;
  #else // WIN32
      struct stat file_status_;
  #endif 
    };

    std::map<std::string, FileStatus> dirContent(const std::string& path);

    std::string cleanPath(const std::string& path, bool windows_path, const char output_separator);

    std::string cleanPathNative(const std::string& path);
  }
}