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
// OVERVIEW: InputBuffer.h
// ========
// Class definition for generic input buffer.
//
// Author: Paulo Pagliosa
// Last revision: 15/05/2023

#ifndef __InputBuffer_h
#define __InputBuffer_h

#include "util/SharedObject.h"
#include <string>

namespace calc::parser
{ // begin namespace calc::parser

using String = std::string;


/////////////////////////////////////////////////////////////////////
//
// InputBuffer: generic input buffer class
// ===========
class InputBuffer: public util::SharedObject
{
public:
  virtual const char* name() const = 0;

  void beginLexeme()
  {
    _mark = _current;
  }

  auto lexeme() const
  {
    return String{_mark, (size_t)(_current - _mark)};
  }

  char lookahead() const
  {
    return *_current;
  }

  char operator *() const
  {
    return lookahead();
  }

  char advance()
  {
    return *_current ? *++_current : 0;
  }

  char operator ++(int)
  {
    char temp = lookahead();

    advance();
    return temp;
  }

  char operator ++()
  {
    return advance();
  }

protected:
  char* _start;
  char* _current;
  char* _mark;

  void setSource(char* source)
  {
    _start = _current = _mark = source;
  }

}; // InputBuffer

} // end namespace calc::parser

#endif // __InputBuffer_h
