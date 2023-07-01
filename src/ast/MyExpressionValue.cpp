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
{	//TODO
	Expression::Value tmp{*this};

	if (isInt(_type) && isInt(other._type))
	{
		/*if (std::get<IntMatrix>(_value).rows() == std::get<IntMatrix>(_value).cols() == 1 && std::get<IntMatrix>(other._value).rows() != 1 || std::get<IntMatrix>(other._value).cols() != 1)
		{	
			const IntMatrix& matrix = std::get<IntMatrix>(other._value);
			IntMatrix aux{ matrix.rows(), matrix.cols() };
			for (size_t i = 0, sr = matrix.rows(); i < sr; ++i)
				for (size_t j = 0, sc = matrix.cols(); j < sr; ++j)
					aux(i,j) = std::get<IntMatrix>(_value) + matrix(i, j);
		}*/
		tmp._value = std::get<IntMatrix>(_value) + std::get<IntMatrix>(other._value);
	}
	else if (isFloat(_type) && isFloat(other._type))
		tmp._value = std::get<FloatMatrix>(_value) + std::get<FloatMatrix>(other._value);
	else if (isFloat(_type) && isInt(other._type))
		tmp._value = std::get<FloatMatrix>(_value) + std::get<FloatMatrix>(other.castTo(_type)._value);
	else if (isInt(_type) && isFloat(other._type))
		throw std::bad_cast();

	return tmp;
}

calc::ast::Expression::Value
calc::ast::Expression::Value::operator -(const Value& other) const
{	//TODO
	Expression::Value tmp{*this};

	if (isInt(_type) && isInt(other._type))
		tmp._value = std::get<IntMatrix>(_value) - std::get<IntMatrix>(other._value);
	else if (isFloat(_type) && isFloat(other._type))
		tmp._value = std::get<FloatMatrix>(_value) - std::get<FloatMatrix>(other._value);
	else if (isFloat(_type) && isInt(other._type))
		tmp._value = std::get<FloatMatrix>(_value) - std::get<FloatMatrix>(other.castTo(_type)._value);
	else if (isInt(_type) && isFloat(other._type))
		throw std::bad_cast();
	
	return tmp;
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
		tmp._value = std::get<FloatMatrix>(_value) - std::get<FloatMatrix>(other._value);
	}
	else if (isFloat(_type) && isInt(other._type))
	{
		if (std::get<FloatMatrix>(_value).cols() != std::get<IntMatrix>(other._value).rows())
			throw std::length_error("Cols and Rows differents");
		tmp._value = std::get<FloatMatrix>(_value) - std::get<FloatMatrix>(other.castTo(_type)._value);
	}
	else if (isInt(_type) && isFloat(other._type))
		throw std::bad_cast();

	return tmp;
}

calc::ast::Expression::Value
calc::ast::Expression::Value::operator /(const Value& other) const
{
	// TODO
	return *this;
}

calc::ast::Expression::Value
calc::ast::Expression::Value::operator -() const
{
	// TODO
	return *this;
}

calc::ast::Expression::Value
calc::ast::Expression::Value::size() const
{
	// TODO
	return *this;
}

calc::ast::Expression::Value
calc::ast::Expression::Value::transpose() const
{
	// TODO
	return *this;
}

calc::ast::Expression::Value
calc::ast::Expression::Value::horzcat(const Value& other) const
{
	if ( isVoid(_type) && isInt(other._type) )
	{
		Expression::Value newValue{*this};
		newValue._value = std::get<IntMatrix>(other._value);
		newValue._type = other._type;

		return newValue;
	}
	else if (isVoid(_type) && isFloat(other._type))
	{
		Expression::Value newValue{*this};
		newValue._value = std::get<FloatMatrix>(other._value);
		newValue._type = other._type;

		return newValue;
	}
	else if ( isInt(_type) && isInt(other._type) )
	{
		const IntMatrix& Me = std::get<IntMatrix>(_value);
		const IntMatrix& Other = std::get<IntMatrix>(other._value);
		auto r = Me.rows();
		auto c1 = Me.cols();
		auto c2 = Other.cols();

		IntMatrix tmp{ r, c1 + c2 };

		for (size_t i = 0; i < r; ++i)
			for (size_t j = 0, c = c1 + c2; j < c; ++j)
				if (j < c1)
					tmp(i, j) = Me(i, j);
				else
					tmp(i, j) = Other(i, j - c1);

		Expression::Value newValue{*this};
		newValue._value = tmp;

		return newValue;
	}
	else if (isFloat(_type) && isFloat(other._type))
	{
		const FloatMatrix& Me = std::get<FloatMatrix>(_value);
		const FloatMatrix& Other = std::get<FloatMatrix>(other._value);
		auto r = Me.rows();
		auto c1 = Me.cols();
		auto c2 = Other.cols();

		FloatMatrix tmp{ r, c1 + c2 };

		for (size_t i = 0; i < r; ++i)
			for (size_t j = 0, c = c1 + c2; j < c; ++j)
				if (j < c1)
					tmp(i, j) = Me(i, j);
				else
					tmp(i, j) = Other(i, j - c1);

		Expression::Value newValue{*this};
		newValue._value = tmp;

		return newValue;
	}

	return *this;
}

calc::ast::Expression::Value
calc::ast::Expression::Value::vertcat(const Value& other) const
{
	// TODO
	if (isVoid(_type) && isInt(other._type))
	{
		Expression::Value newValue{*this};
		newValue._value = std::get<IntMatrix>(other._value);
		newValue._type = other._type;

		return newValue;
	}
	else if (isVoid(_type) && isFloat(other._type))
	{
		Expression::Value newValue{*this};
		newValue._value = std::get<FloatMatrix>(other._value);
		newValue._type = other._type;

		return newValue;
	}
	else if (isInt(_type) && isInt(other._type))
	{
		const IntMatrix& Me = std::get<IntMatrix>(_value);
		const IntMatrix& Other = std::get<IntMatrix>(other._value);
		auto r1 = Me.rows();
		auto r2 = Other.rows();
		auto c = Me.cols();

		IntMatrix tmp{ r1 + r2, c };

		for (size_t i = 0, r = r1+r2; i < r; ++i)
			for (size_t j = 0; j < c; ++j)
				if (i < r1)
					tmp(i, j) = Me(i, j);
				else
					tmp(i, j) = Other(i - r1, j);

		Expression::Value newValue{*this};
		newValue._value = tmp;

		return newValue;
	}
	else if (isFloat(_type) && isFloat(other._type))
	{
		const FloatMatrix& Me = std::get<FloatMatrix>(_value);
		const FloatMatrix& Other = std::get<FloatMatrix>(other._value);
		auto r1 = Me.rows();
		auto r2 = Other.rows();
		auto c = Me.cols();

		FloatMatrix tmp{ r1 + r2, c };

		for (size_t i = 0, r = r1 + r2; i < r; ++i)
			for (size_t j = 0; j < c; ++j)
				if (i < r1)
					tmp(i, j) = Me(i, j);
				else
					tmp(i, j) = Other(i - r1, j);

		Expression::Value newValue{*this};
		newValue._value = tmp;

		return newValue;
	}
	return *this;
}

calc::ast::Expression::Value
calc::ast::Expression::Value::operator ()(const Value& other) const
{
	//TODO -> tem que ainda aceitar um vetor e retornar as posições colocadas no vetor e também tem que devolver uma sub-matriz a partir de um elemento x até y, ou seja 2:5(segunda linha até a 5 coluna)
	Expression::Value newValue{*this};

	std::visit([&](auto&& Me, auto&& Other)
		{
			if (Other.rows() != 1 && Other.cols() != 1 || Other(0, 0) >= Me.rows() * Me.cols())
				throw std::out_of_range("Invalid range");
			else if (!isInt(other._type))
				throw std::invalid_argument("Type is not int");
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
	// a(1:2, [3,0,2]) ela tem q se restringir da linha 1 até 2 e devolver as colunas 3, 0 e 2 e devolve uma submatriz de qualquer tipo
	// Para [3, 0 ,2 ] ele só pode ser vetor, ou seja, não pode ter mais do que 1 linha, se não é erro
	Expression::Value newValue;
	
	std::visit([&](auto&& Me, auto&& Otherl, auto&& Otherc)
		{
			if (isVoid(l1._type) || isVoid(l2._type))
			{
				std::decay_t<decltype(Me)> tmp{};
				newValue._value = tmp;
			}
			else if (Me.rows() > Otherl.rows() || Me.cols() > Otherl.cols() || Me.rows() > Otherc.rows() || Me.cols() > Otherc.cols())
				throw std::length_error("Rows or Cols is invalid");


		}, _value, l1._value, l2._value);
	
	return newValue;
}

calc::ast::Expression::Value
calc::ast::Expression::Value::rows(const Value& r) const
{
	// TODO
	// Operador :, ou seja, ele tem que imprimir todas as linhas
	// Ele aceita um matriz de linha 1 somente, de resto é erro
	
	Expression::Value newValue;
	
	std::visit([&](auto&& Me, auto&& OtherR)
		{
			std::decay_t<decltype(Me)> tmp{Me};
			if (r._type == ":")
				for (size_t i = 0, sr = Me.rows(); i < sr; ++i)
				{
					tmp(i, 0) = Me(i, 0);
					newValue._value = tmp;
				}

		}, _value, r._value);

	return newValue;
}

calc::ast::Expression::Value
calc::ast::Expression::Value::cols(const Value& c) const
{
	// TODO
	// Operador :, ou seja, ele tem que imprimir todas as linhas
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