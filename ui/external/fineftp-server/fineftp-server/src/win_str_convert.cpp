#include "win_str_convert.h"

#ifdef WIN32
  #define WIN32_LEAN_AND_MEAN
  #define NOMINMAX
  #include <windows.h>
#endif // WIN32

#include <string>

namespace fineftp
{
  namespace StrConvert
  {
#ifdef WIN32
    std::string WideToAnsi(const std::wstring& wstr)
    {
        int count = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), static_cast<int>(wstr.length()), NULL, 0, NULL, NULL);
        std::string str(count, 0);
        WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, &str[0], count, NULL, NULL);
        return str;
    }

    std::wstring AnsiToWide(const std::string& str)
    {
        int count = MultiByteToWideChar(CP_ACP, 0, str.c_str(), static_cast<int>(str.length()), NULL, 0);
        std::wstring wstr(count, 0);
        MultiByteToWideChar(CP_ACP, 0, str.c_str(), static_cast<int>(str.length()), &wstr[0], count);
        return wstr;
    }

    std::string WideToUtf8(const std::wstring& wstr)
    {
        int count = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.length()), NULL, 0, NULL, NULL);
        std::string str(count, 0);
        WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], count, NULL, NULL);
        return str;
    }

    std::wstring Utf8ToWide(const std::string& str)
    {
        int count = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.length()), NULL, 0);
        std::wstring wstr(count, 0);
        MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.length()), &wstr[0], count);
        return wstr;
    }

    std::string AnsiToUtf8(const std::string& str)
    {
      return WideToUtf8(AnsiToWide(str));
    }

    std::string Utf8ToAnsi(const std::string& str)
    {
      return WideToAnsi(Utf8ToWide(str));
    }
#endif
  }
}
