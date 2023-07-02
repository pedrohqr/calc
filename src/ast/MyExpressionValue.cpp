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
	Expression::Value newValue{*this};

	std::visit([&](auto&& Me, auto&& Other) {

		newValue._value = Me - Other;

		if (_type != other._type)
			newValue._type = Type::Float();

		}, _value, other._value);

	return newValue;
}

calc::ast::Expression::Value
calc::ast::Expression::Value::operator *(const Value& other) const
{
	// TODO
	Expression::Value tmp{*this};

	if (isInt(_type) && isInt(other._type))
	{
		if (std::get<IntMatrix>(_value).cols() != std::get<IntMatrix>(other._value).rows())
			throw std::length_error("Cols and Rows differents");
		tmp._value = std::get<IntMatrix>(_value) * std::get<IntMatrix>(other._value);
	}
	else if (isFloat(_type) && isFloat(other._type))
	{
		if (std::get<FloatMatrix>(_value).cols() != std::get<FloatMatrix>(other._value).rows())
			throw std::length_error("Cols and Rows differents");
		tmp._value = std::get<FloatMatrix>(_value) * std::get<FloatMatrix>(other._value);
	}
	else if (isFloat(_type) && isInt(other._type))
	{
		if (std::get<FloatMatrix>(_value).cols() != std::get<IntMatrix>(other._value).rows())
			throw std::length_error("Cols and Rows differents");
		tmp._value = std::get<FloatMatrix>(_value) * std::get<FloatMatrix>(other.castTo(_type)._value);
	}
	else if (isInt(_type) && isFloat(other._type))
		throw std::bad_cast();

	return tmp;
}

calc::ast::Expression::Value
calc::ast::Expression::Value::operator /(const Value& other) const
{
	Expression::Value newValue{};
	newValue._type = Type::Float();	

	std::visit([&](auto&& Me, auto&& Other) {
		#ifdef _DEBUG
			if (Other.rows() != 1 || Other.cols() != 1)
				util::error<std::logic_error>("Invalid value to divide");
		#endif // _DEBUG

			newValue._value = FloatMatrix{ Me.rows(), Me.cols() };

			std::decay_t<decltype(Other(0, 0))> sValue{ Other(0, 0) };

			for (size_t i = 0; i < Me.rows(); ++i)
				for (size_t j = 0; j < Me.cols(); ++j)
					std::get<FloatMatrix>(newValue._value)(i, j) = static_cast<float>(Me(i, j)) / sValue ;

		}, _value, other._value);

	return newValue;
}

calc::ast::Expression::Value
calc::ast::Expression::Value::operator -() const
{
	Expression::Value newValue{*this};

	std::visit([&](auto&& matrix) {

			newValue._value = -matrix;

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
			tmp(0, 0) = static_cast<int>( matrix.rows() );
			tmp(0, 1) = static_cast<int>( matrix.cols() );

			newValue._value = tmp;
		}, newValue._value);

	return newValue;
}

calc::ast::Expression::Value
calc::ast::Expression::Value::transpose() const
{
	Expression::Value newValue{*this};

	std::visit([&](auto&& Me) {			

			newValue._value = Me.transpose();

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
		util::error("Matrix value must be same type");

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
		util::error("Matrix value must be same type");
	
	return *this;
}

calc::ast::Expression::Value
calc::ast::Expression::Value::operator ()(const Value& other) const
{	
	Expression::Value newValue{*this};

	std::visit([&](auto&& Me, auto&& Other) {
		#ifdef _DEBUG
			if (isFloat(other._type))
				util::error<std::invalid_argument>("Index cannot be float");
		#endif				
			std::decay_t<decltype(Me)> tmp{ Other.rows(), Other.cols() };

			for (size_t i = 0; i < Other.rows(); ++i)
				for (size_t j = 0; j < Other.cols(); ++j)						
						tmp(i, j) = Me(Other(i, j));


			newValue._value = tmp;

		},_value, other._value);
	
	return newValue;
}

calc::ast::Expression::Value
calc::ast::Expression::Value::operator ()(const Value& l1, const Value& l2) const
{
	Expression::Value newValue{*this};

	std::visit([&](auto&& Me, auto&& L1, auto&& L2) {
#ifdef _DEBUG
		if (isFloat(l1._type))
			util::error<std::invalid_argument>("Left index cannot be float");
		if (isFloat(l2._type))
			util::error<std::invalid_argument>("Right index cannot be float");
#endif	
		const auto sL1 = L1.rows() * L1.cols();
		const auto sL2 = L2.rows() * L2.cols();
		std::decay_t<decltype(Me)> tmp{ sL1, sL2 };
				
		for (size_t i = 0; i < sL1; ++i)
			for (size_t j = 0; j < sL2; ++j)
				tmp(i, j) = Me(L1(i), L2(j));

		newValue._value = tmp;

		}, _value, l1._value, l2._value);

	return newValue;
}

calc::ast::Expression::Value
calc::ast::Expression::Value::rows(const Value& other) const
{
	Expression::Value newValue{*this};
	std::visit([&](auto&& Me, auto&& Other) {
#ifdef _DEBUG
		if (isFloat(other._type))
			util::error<std::invalid_argument>("Index cannot be float");
#endif
		std::decay_t<decltype(Me)> tmp{ Other.cols(), Me.cols() };

		for (size_t j = 0; j < Other.cols(); ++j)
			for (size_t i = 0; i < Me.cols(); ++i)
				tmp(j, i) = Me(Other(0, j), i);

		newValue._value = tmp;
		}, _value, other._value);

	return newValue;
}

calc::ast::Expression::Value
calc::ast::Expression::Value::cols(const Value& other) const
{
	Expression::Value newValue{*this};
	std::visit([&](auto&& Me, auto&& Other) {
#ifdef _DEBUG
			if (isFloat(other._type))
				util::error<std::invalid_argument>("Index cannot be float");
#endif
			std::decay_t<decltype(Me)> tmp{ Me.rows(), Other.cols() };

			for (size_t j = 0; j < Other.cols(); ++j)
				for (size_t i = 0; i < Me.rows(); ++i)
					tmp(i, j) = Me(i, Other(0, j));
			
			newValue._value = tmp;
		}, _value, other._value);

	return newValue;
}

calc::ast::Expression::Value
calc::ast::Expression::Value::vector() const
{
	Expression::Value newValue{*this};

	std::visit([&](auto&& Me) {

		newValue._value = Me.linearize();

		}, _value);

	return newValue;
}

void
calc::ast::Expression::Value::set(const Value& index, const Value& val)
{
	std:visit([&](auto&& Me, auto&& Index, auto&& Val) {

#ifdef _DEBUG
			if (isFloat(index._type))
				util::error<std::invalid_argument>("Index cannot be float");
#endif	

			const auto sVal = Val.rows() * Val.cols();
			const auto sInd = Index.rows() * Index.cols();
	
			if (sVal != 1)
				for (size_t i = 0; i < sVal; ++i)
					Me(Index(i)) = Val(i);
			else
				for (size_t i = 0; i < sInd; ++i)
					Me(Index(i)) = Val(0, 0);

		}, _value, index._value, val._value);
}

void
calc::ast::Expression::Value::set(const Value& l1, const Value& l2, const Value& val)
{
	std:visit([&](auto&& Me, auto&& L1, auto&& L2, auto&& Val) {

#ifdef _DEBUG
		if (isFloat(l1._type))
			util::error<std::invalid_argument>("Left index cannot be float");
		if (isFloat(l2._type))
			util::error<std::invalid_argument>("Right index cannot be float");
#endif	

		const auto sVal = Val.rows() * Val.cols();
		const auto sInd = L1.rows() * L2.rows();		

		if (sVal != 1)
			for (size_t i = 0; i < L1.rows(); ++i)
				for (size_t j = 0; j < L2.rows(); ++j)
					Me(L1(0, i), L2(0, j)) = Val(L1(i), L2(j));
		else
			for (size_t i = 0; i < L1.rows(); ++i)
				for (size_t j = 0; j < L2.rows(); ++j)
					Me(L1(0, i), L2(j, 0)) = Val(0, 0);

	}, _value, l1._value, l2._value, val._value);
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
calc::ast::Expression::Value::setVector(const Value& other)
{
	*this = other.vector();
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
calc::ast::Expression::Value::colon(const Value& from, const Value& to, const Value& step)
{
	Expression::Value newValue{};	
	
	std::visit([&](auto&& From, auto&& To, auto&& Step) {

		#ifdef _DEBUG
			if (From.cols() != 1 || From.rows() != 1)
				util::error<std::logic_error>("First argument must be a single number");
			else if (Step.cols() != 1 || Step.rows() != 1)
				util::error<std::logic_error>("Step argument must be a single number");
			else if (To.cols() != 1 || To.rows() != 1)
				util::error<std::logic_error>("Second argument must be a single number");
		#endif // _DEBUG

			std::decay_t<decltype(From(0))> start{From(0)};
			std::decay_t<decltype(Step(0))> incremment{Step(0)};
			std::decay_t<decltype(To(0))> end{To(0)};

			std::decay_t<decltype(Step)> tmp{ 1, static_cast<size_t>((end-start)/incremment)+1 };

			decltype(incremment) val = start;

			for (size_t i = 0; val <= end; val = val + incremment)			
				tmp(0, i++) = val;

			newValue._value = tmp;
			newValue._type = step._type;

		}, from._value, to._value, step._value);

	return newValue;
}