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
// OVERVIEW: Reference.h
// ========
// Class definition for generic reference.
//
// Author: Paulo Pagliosa
// Last revision: 09/06/2023

#ifndef __Reference_h
#define __Reference_h

#include "ast/ExpressionList.h"
#include "symbol/Function.h"

namespace calc::ast
{ // begin namespace calc::ast

class Assignment;


/////////////////////////////////////////////////////////////////////
//
// Reference: generic reference class
// =========
class Reference: public Expression
{
public:
  enum class Place
  {
    RHS,
    LHS
  };

  using enum Place;

  symbol::Variable* variable{};
  symbol::Function* function{};

  Reference(const String& name, Place place = RHS):
    _name{name},
    _place{place}
  {
    // do nothing
  }

  auto& name() const
  {
    return _name;
  }

  auto& arguments()
  {
    return _arguments;
  }

  void resolve(Scope*) override;
  Value eval(Frame*) const override;

private:
  struct Index
  {
    bool colon;
    Expression::Value value;
  };

  String _name;
  ExpressionList _arguments;
  Place _place;

  static Index evalIndex(Frame*, Expression*);

  friend Assignment;

}; // Reference

using ReferencePtr = util::ObjectPtr<Reference>;

} // end namespace calc::ast

#endif // __Reference_h
