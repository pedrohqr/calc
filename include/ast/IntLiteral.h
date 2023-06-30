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
// OVERVIEW: IntLiteral.h
// ========
// Class definition for int literal.
//
// Author: Paulo Pagliosa
// Last revision: 15/05/2023

#ifndef __IntLiteral_h
#define __IntLiteral_h

#include "ast/MyExpressionValue.h"

namespace calc::ast
{ // begin namespace calc::ast


/////////////////////////////////////////////////////////////////////
//
// IntLiteral: int literal class
// ==========
class IntLiteral: public Expression
{
public:
  const int value;

  IntLiteral(int value):
    value{value}
  {
    setResolvedType(Type::Int());
  }

  void resolve(Scope*) override;
  Value eval(Frame*) const override;

}; // IntLiteral

} // end namespace calc::ast

#endif // __IntLiteral_h
