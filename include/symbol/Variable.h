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
// OVERVIEW: Variable.h
// ========
// Class definition for variable.
//
// Author: Paulo Pagliosa
// Last revision: 18/05/2023

#ifndef __Variable_h
#define __Variable_h

#include "ast/MyExpressionValue.h"
#include "symbol/Symbol.h"

namespace calc::symbol
{ // begin namespace calc::symbol


//////////////////////////////////////////////////////////
//
// Variable: variable class
// ========
class Variable: public Symbol
{
public:
  Variable(const String& name):
    Symbol{name}
  {
    // do nothing
  }

  auto type() const
  {
    return _type;
  }

protected:
  Type* _type{Type::Variant()};

}; // Variable

using VariablePtr = util::ObjectPtr<Variable>;

} // end namespace calc::symbol

#endif // __Variable_h
