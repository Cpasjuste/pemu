#include "filesystem.h"

#include <list>
#include <sstream>
#include <mutex>
#include <iomanip>

#ifdef WIN32

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <win_str_convert.h>

#else // WIN32

#include <dirent.h>

#endif // WIN32

////////////////////////////////////////////////////////////////////////////////
/// Filesystem
////////////////////////////////////////////////////////////////////////////////

namespace fineftp
{
namespace Filesystem
{

  FileStatus::FileStatus(const std::string& path)
    : path_(path)
  {
#ifdef WIN32
    std::wstring w_path_ = StrConvert::Utf8ToWide(path);
    const int error_code = _wstat64(w_path_.c_str(), &file_status_);
#else // WIN32
    const int error_code = stat(path.c_str(), &file_status_);
#endif // WIN32
    is_ok_ = (error_code == 0);
  }

  FileStatus::~FileStatus() {}

  bool FileStatus::isOk() const
  {
    return is_ok_;
  }

  FileType FileStatus::type() const
  {
    if (!is_ok_)
      return FileType::Unknown;

    switch (file_status_.st_mode & S_IFMT) {
    case S_IFREG:  return FileType::RegularFile;
    case S_IFDIR:  return FileType::Dir;
    case S_IFCHR:  return FileType::CharacterDevice;
#ifndef WIN32
    case S_IFBLK:  return FileType::BlockDevice;
    case S_IFIFO:  return FileType::Fifo;
    case S_IFLNK:  return FileType::SymbolicLink;
    case S_IFSOCK: return FileType::Socket;
#endif // !WIN32
    default:       return FileType::Unknown;
    }

  }

  int64_t FileStatus::fileSize() const
  {
    if (!is_ok_)
      return 0;

    return file_status_.st_size;
  }

#ifdef WIN32
  bool FileStatus::permissionRootRead()     const { return 0 != (file_status_.st_mode & S_IREAD); }
  bool FileStatus::permissionRootWrite()    const { return 0 != (file_status_.st_mode & S_IWRITE); }
  bool FileStatus::permissionRootExecute()  const { return 0 != (file_status_.st_mode & S_IEXEC); }
  bool FileStatus::permissionGroupRead()    const { return 0 != (file_status_.st_mode & S_IREAD); }
  bool FileStatus::permissionGroupWrite()   const { return 0 != (file_status_.st_mode & S_IWRITE); }
  bool FileStatus::permissionGroupExecute() const { return 0 != (file_status_.st_mode & S_IEXEC); }
  bool FileStatus::permissionOwnerRead()    const { return 0 != (file_status_.st_mode & S_IREAD); }
  bool FileStatus::permissionOwnerWrite()   const { return 0 != (file_status_.st_mode & S_IWRITE); }
  bool FileStatus::permissionOwnerExecute() const { return 0 != (file_status_.st_mode & S_IEXEC); }
#else // WIN32
  bool FileStatus::permissionRootRead()     const { return 0 != (file_status_.st_mode & S_IRUSR); }
  bool FileStatus::permissionRootWrite()    const { return 0 != (file_status_.st_mode & S_IWUSR); }
  bool FileStatus::permissionRootExecute()  const { return 0 != (file_status_.st_mode & S_IXUSR); }
  bool FileStatus::permissionGroupRead()    const { return 0 != (file_status_.st_mode & S_IRGRP); }
  bool FileStatus::permissionGroupWrite()   const { return 0 != (file_status_.st_mode & S_IWGRP); }
  bool FileStatus::permissionGroupExecute() const { return 0 != (file_status_.st_mode & S_IXGRP); }
  bool FileStatus::permissionOwnerRead()    const { return 0 != (file_status_.st_mode & S_IROTH); }
  bool FileStatus::permissionOwnerWrite()   const { return 0 != (file_status_.st_mode & S_IWOTH); }
  bool FileStatus::permissionOwnerExecute() const { return 0 != (file_status_.st_mode & S_IXOTH); }
#endif // WIN32


  std::string FileStatus::permissionString() const
  {
    std::string permission_string(9, '-');

    if (!is_ok_)
      return permission_string;

#ifdef WIN32
    // Root
    permission_string[0] = (file_status_.st_mode & S_IREAD)  ? 'r' : '-';
    permission_string[1] = (file_status_.st_mode & S_IWRITE) ? 'w' : '-';
    permission_string[2] = (file_status_.st_mode & S_IEXEC)  ? 'x' : '-';
    // Group
    permission_string[3] = (file_status_.st_mode & S_IREAD)  ? 'r' : '-';
    permission_string[4] = (file_status_.st_mode & S_IWRITE) ? 'w' : '-';
    permission_string[5] = (file_status_.st_mode & S_IEXEC)  ? 'x' : '-';
    // Owner
    permission_string[6] = (file_status_.st_mode & S_IREAD)  ? 'r' : '-';
    permission_string[7] = (file_status_.st_mode & S_IWRITE) ? 'w' : '-';
    permission_string[8] = (file_status_.st_mode & S_IEXEC)  ? 'x' : '-';
#else // WIN32
    // Root
    permission_string[0] = (file_status_.st_mode & S_IRUSR) ? 'r' : '-';
    permission_string[1] = (file_status_.st_mode & S_IWUSR) ? 'w' : '-';
    permission_string[2] = (file_status_.st_mode & S_IXUSR) ? 'x' : '-';
    // Group
    permission_string[3] = (file_status_.st_mode & S_IRGRP) ? 'r' : '-';
    permission_string[4] = (file_status_.st_mode & S_IWGRP) ? 'w' : '-';
    permission_string[5] = (file_status_.st_mode & S_IXGRP) ? 'x' : '-';
    // Owner
    permission_string[6] = (file_status_.st_mode & S_IROTH) ? 'r' : '-';
    permission_string[7] = (file_status_.st_mode & S_IWOTH) ? 'w' : '-';
    permission_string[8] = (file_status_.st_mode & S_IXOTH) ? 'x' : '-';
#endif // WIN32
    return permission_string;
  }

  std::string FileStatus::ownerString() const
  {
    return "fineFTP";
  }

  std::string FileStatus::groupString() const
  {
    return "fineFTP";
  }

  std::string FileStatus::timeString() const
  {
    if (!is_ok_)
      return "Jan  1 1970";

    // The FTP Time format can be:
    // 
    //     MMM DD hh:mm
    //   OR
    //     MMM DD  YYYY
    //   OR
    //     MMM DD YYYY
    //
    // This means, that we can only return the time for files with the same
    // year as the current year.
    // 
    // https://files.stairways.com/other/ftp-list-specs-info.txt

    auto now = std::chrono::system_clock::now();
    time_t now_time_t = std::chrono::system_clock::to_time_t(now);
    //struct tm* now_timeinfo = localtime(&now_time_t);
    //int current_year = now_timeinfo->tm_year;

    //struct tm* file_timeinfo = localtime(&file_status_.st_ctime);
    //int file_year = file_timeinfo->tm_year;



    std::tm now_timeinfo {};
    std::tm file_timeinfo{};

#if defined(__unix__)
    localtime_r(&now_time_t,            &now_timeinfo);
    localtime_r(&file_status_.st_ctime, &file_timeinfo);
#elif defined(_MSC_VER)
    localtime_s(&now_timeinfo,  &now_time_t);
    localtime_s(&file_timeinfo, &file_status_.st_ctime);
#else
    static std::mutex mtx;
    {
      std::lock_guard<std::mutex> lock(mtx);

      now_timeinfo = *std::localtime(&now_time_t);
      file_timeinfo = *std::localtime(&file_status_.st_ctime);
    }
#endif

    int current_year = now_timeinfo.tm_year;
    int file_year    = file_timeinfo.tm_year;


    // Hardcoded english month names, because returning a localized string by strftime here may break certain FTP clients
    static std::string month_names[] =
    {
      "Jan",
      "Feb",
      "Mar",
      "Apr",
      "May",
      "Jun",
      "Jul",
      "Aug",
      "Sep",
      "Oct",
      "Nov",
      "Dec"
    };

    std::stringstream date;

    if (file_year == current_year)
    {
      // We are allowed to return the time!
      date << std::setw( 3 ) << file_timeinfo.tm_mday << " "
           << std::setw( 2 ) << file_timeinfo.tm_hour << ":"
           << std::setw( 2 ) << std::setfill( '0' ) << file_timeinfo.tm_min;
    }
    else
    {
      // We must not return the time, only the date :(
      static constexpr auto tm_year_base_year = 1900;
      date << std::setw( 3 ) << file_timeinfo.tm_mday
           << "  " << ( file_timeinfo.tm_year + tm_year_base_year );
    }

    return month_names[file_timeinfo.tm_mon] + date.str();
  }

  bool FileStatus::canOpenDir() const
  {
    if (!is_ok_)
      return false;

    if (type() != FileType::Dir)
      return false;

    bool can_open_dir(false);
#ifdef WIN32
    std::string find_file_path = path_ + "\\*";
    std::replace(find_file_path.begin(), find_file_path.end(), '/', '\\');

    std::wstring w_find_file_path = StrConvert::Utf8ToWide(find_file_path);

    HANDLE hFind;
    WIN32_FIND_DATAW ffd;
    hFind = FindFirstFileW(w_find_file_path.c_str(), &ffd);
    if (hFind != INVALID_HANDLE_VALUE)
    {
      can_open_dir = true;
    }
    FindClose(hFind);
#else // WIN32
    DIR *dp;
    if ((dp = opendir(path_.c_str())) != NULL)
    {
      can_open_dir = true;
    }
    closedir(dp);
#endif // WIN32

    return can_open_dir;
  }

  std::map<std::string, FileStatus> dirContent(const std::string& path)
  {
    std::map<std::string, FileStatus> content;
#ifdef WIN32
    std::string find_file_path = path + "\\*";
    std::replace(find_file_path.begin(), find_file_path.end(), '/', '\\');

    std::wstring w_find_file_path = StrConvert::Utf8ToWide(find_file_path);

    HANDLE hFind;
    WIN32_FIND_DATAW ffd;
    hFind = FindFirstFileW(w_find_file_path.c_str(), &ffd);
    if (hFind == INVALID_HANDLE_VALUE)
    {
      std::cerr << "FindFirstFile Error" << std::endl;
      return content;
    }

    do
    {
      std::string file_name = StrConvert::WideToUtf8(std::wstring(ffd.cFileName));
      content.emplace(file_name, FileStatus(path + "\\" + file_name));
    } while (FindNextFileW(hFind, &ffd) != 0);
    FindClose(hFind);
#else // WIN32
    DIR *dp;
    struct dirent *dirp;
    if((dp = opendir(path.c_str())) == NULL)
    {
        std::cerr << "Error opening directory: " << strerror(errno) << std::endl;
        return content;
    }

    while ((dirp = readdir(dp)) != NULL)
    {
      content.emplace(std::string(dirp->d_name), FileStatus(path + "/" + std::string(dirp->d_name)));
    }
    closedir(dp);

#endif // WIN32
    return content;
  }

  std::string cleanPath(const std::string& path, bool windows_path, const char output_separator)
  {
    if (path.empty())
    {
      return ".";
    }

    // Find the root for absolute paths

    std::string absolute_root;

    if (windows_path)
    {
      /* On Windows, a root folder can be:
       *    C:\
       *    //Host
       *    \\Host
       */

      std::regex win_local_drive("^[a-zA-Z]\\:");                // Local drive
      std::regex win_network_drive("^[/\\\\]{2}[^/\\\\]+");      // Network path starting with two slashes or backslashes followed by a hostname

      if (std::regex_search(path, win_local_drive))
      {
        // Windows local drive, consisting of drive-letter and colon
        absolute_root = path.substr(0, 2);
      }
      else if (std::regex_search(path, win_network_drive))
      {
        // Window network drive, consisting of \\ and hostname
        size_t sep_pos = path.find_first_of("/\\", 2);
        absolute_root = path.substr(0, sep_pos); // If no seperator was found, this will return the entire string
      }
    }
    else
    {
      // On Unix there is only one root and it is '/'
      if (path[0] == '/')
      {
        absolute_root = '/';
      }
    }

    // Split the path
    std::list<std::string> components;

    if (path.size() >= (absolute_root.size() + 1))
    {
      size_t start, end;
      
      if (absolute_root.empty())
        start = 0;
      else
        start = absolute_root.size();

      do
      {
        if (windows_path)
          end = path.find_first_of("/\\", start);
        else
          end = path.find_first_of("/", start);

        std::string this_component;
        if (end == std::string::npos)
          this_component = path.substr(start);
        else
          this_component = path.substr(start, end - start);
        
        // The components-stack that will increase and shrink depending on the folders and .. elements in the splitted path
        if (this_component.empty() || (this_component == "."))
        {
        }
        else if (this_component == "..")
        {
          if (!absolute_root.empty())
          {
            if (!components.empty())
            {
              // Move one folder up if we are not already at the root
              components.pop_back();
            }
          }
          else
          {
            if (!components.empty() && (components.back() != ".."))
            {
              // Move one folder up by removing it. We must not remove ".." elements that we were not able to resolve previously.
              components.pop_back();
            }
            else
            {
              components.push_back("..");
            }
          }
        }
        else
        {
          components.push_back(this_component);
        }

        if (end == std::string::npos)
          break;
        else
          start = end + 1;

      } while (start < path.size());

      // Join the components again
      if (components.size() == 0 && absolute_root.empty())
      {
          return ".";
      }
    }

    std::stringstream path_ss;
    path_ss << absolute_root;

    if (windows_path && !absolute_root.empty())
    {
      path_ss << output_separator; // The windows drive must be followed by a separator. When referencing a network drive.
    }

    auto comp_it = components.begin();
    while (comp_it != components.end())
    {
      if (comp_it != components.begin())
        path_ss << output_separator;
      
      path_ss << *comp_it;

      comp_it++;
    }

    return path_ss.str();
  }

  std::string cleanPathNative(const std::string& path)
  {
#ifdef WIN32
    bool windows_path = true;
    char separator = '\\';
#else // WIN32
    bool windows_path = false;
    char separator = '/';
#endif // WIN32
    return cleanPath(path, windows_path, separator);
  }
}
}
