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
// OVERVIEW: SymbolMap.h
// ========
// Class definition for symbol map.
//
// Author: Paulo Pagliosa
// Last revision: 17/05/2023

#ifndef __SymbolMap_h
#define __SymbolMap_h

#include "symbol/Symbol.h"
#include <cassert>
#include <map>
#include <concepts>

namespace calc::symbol
{ // begin namespace calc::symbol

using SymbolMapBase = std::map<String, SymbolPtr>;


/////////////////////////////////////////////////////////////////////
//
// SymbolMap: symbol map class
// =========
template <std::derived_from<Symbol> T>
class SymbolMap
{
public:
  void add(const T* symbol)
  {
    assert(symbol != nullptr);
    _map.insert({symbol->name(), symbol});
  }

  auto find(const String& name) const
  {
    auto it = _map.find(name);
    return it != _map.end() ? (T*)it->second.get() : nullptr;
  }

  auto remove(const String& name)
  {
    return _map.erase(name) != 0;
  }

  void clear()
  {
    _map.clear();
  }

  auto size() const
  {
    return _map.size();
  }

  auto empty() const
  {
    return _map.empty();
  }

  auto begin() const
  {
    return _map.begin();
  }

  auto end() const
  {
    return _map.end();
  }

  void write(Writer& writer) const
  {
    for (auto& [name, symbol] : _map)
      symbol->write(writer);
  }

private:
  SymbolMapBase _map;

}; // SymbolMap

} // end namespace calc::symbol

#endif // __SymbolMap_h
