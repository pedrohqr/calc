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
// OVERVIEW: ErrorHandler.h
// ========
// Class definition for generic error handler.
//
// Author: Paulo Pagliosa
// Last revision: 17/05/2023

#ifndef __ErrorHandler_h
#define __ErrorHandler_h

#include "util/Exception.h"

namespace calc
{ // begin namespace calc

using String = std::string;


/////////////////////////////////////////////////////////////////////
//
// ErrorMessageTableEntry: error message table entry
// ======================
class ErrorMessageTableEntry
{
public:
  int code;
  const char* message;

}; // ErrorMessageTableEntry

//
// Auxiliary function
//
const char* searchErrorMessage(ErrorMessageTableEntry*, int);

//
// Macros to declare a error message table
//
#define DECLARE_ERROR_MESSAGE_TABLE(cls) \
private: \
  static ErrorMessageTableEntry _errmsg[]; \
protected: \
  const char* findErrorMessage(int) const override

#define DEFINE_ERROR_MESSAGE_TABLE(cls, base) \
const char* \
cls::findErrorMessage(int code) const \
{ \
  auto msg = searchErrorMessage(_errmsg, code); \
  if (msg == nullptr) \
    msg = base::findErrorMessage(code); \
  return msg; \
} \
DEFINE_ERROR_MESSAGE_TABLE_ENTRIES(cls)

#define DEFINE_ERROR_MESSAGE_TABLE_ENTRIES(cls) \
ErrorMessageTableEntry cls::_errmsg[] = {

#define ERROR_MESSAGE(code, msg) \
{code, ##msg}

#define END_ERROR_MESSAGE_TABLE \
{-1, 0}}


/////////////////////////////////////////////////////////////////////
//
// ErrorHandler: generic error handler class
// ============
class ErrorHandler
{
public:
  void error(int, ...) const;

protected:
  virtual void handleError(int, va_list) const;
  virtual String errorMessageFormat(const char*) const;
  virtual const char* findErrorMessage(int) const;
  virtual void throwErrorMessage(const char*) const;

  const char* errorMessage(int) const;

}; // ErrorHandler

} // end namespace calc

#endif // __ErrorHandler_h
