#ifndef __Exception_h
#define __Exception_h

#include <cstdarg>
#include <stdexcept>

namespace util
{ // begin namespace util

extern std::string errorMessage(const char*, va_list);

template <typename E = std::runtime_error>
inline auto
error(const char* format...)
{
  va_list args;

  va_start(args, format);
  throw E{errorMessage(format, args)};
}

} // end namespace util

#endif // __Exception_h
