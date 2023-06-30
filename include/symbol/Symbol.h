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
// OVERVIEW: Symbol.h
// ========
// Class definition for generic symbol.
//
// Author: Paulo Pagliosa
// Last revision: 17/05/2023

#ifndef __Symbol_h
#define __Symbol_h

#include "util/SharedObject.h"
#include <string>

namespace calc
{ // begin namespace calc

//
// Forward definition
//
class Writer;

namespace symbol
{ // begin namespace symbol

using String = std::string;


/////////////////////////////////////////////////////////////////////
//
// Symbol: generic symbol class
// ======
class Symbol: public util::SharedObject
{
public:
  virtual ~Symbol() = default;

  auto name() const
  {
    return _name.c_str();
  }

  virtual void write(Writer&) const;

protected:
  Symbol(const String& name):
    _name{name}
  {
    // do nothing
  }

private:
  String _name;

}; // Symbol

using SymbolPtr = util::ObjectPtr<Symbol>;

} // end namespace symbol

} // end namespace calc

#endif // __Symbol_h
