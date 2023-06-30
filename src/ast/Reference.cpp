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
// OVERVIEW: Reference.cpp
// ========
// Source file for reference.
//
// Author: Paulo Pagliosa
// Last revision: 09/06/2023

#include "ast/ColonAtom.h"
#include "Frame.h"
#include "ProblemReporter.h"

namespace calc::ast
{ // begin namespace calc::ast

inline auto
isColonAtom(Expression* e)
{
  return dynamic_cast<ColonAtom*>(e) != nullptr;
}


/////////////////////////////////////////////////////////////////////
//
// Reference implementation
// =========
void
Reference::resolve(Scope* scope)
//[]----------------------------------------------------[]
//|  Resolve                                             |
//[]----------------------------------------------------[]
{
  auto hasColonAtom = false;

  for (auto a : _arguments)
  {
    a->resolveVoid(scope);
    hasColonAtom |= isColonAtom(a);
  }
  if ((variable = scope->lookupVariable(_name)) != nullptr)
  {
    if (_arguments.size() > 2)
      problemReporter.tooManyArguments(scope, this);
  }
  else if (_place == LHS || hasColonAtom)
    problemReporter.undefinedVariable(scope, this);
  else if ((function = scope->lookupFunction(_name)) == nullptr)
    problemReporter.undefinedVariableOrFunction(scope, this);
  else if (_arguments.size() > function->parameters().size())
    problemReporter.tooManyArguments(scope, this);
}

Reference::Index
Reference::evalIndex(Frame* frame, Expression* e)
//[]----------------------------------------------------[]
//|  Eval index                                          |
//[]----------------------------------------------------[]
{
  Index i;

  if (!(i.colon = isColonAtom(e)))
    i.value = e->eval(frame);
  return i;
}

Expression::Value
Reference::eval(Frame* frame) const
//[]----------------------------------------------------[]
//|  Eval                                                |
//[]----------------------------------------------------[]
{
  if (variable != nullptr)
  {
    auto& v = frame->findRecord(_name)->value;

    if (auto nargs = _arguments.size())
    {
      auto a = _arguments.begin();
      auto i = evalIndex(frame, *a);

      if (nargs == 1)
        return i.colon ? v.vector() : v(i.value);
      ++a;

      auto j = evalIndex(frame, *a);

      if (!i.colon)
        return j.colon ? v.rows(i.value) : v(i.value, j.value);
      if (!j.colon)
        return v.cols(j.value);
    }
    return v;
  }
  // TODO
  return Value{};
}

} // end namespace calc::ast
