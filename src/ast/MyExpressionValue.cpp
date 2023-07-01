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
	//TODO
	return *this;
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
	//TODO -> tem que ainda aceitar um vetor e retornar as posi��es colocadas no vetor e tamb�m tem que devolver uma sub-matriz a partir de um elemento x at� y, ou seja 2:5(segunda linha at� a 5 coluna)
	Expression::Value newValue{*this};

	std::visit([&](auto&& Me, auto&& Other)
		{
			#ifdef _DEBUG
				if (Other.rows() != 1 && Other.cols() != 1 || Other(0, 0) >= Me.rows() * Me.cols())
					util::error<std::out_of_range>("Invalid Rows or Cols");
			#endif

			#ifdef _DEBUG
				else if (!isInt(other._type))
					util::error<std::invalid_argument>("Tyep is not int");
			#endif
			
				else
				{
					std::decay_t<decltype(Me)> tmp{1, 1};

					for (size_t i = 0, sr = Me.rows();i < sr; ++i)
						for (size_t j = 0, sc = Me.cols(); j < sc; ++j)
						{
							if ((i * sc + j) == Other(0, 0))
							{
								tmp(0, 0) = Me(i, j);
								newValue._value = tmp;
								break;
							}
						}
				}
			},_value, other._value);
	
	return newValue;
}

calc::ast::Expression::Value
calc::ast::Expression::Value::operator ()(const Value& l1, const Value& l2) const
{
	// TODO
	// Ele vai devolver i elemento localizado na linha x e na coluna y.
	// Se ele tiver matrix vazia, devolve essa matriz vazia
	// a(1:2, [3,0,2]) ela tem q se restringir da linha 1 ate 2 e devolver as colunas 3, 0 e 2 e devolve uma submatriz de qualquer tipo
	// Para [3, 0 ,2 ] ele s� pode ser vetor, ou seja, nao pode ter mais do que 1 linha, se nao e erro
	Expression::Value newValue;
	
	std::visit([&](auto&& Me, auto&& Otherl, auto&& Otherc)
		{
			#ifdef _DEBUG	
			if (Me.rows() < Otherl(0, 0) || Me.cols() < Otherc(0, 0))
					util::error<std::length_error>("Rows or Cols is invalid");
			#endif
			
			if (isVoid(l1._type) || isVoid(l2._type))
			{
				std::decay_t<decltype(Me)> tmp{};
				newValue._value = tmp;
			}

			std::decay_t<decltype(Me)> tmp{1, 1};

			tmp(0,0) = Me(Otherl(0, 0), Otherc(0, 0));
			newValue._value = tmp;


		}, _value, l1._value, l2._value);
	
	return newValue;
}

calc::ast::Expression::Value
calc::ast::Expression::Value::rows(const Value& r) const
{
	Expression::Value newValue;
	std::visit([&](auto&& Me, auto&& OtherR)
		{			
			#ifdef _DEBUG
				if (OtherR.rows() != 1 && OtherR.cols() != 1)
					util::error<std::length_error>("Rows and Cols must be one");
			#endif
			
				else
				{
					std::decay_t<decltype(Me)> tmp{1, Me.cols()};
					for(size_t j = 0, sc = Me.cols(); j < sc; ++j)
					{
						tmp(0, j) = Me(OtherR(0,0), j);
						newValue._value = tmp;
					}
				}

		}, _value, r._value);

	return newValue;
}

calc::ast::Expression::Value
calc::ast::Expression::Value::cols(const Value& c) const
{
	Expression::Value newValue;
	std::visit([&](auto&& Me, auto&& OtherR)
		{
#ifdef _DEBUG
			if (OtherR.rows() != 1 && OtherR.cols() != 1)
				util::error<std::length_error>("Rows and Cols must be one");
#endif

			else
			{
				std::decay_t<decltype(Me)> tmp{Me.rows(), 1};
				for (size_t i = 0, sr = Me.rows(); i < sr; ++i)
				{
					tmp(i, 0) = Me(i, OtherR(0, 0));
					newValue._value = tmp;
				}
			}

		}, _value, c._value);

	return newValue;
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