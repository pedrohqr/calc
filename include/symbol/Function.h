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
// OVERVIEW: Function.h
// ========
// Class definition for function.
//
// Author: Paulo Pagliosa
// Last revision: 17/05/2023

#ifndef __Function_h
#define __Function_h

#include "ast/FunctionDeclaration.h"
#include "symbol/Variable.h"

namespace calc::symbol
{ // begin namespace calc::symbol


/////////////////////////////////////////////////////////////////////
//
// Function: function class
// ========
class Function: public Symbol
{
public:
  using Parameters = util::ObjectPtrList<Variable>;

  Function(const String& name):
    Symbol{name}
  {
    // do nothing
  }

  Function(ast::FunctionDeclaration* f):
    Symbol{f->name()}
  {
    // TODO
    f->function = this;
  }

  auto& parameters()
  {
    return _parameters;
  }

  auto& output()
  {
    return _output;
  }

private:
  Parameters _parameters;
  Parameters _output;

}; // Function

} // end namespace calc::symbol

#endif // __Function_h
