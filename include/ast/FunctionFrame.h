//
// OVERVIEW: FunctionFrame.h
// ========
// Class definition for function frame.
//
// Author: Pedro Ramos and Bruno Kakazu
// Last revision: 04/07/2023

#ifndef __Function_frame_h
#define __Function_frame_h

#include "ast/MyExpressionValue.h"

namespace calc::ast
{

using ValueFunc = std::function<Expression::Value()>;

Expression::Value identity();

class FunctionFrame
{
public:
private:
	std::map<String, ValueFunc> builtinFunc =
	{
		{"eye", identity}
	};
};

}

#endif // !__Function_frame_h
