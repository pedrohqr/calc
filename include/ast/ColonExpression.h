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
// OVERVIEW: ColonExpression.h
// ========
// Class definition for colon expression.
//
// Author: Paulo Pagliosa
// Last revision: 15/05/2023

#ifndef __ColonExpression_h
#define __ColonExpression_h

#include "ast/MyExpressionValue.h"

namespace calc::ast
{ // begin namespace calc::ast


/////////////////////////////////////////////////////////////////////
//
// ColonExpression: colon expression class
// ===============
class ColonExpression: public Expression
{
public:
  ColonExpression(Expression* e1, Expression* e3, Expression* e2):
    _e1(e1),
    _e3(e3),
    _e2(e2)
  {
    // do nothing
  }

  void resolve(Scope*) override;
  Value eval(Frame*) const override;

private:
  ExpressionPtr _e1;
  ExpressionPtr _e3;
  ExpressionPtr _e2;

}; // ColonExpression

} // end namespace calc::ast

#endif // __ColonExpression_h
