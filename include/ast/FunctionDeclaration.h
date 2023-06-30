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
// OVERVIEW: FunctionDeclaration.h
// ========
// Class definition for function declaration.
//
// Author: Paulo Pagliosa
// Last revision: 17/05/2023

#ifndef __FunctionDeclaration_h
#define __FunctionDeclaration_h

#include "ast/Declaration.h"

namespace calc
{ // namespace calc

//
// Forward definition
//
namespace symbol { class Function; }

namespace ast
{ // begin namespace ast


/////////////////////////////////////////////////////////////////////
//
// Parameter: parameter class
// =========
class Parameter: public Declaration
{
public:
  using Declaration::Declaration;

}; // Parameter

using ParameterList = List<Parameter>;


/////////////////////////////////////////////////////////////////////
//
// FunctionDeclaration: function declaration class
// ===================
class FunctionDeclaration: public Declaration
{
public:
  symbol::Function* function{};

  FunctionDeclaration():
    Declaration{{}}
  {
    // do nothing
  }

  void setName(const String& name)
  {
    this->_name = name;
  }

  auto& parameters()
  {
    return _parameters;
  }

  auto& output()
  {
    return _output;
  }

  void resolve(Scope*) override;
  JumpCode execute(Frame*) const override;

private:
  ParameterList _parameters;
  ParameterList _output;

}; // FunctionDeclaration

using FunctionDeclarationPtr = util::ObjectPtr<FunctionDeclaration>;

} // end namespace ast

} // end namespace calc

#endif // __FunctionDeclaration_h
