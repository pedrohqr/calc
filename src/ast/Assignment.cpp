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
// Class definition for assignment expression.
//
// Author: Paulo Pagliosa
// Last revision: 09/06/2023

#include "ast/Assignment.h"
#include "Frame.h"
#include "ProblemReporter.h"

namespace calc::ast
{ // begin namespace calc::ast


/////////////////////////////////////////////////////////////////////
//
// Assignment implementation
// ==========
void
Assignment::resolve(Scope* scope)
//[]----------------------------------------------------[]
//|  Resolve                                             |
//[]----------------------------------------------------[]
{
  _expression->resolve(scope);

  size_t rs{1};

  if (_reference != nullptr)
    if (auto function = _reference->function)
      rs = function->output().size();
  if (_lhs.size() > rs)
    problemReporter.tooManyOutputArguments(scope, _expression);
}

//
// Auxiliary function
//
inline auto&
value(Frame* frame, const Variable* v)
{
  return frame->findRecord(v->name())->value;
}

void
Assignment::setValue(Frame* frame, Reference* r, const Expression::Value& rv)
//[]----------------------------------------------------[]
//|  Set value                                           |
//[]----------------------------------------------------[]
{
  auto& varName = r->name();
  auto var = frame->findRecord(varName);

  if (nullptr == var)
  {
    frame->buildVariable(varName);
    r->resolve(frame->scope());
    var = frame->findRecord(varName);
    assert(var != nullptr);
  }
  try
  {
    [&]()
    {
      auto& lv = var->value;
      const auto& args = r->arguments();

      if (auto nargs = args.size())
      {
        auto a = args.begin();
        auto i = r->evalIndex(frame, *a);

        if (nargs == 1)
          return i.colon ? lv.setVector(rv) : lv.set(i.value, rv);
        ++a;

        auto j = r->evalIndex(frame, *a);

        if (!i.colon)
          return j.colon ?
            lv.setRows(i.value, rv) :
            lv.set(i.value, j.value, rv);
        if (!j.colon)
          return lv.setCols(j.value, rv);
      }
      lv = rv;
    }();
  }
  catch (...)
  {
    if (!var->initialized)
      frame->removeVariable(varName);
    throw;
  }
  var->initialized = true;
}

Statement::JumpCode
Assignment::execute(Frame* frame) const
//[]----------------------------------------------------[]
//|  Execute                                             |
//[]----------------------------------------------------[]
{
    // TODO
  setValue(frame, *_lhs.begin(), _expression->eval(frame));
  return NEXT;
}

} // end namespace calc::ast
