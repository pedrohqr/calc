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
// OVERVIEW: ProblemReporter.cpp
// ========
// Soource file for problem reporter.
//
// Author: Paulo Pagliosa
// Last revision: 17/05/2023

#include "ProblemReporter.h"
#include "ast/Reference.h"

namespace calc
{ // begin namespace calc


/////////////////////////////////////////////////////////////////////
//
// ProblemReporter implementation
// ===============
ProblemReporter problemReporter;

DEFINE_ERROR_MESSAGE_TABLE(ProblemReporter, ErrorHandler)
ERROR_MESSAGE(UNDEFINED_VARIABLE,
  "Undefined variable '%s'"),
  ERROR_MESSAGE(UNDEFINED_VARIABLE_OR_FUNCTION,
    "Undefined variable or function '%s'"),
  ERROR_MESSAGE(FUNCTION_IS_ALREADY_DEFINED,
    "Function '%s' is already defined"),
  ERROR_MESSAGE(TOO_MANY_ARGUMENTS,
    "Too many arguments"),
  ERROR_MESSAGE(TOO_MANY_OUTPUT_ARGUMENTS,
    "Too many output arguments"),
  END_ERROR_MESSAGE_TABLE;

void
ProblemReporter::undefinedVariable(Scope* scope, Reference* r)
//[]----------------------------------------------------[]
//|  Undefined variable                                  |
//[]----------------------------------------------------[]
{
  error(UNDEFINED_VARIABLE, r->name().c_str());
}

void
ProblemReporter::undefinedVariableOrFunction(Scope* scope, Reference* r)
//[]----------------------------------------------------[]
//|  Undefined variable or function                      |
//[]----------------------------------------------------[]
{
  error(UNDEFINED_VARIABLE_OR_FUNCTION, r->name().c_str());
}

void
ProblemReporter::functionIsAlreadyDefined(Scope* scope, const String& name)
//[]----------------------------------------------------[]
//|  Function is already defined                         |
//[]----------------------------------------------------[]
{
  error(FUNCTION_IS_ALREADY_DEFINED, name.c_str());
}

void
ProblemReporter::tooManyArguments(Scope* scope, Expression* e)
//[]----------------------------------------------------[]
//|  Too many arguments                                  |
//[]----------------------------------------------------[]
{
  (void)e;
  error(TOO_MANY_ARGUMENTS);
}

void
ProblemReporter::tooManyOutputArguments(Scope* scope, Expression* e)
//[]----------------------------------------------------[]
//|  Too many outout arguments                           |
//[]----------------------------------------------------[]
{
  (void)e;
  error(TOO_MANY_OUTPUT_ARGUMENTS);
}

} // end namespace calc
