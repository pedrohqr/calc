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
// OVERVIEW: MatrixExpression.h
// ========
// Class definition for matrix expression.
//
// Author: Paulo Pagliosa
// Last revision: 15/05/2023

#ifndef __MatrixExpression_h
#define __MatrixExpression_h

#include "ast/ExpressionList.h"

namespace calc::ast
{ // begin namespace calc::ast


/////////////////////////////////////////////////////////////////////
//
// MatrixRow: matrix row class
// =========
class MatrixRow: public Expression, public ExpressionList
{
public:
  void resolve(Scope*) override;
  Value eval(Frame*) const override;

}; // MatrixRow

using MatrixRowPtr = util::ObjectPtr<MatrixRow>;
using RowList = List<MatrixRow>;


//////////////////////////////////////////////////////////
//
// MatrixExpression: matrix expression class
// ================
class MatrixExpression: public Expression
{
public:
  auto& rows()
  {
    return _rows;
  }

  void resolve(Scope*) override;
  Value eval(Frame*) const override;

private:
  RowList _rows;

}; // MatrixExpression

using MatrixExpressionPtr = util::ObjectPtr<MatrixExpression>;

} // end namespace calc::ast

#endif // __MatrixExpression_h
