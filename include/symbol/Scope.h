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
// OVERVIEW: Scope.h
// ========
// Class definition for generic scope.
//
// Author: Paulo Pagliosa
// Last revision: 18/05/2023

#ifndef __Scope_h
#define __Scope_h

#include "symbol/SymbolMap.h"
#include "symbol/Function.h"

namespace calc
{ // begin namespace calc

//
// Forward definitions
//
namespace ast { class Assigment; }
class Frame;
class Writer;

namespace symbol
{ // begin namespace symbol


/////////////////////////////////////////////////////////////////////
//
// Scope: generic scope class
// =====
class Scope: public util::SharedObject
{
public:
  Scope* parent() const
  {
    return _parent;
  }

  virtual void buildVariable(const String&);
  virtual void buildFunction(ast::FunctionDeclaration*);

  virtual Variable* lookupVariable(const String&) const;
  virtual Function* lookupFunction(const String&) const;

  virtual void write(Writer&) const;

protected:
  Scope* _parent;
  SymbolMap<Variable> _variables;
  SymbolMap<Function> _functions;

  Scope(Scope* parent = nullptr):
    _parent{parent}
  {
    // do nothing
  }

  virtual void addVariable(Variable*);
  virtual void addFunction(Function*);

  friend Frame;
  friend ast::Assigment;

}; // Scope

} // end namespace symbol

} // end namespace calc

#endif // __Scope_h
