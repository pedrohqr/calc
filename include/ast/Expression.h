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
// OVERVIEW: Expression.h
// ========
// Class definition for generic expression.
//
// Author: Paulo Pagliosa
// Last revision: 07/06/2023

#ifndef __Expression_h
#define __Expression_h

#include "ast/ASTNode.h"
#include "symbol/Type.h"

namespace calc
{ // begin namespace calc

//
// Forward definitions
//
class Frame;
namespace symbol { class Scope; }

using namespace symbol;

namespace ast
{ // begin namespace ast


/////////////////////////////////////////////////////////////////////
//
// Expression: generic expression class
// ==========
class Expression: public AbstractNode
{
public:
  class Value;

  int beginLine{};

  virtual void resolve(Scope*) = 0;
  virtual Value eval(Frame*) const = 0;

  const Type* resolveVoid(Scope*);

  const Type* resolvedType() const
  {
    return _resolvedType;
  }

protected:
  Expression() = default;

  void setResolvedType(const Type* type)
  {
    _resolvedType = type;
  }

private:
  TypePtr _resolvedType = Type::Variant();

  friend class Value;

}; // Expression

using ExpressionPtr = util::ObjectPtr<Expression>;

} // end namespace ast

} // end namespace calc

#endif // __Expression_h
