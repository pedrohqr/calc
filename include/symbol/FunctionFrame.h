//
// OVERVIEW: FunctionFrame.h
// ========
// Class definition for function frame.
//
// Author: Pedro Ramos and Bruno Kakazu
// Last revision: 04/07/2023

#ifndef __Function_frame_h
#define __Function_frame_h

#include "Frame.h"
#include "ast/MyExpressionValue.h"
#include "ast/Reference.h"
#include "MiscFunctions.h"

namespace calc
{
using namespace ast;

using ValueFunc = std::function<bool(Frame&, const ExpressionList&, Expression::Value&)>;

class FunctionFrame
{
public:
	FunctionFrame(Frame& frame, const ExpressionList& args):
		_frame{frame},
		_args{args}
	{
		// do nothing
	}

	inline
	bool callBuiltInFunction(const String& funcName, Expression::Value& value)
	{		
		if (builtinFunctions.count(funcName) > 0)
			return builtinFunctions[funcName](_frame, _args, value);
		return false;
	}
private:
	Frame& _frame;
	const ExpressionList& _args;
	std::map<String, ValueFunc> builtinFunctions =
	{
		{"eye", eye},
		{"zeros", zeros},
		{"ones", ones},
		{"diag", diag}
	};
};

}

#endif // !__Function_frame_h
