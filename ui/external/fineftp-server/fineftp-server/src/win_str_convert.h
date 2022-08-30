#include <string>

namespace fineftp
{
  namespace StrConvert
  {
#ifdef WIN32
    std::string WideToAnsi(const std::wstring& wstr);

    std::wstring AnsiToWide(const std::string& str);

    std::string WideToUtf8(const std::wstring& wstr);

    std::wstring Utf8ToWide(const std::string& str);

    std::string AnsiToUtf8(const std::string& str);

    std::string Utf8ToAnsi(const std::string& str);
#endif // WIN32
  }
}