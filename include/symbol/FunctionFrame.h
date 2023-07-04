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
#include "MiscFunctions.h"

namespace calc
{
using namespace ast;

using ValueFunc = std::function<bool(Frame&)>;
//using Code = std::function<bool(FunctionFrame&)>;

class FunctionFrame
{
public:
	bool callBuiltInFunction(const String& funcName, Frame& frame)
	{
		if (builtinFunctions.count(funcName) > 0)
			return builtinFunctions[funcName](frame);
		return false;
	}
private:
	std::map<String, ValueFunc> builtinFunctions =
	{
		{"eye", eye},
		{"zeros", zeros},
		{"ones", ones}
	};
};

}

#endif // !__Function_frame_h
