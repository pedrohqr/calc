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
// OVERVIEW: Frame.h
// ========
// Class definition for execution frame.
//
// Author: Paulo Pagliosa
// Last revision: 09/06/2023

#ifndef __Frame_h
#define __Frame_h

#include "ast/MyExpressionValue.h"
#include "symbol/Scope.h"

namespace calc
{ // begin namespace calc


/////////////////////////////////////////////////////////////////////
//
// FrameRecord: frame record class
// ===========
class FrameRecord: public Symbol
{
public:
  ast::Expression::Value value;
  bool initialized{};

  FrameRecord(const String& name):
    Symbol{name}
  {
    // do nothing
  }

  void write(Writer&) const override;

}; // FrameRecord

using FrameRecordMap = symbol::SymbolMap<FrameRecord>;


/////////////////////////////////////////////////////////////////////
//
// Frame: execution frame class
// =====
class Frame: protected FrameRecordMap
{
public:
  Frame(Scope* scope, Frame* parent = nullptr):
    _scope{scope},
    _parent{parent}
  {
    // do nothing
  }

  auto parent() const
  {
    return _parent;
  }

  auto* scope() const
  {
    return _scope;
  }

  virtual FrameRecord* findRecord(const String&) const;

  virtual void buildVariable(const String&);

  auto removeVariable(const String& name)
  {
    return _scope->_variables.remove(name) && remove(name);
  }
 
  using FrameRecordMap::write;

protected:
  Frame* _parent;
  Scope* _scope;

  void build();

}; // Frame

} // end namespace calc

#endif // __Frame_h
