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
// OVERVIEW: Type.h
// ========
// Class definition for generic type.
//
// Author: Paulo Pagliosa
// Last revision: 07/06/2023

#ifndef __Type_h
#define __Type_h

#include "symbol/Symbol.h"

namespace calc::symbol
{ // begin namespace calc::symbol


/////////////////////////////////////////////////////////////////////
//
// Type: generic type class
// ====
class Type: public Symbol
{
public:
  static const auto Void()
  {
    static Type _void{"void"};
    return &_void;
  }

  static const auto Int()
  {
    static Type _int{"int"};
    return &_int;
  }

  static const auto Float()
  {
    static Type _float{"float"};
    return &_float;
  }

  static const auto Variant()
  {
    static Type _variant{"variant"};
    return &_variant;
  }

private:
  Type(const char* name):
    Symbol{name}
  {
    makeUse(this);
  }

}; // Type

inline auto
isVoid(const Type* type)
{
  return type == Type::Void();
}

inline auto
isInt(const Type* type)
{
    return type == Type::Int();
}

inline auto
isFloat(const Type* type)
{
    return type == Type::Float();
}

using TypePtr = util::ObjectPtr<Type>;

} // end namespace calc::symbol

#endif // __Type_h
