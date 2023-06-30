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
// OVERVIEW: GlobalScope.h
// ========
// Class definition for global scope.
//
// Author: Paulo Pagliosa
// Last revision: 09/06/2023

#ifndef __GlobalScope_h
#define __GlobalScope_h

#include "Frame.h"

namespace calc::symbol
{ // begin namespace calc::symbol


/////////////////////////////////////////////////////////////////////
//
// GlobalScope: global scope class
// ===========
class GlobalScope: public Scope, public Frame
{
public:
  GlobalScope():
    Frame{this}
  {
    // do nothing
  }

  void clear()
  {
    _variables.clear(), Frame::clear();
  }

private:
  void addVariable(Variable*) override;

}; // GlobalScope

} // end namespace calc::symbol

#endif // __GlobalScope_h
