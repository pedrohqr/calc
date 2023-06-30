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
// OVERVIEW: Parser.h
// ========
// Class definition for generic statement.
//
// Author: Paulo Pagliosa
// Last revision: 16/05/2023

#ifndef __Statement_h
#define __Statement_h

#include "ast/ASTNode.h"

namespace calc
{ // begin namespace calc

//
// Forward definitions
//
class Frame;
namespace symbol { class Scope; }

using namespace symbol;

namespace ast
{ // begin namespace ast


/////////////////////////////////////////////////////////////////////
//
// Statement: generic statement class
// =========
class Statement: public AbstractNode
{
public:
  enum JumpCode
  {
    NEXT,
    RETURN,
    CONTINUE,
    BREAK
  };

  virtual void resolve(Scope*) = 0;
  virtual JumpCode execute(Frame*) const = 0;

}; // Statement

using StatementPtr = util::ObjectPtr<Statement>;

} // end namespace ast

} // end namespace calc

#endif // __Statement_h
