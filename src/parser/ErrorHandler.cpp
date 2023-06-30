//[]---------------------------------------------------------------[]
//|                                                                 |
//| Copyright (C) 2010, 2023 Paulo Pagliosa.                        |
//|                                                                 |
//| This software is provided 'as-is', without any express or       |
//| implied warranty. In no event will the authors be held liable   |
//| for any damages arising from the use of this software.          |
//|                                                                 |
//| Permission is granted to anyone to use this software for any    |
//| purpose, including commercial applications, and to alter it and |
//| redistribute it freely, subject to the following restrictions:  |
//|                                                                 |
//| 1. The origin of this software must not be misrepresented; you  |
//| must not claim that you wrote the original software. If you use |
//| this software in a product, an acknowledgment in the product    |
//| documentation would be appreciated but is not required.         |
//|                                                                 |
//| 2. Altered source versions must be plainly marked as such, and  |
//| must not be misrepresented as being the original software.      |
//|                                                                 |
//| 3. This notice may not be removed or altered from any source    |
//| distribution.                                                   |
//|                                                                 |
//[]---------------------------------------------------------------[]
//
// OVERVIEW: ErrorHandler.cpp
// ========
// Source file for generic error handler.
//
// Author: Paulo Pagliosa
// Last revision: 17/05/2023

#include "parser/ErrorHandler.h"

namespace calc
{ // begin namespace calc

//
// Auxiliary function
//
const char*
searchErrorMessage(ErrorMessageTableEntry* entries, int code)
//[]----------------------------------------------------[]
//|  Search error message                                |
//[]----------------------------------------------------[]
{
  while (entries->code != -1)
  {
    if (code == entries->code)
      break;
    entries++;
  }
  return entries->message;
}


/////////////////////////////////////////////////////////////////////
//
// ErrorHandler implementation
// ============
const char*
ErrorHandler::findErrorMessage(int) const
//[]----------------------------------------------------[]
//|  Find error message                                  |
//[]----------------------------------------------------[]
{
  return nullptr;
}

void
ErrorHandler::throwErrorMessage(const char* msg) const
//[]----------------------------------------------------[]
//|  Throw error message                                 |
//[]----------------------------------------------------[]
{
  throw std::runtime_error{msg};
}

void
ErrorHandler::error(int code, ...) const
//[]----------------------------------------------------[]
//|  Error                                               |
//[]----------------------------------------------------[]
{
  va_list args;

  va_start(args, code);
  handleError(code, args);
}

const char*
ErrorHandler::errorMessage(int code) const
//[]----------------------------------------------------[]
//|  Error message                                       |
//[]----------------------------------------------------[]
{
  auto msg = findErrorMessage(code);
  return nullptr == msg ? "undefined error code" : msg;
}

void
ErrorHandler::handleError(int code, va_list args) const
//[]----------------------------------------------------[]
//|  Handle error                                        |
//[]----------------------------------------------------[]
{
  constexpr auto maxLen = 1024;
  char buffer[maxLen];
  auto fmt = errorMessageFormat(errorMessage(code));

  vsnprintf(buffer, maxLen, fmt.c_str(), args);
  throwErrorMessage(buffer);
}

String
ErrorHandler::errorMessageFormat(const char* msg) const
//[]----------------------------------------------------[]
//|  Error Message formar                                |
//[]----------------------------------------------------[]
{
  constexpr auto maxLen = 1024;
  char buffer[maxLen];

  snprintf(buffer, maxLen, "Error: %s", msg);
  return buffer;
}

} // end namespace calc
