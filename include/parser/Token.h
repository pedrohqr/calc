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
// OVERVIEW: Token.h
// ========
// Class definition for token.
//
// Author: Paulo Pagliosa
// Last revision: 18/05/2023

#ifndef __Token_h
#define __Token_h

#include "util/SharedObject.h"
#include <cstring>

namespace calc::parser
{ // begin namespace calc::parser

using String = std::string;


/////////////////////////////////////////////////////////////////////
//
// Token: token class
// =====
class Token: public util::SharedObject
{
public:
  int type;
  int line;
  String lexeme;
  union
  {
    int integer;
    float real;
    int op;
    void* value;
  };

  Token():
    type{-1}
  {
    // do nothing
  }

  Token(int type):
    type{type}
  {
    // do nothing
  }

  Token(int type, const String& lexeme):
    type{type},
    lexeme{lexeme}
  {
    // do nothing
  }

private:
  Token* _next{};

  friend class Parser;

}; // Token

} // end namespace calc::parser

#endif // __Token_h
