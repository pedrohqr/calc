#include "ast/MyExpressionValue.h"

calc::ast::Expression::Value::Value():
	_type{ Type::Void() },
	_value{ FloatMatrix() }
{
	// do nothing
}

calc::ast::Expression::Value::Value(int type):
	_type{ Type::Int() },
	_value{ IntMatrix(1, 1) }
{
	std::get<IntMatrix>(_value)(0, 0) = type;
}

calc::ast::Expression::Value::Value(float type):
	_type{ Type::Float() },
	_value{ FloatMatrix(1, 1) }
{
	std::get<FloatMatrix>(_value)(0, 0) = type;
}

calc::ast::Expression::Value
calc::ast::Expression::Value::castTo(const Type* required_type) const
{
	if (required_type == _type)
		return *this;
	else 
	{
		Expression::Value newValue{*this};

		if (isInt(required_type))
		{
			const FloatMatrix& matrix = std::get<FloatMatrix>(_value);
			IntMatrix tmp{ matrix.rows(), matrix.cols() };

			for (size_t i = 0, s = tmp.rows(); i < s; ++i)
				for (size_t j = 0, t = tmp.cols(); j < t; ++j)
					tmp(i, j) = (int)matrix(i, j);

			newValue._value = tmp;
			newValue._type = Type::Int();
			return newValue;
		}
		else if (isFloat(required_type))
		{
			const IntMatrix& matrix = std::get<IntMatrix>(_value);
			FloatMatrix tmp{ matrix.rows(), matrix.cols() };

			for (size_t i = 0, s = tmp.rows(); i < s; ++i)
				for (size_t j = 0, t = tmp.cols(); j < t; ++j)
					tmp(i, j) = (float)matrix(i, j);

			newValue._value = tmp;
			newValue._type = Type::Float();
			return newValue;
		}
		return *this;
	}
}

calc::ast::Expression::Value
calc::ast::Expression::Value::operator +(const Value& other) const
{
	Expression::Value newValue{*this};

	std::visit([&](auto&& Me, auto&& Other){

			newValue._value = Me + Other;

			if (_type != other._type)
				newValue._type = Type::Float();

		}, _value, other._value);

	return newValue;
}

calc::ast::Expression::Value
calc::ast::Expression::Value::operator -(const Value& other) const
{
	// TODO
	return *this;
}

calc::ast::Expression::Value
calc::ast::Expression::Value::operator *(const Value& other) const
{
	// TODO
	return *this;
}

calc::ast::Expression::Value
calc::ast::Expression::Value::operator /(const Value& other) const
{
	Expression::Value newValue{};
	newValue._type = Type::Float();	

	std::visit([&](auto&& Me, auto&& Other) {

			if (Other.rows() != 1 || Other.cols() != 1)
			{
				std::cerr << "Invalid value to divide.\n";
				return *this;
			}

			newValue._value = FloatMatrix{ Me.rows(), Me.cols() };

			std::decay_t<decltype(Other(0, 0))> sValue{ Other(0, 0) };

			for (size_t i = 0; i < Me.rows(); ++i)
				for (size_t j = 0; j < Me.cols(); ++j)
					std::get<FloatMatrix>(newValue._value)(i, j) = (float) Me(i, j) / sValue;

		}, _value, other._value);

	return newValue;
}

calc::ast::Expression::Value
calc::ast::Expression::Value::operator -() const
{
	Expression::Value newValue{*this};

	std::visit([&](auto&& matrix) {

			for (size_t i = 0, sr = matrix.rows(); i < sr; ++i)
				for (size_t j = 0, sc = matrix.cols(); j < sc; ++j)
					matrix(i, j) = -matrix(i, j);
			newValue._value = matrix;

		}, newValue._value);

	return newValue;
}

calc::ast::Expression::Value
calc::ast::Expression::Value::size() const
{
	Expression::Value newValue{*this};
	IntMatrix tmp{ 1, 2 };

	newValue._type = Type::Int();
		
	std::visit([&](auto&& matrix) {
			tmp(0, 0) = (int) matrix.rows();
			tmp(0, 1) = (int) matrix.cols();

			newValue._value = tmp;
		}, newValue._value);

	return newValue;
}

calc::ast::Expression::Value
calc::ast::Expression::Value::transpose() const
{
	Expression::Value newValue{*this};

	std::visit([&](auto&& Me) {

			std::decay_t<decltype(Me)> tmp{ Me.cols(), Me.rows() };

			for (size_t i = 0; i < Me.rows(); ++i)
				for (size_t j = 0; j < Me.cols(); ++j)
					tmp(j, i) = Me(i, j);

			newValue._value = tmp;

		}, _value);

	return newValue;
}

calc::ast::Expression::Value
calc::ast::Expression::Value::horzcat(const Value& other) const
{
	if (isVoid(_type))
		return Expression::Value{other};
	else if (_type == other._type)
	{
		Expression::Value newValue{*this};

		std::visit([&](auto&& Me, auto&& Other) {

				std::decay_t<decltype(Me)> tmp{ Me.rows(), Me.cols() + Other.rows() };

				for (size_t i = 0, r = Me.rows(); i < r; ++i)
					for (size_t j = 0; j < Me.cols() + Other.cols(); ++j)
						if (j < Me.cols())
							tmp(i, j) = Me(i, j);
						else
							tmp(i, j) = Other(i , j - Me.cols());

				newValue._value = tmp;

			}, _value, other._value);

		return newValue;
	}
	else
		throw std::bad_typeid();

	return *this;
}

calc::ast::Expression::Value
calc::ast::Expression::Value::vertcat(const Value& other) const
{
	if (isVoid(_type))
		return Expression::Value{other};
	else if (_type == other._type)
	{
		Expression::Value newValue{*this};

		std::visit([&](auto&& Me, auto&& Other) {

				std::decay_t<decltype(Me)> tmp{ Me.rows() + Other.rows(), Me.cols() };

				for (size_t i = 0, r = Me.rows() + Other.rows(); i < r; ++i)
					for (size_t j = 0; j < Me.cols(); ++j)
						if (i < Me.rows())
							tmp(i, j) = Me(i, j);
						else
							tmp(i, j) = Other(i - Me.rows(), j);

				newValue._value = tmp;

			}, _value, other._value);

		return newValue;
	}
	else
		throw std::bad_typeid();

	return *this;
}

calc::ast::Expression::Value
calc::ast::Expression::Value::operator ()(const Value& other) const
{
	// TODO
	return *this;
}

calc::ast::Expression::Value
calc::ast::Expression::Value::operator ()(const Value& l1, const Value& l2) const
{
	// TODO
	return *this;
}

calc::ast::Expression::Value
calc::ast::Expression::Value::rows(const Value& r) const
{
	// TODO
	return *this;
}

calc::ast::Expression::Value
calc::ast::Expression::Value::cols(const Value& c) const
{
	// TODO
	return *this;
}

calc::ast::Expression::Value
calc::ast::Expression::Value::vector() const
{
	// TODO
	return *this;
}

void
calc::ast::Expression::Value::set(const Value&, const Value&)
{
	// TODO
}

void
calc::ast::Expression::Value::set(const Value&, const Value&, const Value&)
{
	// TODO
}

void
calc::ast::Expression::Value::setRows(const Value&, const Value&)
{
	// TODO
}

void
calc::ast::Expression::Value::setCols(const Value&, const Value&)
{
	// TODO
}

void
calc::ast::Expression::Value::setVector(const Value&)
{
	// TODO
}

void
calc::ast::Expression::Value::write(Writer& writer) const
{
	writer.endLine();

	std::visit([&](auto&& matrix) {
			matrix.print(writer);
		}, _value);
}

calc::ast::Expression::Value
calc::ast::Expression::Value::colon(const Value&, const Value&, const Value&)
{
	// TODO
	Value a{};
	return a;
}