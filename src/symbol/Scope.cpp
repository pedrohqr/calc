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
// OVERVIEW: Scope.cpp
// ========
// Source file for generic scope.
//
// Author: Paulo Pagliosa
// Last revision: 18/05/2023

#include "symbol/Scope.h"
#include "ProblemReporter.h"

namespace calc::symbol
{ // begin namespace calc::symbol


/////////////////////////////////////////////////////////////////////
//
// Scope implementation
// =====
void
Scope::buildVariable(const String& name)
//[]----------------------------------------------------[]
//|  Build variable                                      |
//[]----------------------------------------------------[]
{
  if (lookupVariable(name) == nullptr)
    addVariable(new Variable{name});
}

void
Scope::buildFunction(FunctionDeclaration* f)
//[]----------------------------------------------------[]
//|  Build function                                      |
//[]----------------------------------------------------[]
{
  if (_functions.find(f->name()))
    problemReporter.functionIsAlreadyDefined(this, f->name());
  else
    addFunction(new Function{f});
}

Variable*
Scope::lookupVariable(const String& name) const
//[]----------------------------------------------------[]
//|  Lookup variable                                     |
//[]----------------------------------------------------[]
{
  auto v = _variables.find(name);
  return !v && _parent ? _parent->lookupVariable(name) : v;
}

Function*
Scope::lookupFunction(const String& name) const
//[]----------------------------------------------------[]
//|  Lookup function                                     |
//[]----------------------------------------------------[]
{
  auto f = _functions.find(name);
  return !f && _parent ? _parent->lookupFunction(name) : f;
}

void
Scope::write(Writer& writer) const
//[]----------------------------------------------------[]
//|  Write                                               |
//[]----------------------------------------------------[]
{
  _variables.write(writer);
  _functions.write(writer);
}

void
Scope::addVariable(Variable* variable)
//[]----------------------------------------------------[]
//|  Add variable                                        |
//[]----------------------------------------------------[]
{
  _variables.add(variable);
}

void
Scope::addFunction(Function* function)
//[]----------------------------------------------------[]
//|  Add function                                        |
//[]----------------------------------------------------[]
{
  _functions.add(function);
}

} // end namespace calc::symbol
