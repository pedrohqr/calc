#include "util/Exception.h"
#include <cstdio>

namespace util
{ // begin namespace util

std::string
errorMessage(const char* format, va_list args)
{
  constexpr auto maxLen = 1024;
  char buffer[maxLen];

  vsnprintf(buffer, maxLen, format, args);
  return buffer;
}

} // begin namespace util
