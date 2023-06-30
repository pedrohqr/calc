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
// OVERVIEW: Assignment.h
// ========
// Class definition for assignment statement.
//
// Author: Paulo Pagliosa
// Last revision: 09/06/2023

#ifndef __Assignment_h
#define __Assignment_h

#include "ast/Reference.h"
#include "ast/Statement.h"

namespace calc::ast
{ // begin namespace calc::ast


/////////////////////////////////////////////////////////////////////
//
// ReferenceList: reference list class
// =============
class ReferenceList: public List<Reference>
{
public:
  void resolve(Scope* scope)
  {
    for (auto r : *this)
      r->resolve(scope);
  }

}; // ReferenceList


/////////////////////////////////////////////////////////////////////
//
// Assignment: assignment statement class
// ==========
class Assignment: public Statement
{
public:
  auto& lhs()
  {
    return _lhs;
  }

  void setExpression(Expression* expression)
  {
    this->_expression = expression;
    _reference = dynamic_cast<Reference*>(expression);
  }

  void resolve(Scope*) override;
  JumpCode execute(Frame*) const override;

private:
  ReferenceList _lhs;
  ExpressionPtr _expression;
  Reference* _reference;

  static void setValue(Frame*, Reference*, const Expression::Value&);

}; // Assignment

using AssignmentPtr = util::ObjectPtr<Assignment>;

} // end namespace calc::ast

#endif // __Assignment_h
