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
// OVERVIEW: StringBuffer.h
// ========
// Class definition for string buffer.
//
// Author: Paulo Pagliosa
// Last revision: 15/05/2023

#ifndef __StringBuffer_h
#define __StringBuffer_h

#include "parser/InputBuffer.h"

namespace calc::parser
{ // begin namespace calc::parser


/////////////////////////////////////////////////////////////////////
//
// StringBuffer: string buffer class
// ============
class StringBuffer: public InputBuffer
{
public:
  StringBuffer(const char* name):
    _name{name}
  {
    setSource(nullptr);
  }

  void set(const char* source)
  {
    setSource(const_cast<char*>(source));
  }

  const char* name() const override
  {
    return _name;
  }

private:
  const char* _name;

}; // StringBuffer

} // end namespace calc::parser

#endif // __StringBuffer_h
