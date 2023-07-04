#include "symbol/MiscFunctions.h"

namespace calc::symbol
{

bool
eye(Frame& frame, const ExpressionList& args, Expression::Value& out)
{
	if (args.size() == 1)
	{
		auto a = args.begin();
		auto i = a->eval(&frame);
		out = Expression::Value::identity(i, i);
	}
	else if (args.size() == 2)
	{
		auto a = args.begin();
		auto i = a->eval(&frame);
		++a;
		auto j = a->eval(&frame);
		out = Expression::Value::identity(i, j);
	}
	else
		return false;
	return true;
}

bool
zeros(Frame& frame, const ExpressionList& args, Expression::Value& out)
{
	if (args.size() == 1)
	{
		auto a = args.begin();
		auto i = a->eval(&frame);
		out = Expression::Value::zeros(i, i);
	}
	else if (args.size() == 2)
	{
		auto a = args.begin();
		auto i = a->eval(&frame);
		++a;
		auto j = a->eval(&frame);
		out = Expression::Value::zeros(i, j);
	}
	else
		return false;
	return true;
}

bool
ones(Frame& frame, const ExpressionList& args, Expression::Value& out)
{
	if (args.size() == 1)
	{
		auto a = args.begin();
		auto i = a->eval(&frame);
		out = Expression::Value::ones(i, i);
	}
	else if (args.size() == 2)
	{
		auto a = args.begin();
		auto i = a->eval(&frame);
		++a;
		auto j = a->eval(&frame);
		out = Expression::Value::ones(i, j);
	}
	else
		return false;
	return true;
}

bool
diag(Frame& frame, const ExpressionList& args, Expression::Value& out)
{
	if (args.size() == 1)
	{
		auto a = args.begin();
		auto i = a->eval(&frame);
		out = Expression::Value::diag(i);
	}
	else
		return false;
	return true;
}

}
