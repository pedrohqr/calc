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
// OVERVIEW: ProblemReporter.h
// ========
// Class definition for problem reporter.
//
// Author: Paulo Pagliosa
// Last revision: 17/05/2023

#ifndef __ProblemReporter_h
#define __ProblemReporter_h

#include "parser/ErrorHandler.h"

namespace calc
{ // begin namespace calc

//
// Forward definitions
//
namespace ast
{ // begin namespace ast

class Expression;
class Reference;

} // end namespace ast

namespace symbol { class Scope; }

using namespace ast;
using namespace symbol;


/////////////////////////////////////////////////////////////////////
//
// ProblemReporter: problem report class
// ===============
class ProblemReporter: public ErrorHandler
{
public:
  void undefinedVariable(Scope*, Reference*);
  void undefinedVariableOrFunction(Scope*, Reference*);
  void functionIsAlreadyDefined(Scope*, const String&);
  void tooManyArguments(Scope*, Expression*);
  void tooManyOutputArguments(Scope*, Expression*);

private:
  enum
  {
    UNDEFINED_VARIABLE,
    UNDEFINED_VARIABLE_OR_FUNCTION,
    FUNCTION_IS_ALREADY_DEFINED,
    TOO_MANY_ARGUMENTS,
    TOO_MANY_OUTPUT_ARGUMENTS
  };

  DECLARE_ERROR_MESSAGE_TABLE(ProblemReporter);

}; // ProblemReporter

//
// Global problem reporter
//
extern ProblemReporter problemReporter;

} // end namespace calc

#endif // __ProblemReporter_h
