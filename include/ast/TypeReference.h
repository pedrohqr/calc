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
// OVERVIEW: TypeReference.h
// ========
// Class definition for type reference.
//
// Author: Paulo Pagliosa
// Last revision: 17/05/2023

#ifndef __TypeReference_h
#define __TypeReference_h

#include "ast/ASTNode.h"
#include "symbol/Type.h"

namespace calc
{ // begin namespace calc

using namespace symbol;

namespace ast
{ // begin namespace ast


/////////////////////////////////////////////////////////////////////
//
// TypeReference: type reference class
// =============
class TypeReference: public AbstractNode
{
public:
  static const auto Int()
  {
    static TypeReference _int{Type::Int()};
    return &_int;
  }

  static const auto Float()
  {
    static TypeReference _float{Type::Float()};
    return &_float;
  }

  auto resolvedType() const
  {
    return _resolvedType;
  }

private:
  const Type* _resolvedType;

  TypeReference(const Type* type):
    _resolvedType{type}
  {
    makeUse(this);
  }

}; // TypeReference

using TypeReferencePtr = util::ObjectPtr<TypeReference>;

} // end namespace ast

} // end namespace calc

#endif // __TypeReference_h
