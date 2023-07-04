#ifndef __Misc_func_h
#define __Misc_func_h

#include "Frame.h"
#include "ast/Reference.h"

namespace calc::symbol
{
using namespace calc::ast;

bool eye(Frame&, const ExpressionList&, Expression::Value&);
bool zeros(Frame&, const ExpressionList&, Expression::Value&);
bool ones(Frame&, const ExpressionList&, Expression::Value&);
bool diag(Frame&, const ExpressionList&, Expression::Value&);

}

#endif // !__Misc_func_h
