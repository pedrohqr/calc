#ifndef __Matrix_h
#define __Matrix_h

#include "Vector.h"
#include "Writer.h"
#include <cstdio>

// An *academic* example of generic dynamic matrix by Paulo Pagliosa

/*
The template class Matrix below *is not* designed for efficiency and
its use in a real-world program is not encouraged. The purpose of
this example is just to demonstrate some C++ features such as
exception throwing, operator overloading, and copy/move operations.
*/

namespace math
{ // begin namespace math

enum class MatrixIndexType
{
  Row,
  Col
};

inline auto
matrixIndexOutOfRange(MatrixIndexType it, int i, size_t n)
{
  util::error<std::logic_error>("Matrix index out of range: %s %d not in [0,%llu)",
    it == MatrixIndexType::Row ? "row" : "column",
    i,
    n);
}

inline auto
matrixDimensionMustAgree(size_t m1, size_t n1, size_t m2, size_t n2)
{
  util::error<std::logic_error>("Matrix dimensions must agree: "
    "(%llu,%llu), (%llu,%llu)",
    m1, n1,
    m2, n2);
}

template <typename T>
class Matrix
{
public:
  using value_type = T;

  struct Element
  {
    int i;
    int j;
    T value;
  };

  using IterFunc = void(const Element&);

  void iterate(IterFunc) const;

  Matrix() = default;

  Matrix(size_t m, size_t n):
    _m{m},
    _n{n}
  {
    _data = new T[m * n];
  }

  Matrix(const Matrix&);
  Matrix(Matrix&&) noexcept;

  ~Matrix()
  {
    delete []_data;
  }

  auto rows() const
  {
    return _m;
  }

  auto cols() const
  {
    return _n;
  }

  T& operator()(int i, int j)
  {
#ifdef _DEBUG
      if (i < 0 || i >= _m || j < 0 || j >= _n) 
          throw std::out_of_range("Matrix index out of bounds.");
#endif // _DEBUG

      return _data[i * _n + j];
  }

  T operator()(int i, int j) const
  {
#ifdef _DEBUG
      if (i < 0 || i >= _m || j < 0 || j >= _n) 
          throw std::out_of_range("Matrix index out of bounds.");
#endif // _DEBUG
      return _data[i * _n + j];
  }

  Matrix& operator =(const Matrix&);
  Matrix& operator =(Matrix&&) noexcept;

  Matrix& operator +=(const Matrix&);
  Matrix& operator -=(const Matrix&);
  Matrix& operator *=(const Matrix&);
  Matrix& operator *=(const T&);

  Matrix operator +(const Matrix&) const;
  Matrix operator -(const Matrix&) const;
  Matrix operator *(const Matrix&) const;
  Matrix operator *(const T&) const;
  Matrix* operator &();
  Vector<T> operator *(const Vector<T>&) const;

  Matrix transpose() const;

  void print(calc::Writer&) const;

  Matrix<T> get_type()
  {
      return this;
  }

  T get_value_type()
  {
      return *static_cast<T*>(this);
  }

private:
  size_t _m{};
  size_t _n{};
  T* _data{};

}; // Matrix

using IntMatrix = Matrix<int>;
using FloatMatrix = Matrix<float>;

template <typename T>
Matrix<T>::Matrix(const Matrix& other):
  Matrix{other._m, other._n}
{
  for (size_t s = _m * _n, i = 0; i < s; ++i)
    _data[i] = other._data[i];
}

template <typename T>
Matrix<T>::Matrix(Matrix&& other) noexcept
{
  _m = other._m;
  _n = other._n;
  _data = other._data;
  other._m = other._n = 0;
  other._data = nullptr;
}

template <typename T>
Matrix<T>&
Matrix<T>::operator =(const Matrix& other)
{
  if (_m != other._m || _n != other._n)
  {
    delete []_data;
    _m = other._m;
    _n = other._n;
    _data = new T[_m * _n];
  }
  for (size_t s = _m * _n, i{}; i < s; ++i)
    _data[i] = other._data[i];
  return *this;
}

template <typename T>
Matrix<T>&
Matrix<T>::operator =(Matrix&& other) noexcept
{
  delete []_data;
  _m = other._m;
  _n = other._n;
  _data = other._data;
  other._m = other._n = 0;
  other._data = nullptr;
  return *this;
}

template <typename T>
Matrix<T>&
Matrix<T>::operator +=(const Matrix& b)
{
#ifdef _DEBUG
    if (_m != b._m || _n != b._n)
        matrixDimensionMustAgree(_m, _n, b._m, b._n);
#endif // _DEBUG
    for (size_t s = _m * _n, i{}; i < s; ++i)
        _data[i] += b._data[i];
    return *this;
}

template <typename T>
Matrix<T>&
Matrix<T>::operator -=(const Matrix& b)
{
#ifdef _DEBUG
    if (_m != b._m || _n != b._n)
        matrixDimensionMustAgree(_m, _n, b._m, b._n);
#endif // _DEBUG
    for (size_t s = _m * _n, i{}; i < s; ++i)
        _data[i] -= b._data[i];
    return *this;
}

template<typename T>
Matrix<T>&
Matrix<T>::operator *=(const Matrix& b)
{
#ifdef _DEBUG
    if (_n != b._m)
        matrixDimensionMustAgree(_m, _n, b._m, b._n);
#endif // _DEBUG
    _data = _data * b;
    return *this;
}

template<typename T>
Matrix<T>&
Matrix<T>::operator *=(const T& value)
{
    for (size_t i = 0, s = _m * _n; i < s; ++i)
        _data[i] = _data[i] * value;

    return *this;
}

template <typename T>
Matrix<T>
Matrix<T>::operator +(const Matrix& b) const
{
/*#ifdef _DEBUG
    if (_m != b._m || _n != b._n)
        matrixDimensionMustAgree(_m, _n, b._m, b._n);
#endif // _DEBUG*/

  Matrix c{_m, _n};

  for (size_t s = _m * _n, i{}; i < s; ++i)
    c._data[i] = _data[i] + b._data[i];
  return c;
}

template<typename T>
Matrix<T>
Matrix<T>::operator -(const Matrix& b) const
{
#ifdef _DEBUG
    if (_m != b._m || _n != b._n)
        matrixDimensionMustAgree(_m, _n, b._m, b._n);
#endif // _DEBUG

    Matrix c{ _m, _n };

    for (size_t s = _m * _n, i{}; i < s; ++i)
        c._data[i] = _data[i] - b._data[i];
    return c;
}

template<typename T>
Matrix<T>
Matrix<T>::operator *(const Matrix& b) const
{
#ifdef _DEBUG
    if (_n != b._m)
        matrixDimensionMustAgree(_m, _n, b._m, b._n);
#endif // _DEBUG
    Matrix<T> tmp{_m, b._n};

    for (size_t row = 0; row < _m; ++row)
        for (size_t col = 0; col < b._n; ++col)
            for (size_t k = 0; k < _n; ++k)
                tmp[row][col] += _data[row][k] * b[k][col];

    return tmp;
}

template<typename T>
Matrix<T>
Matrix<T>::operator *(const T& value) const
{
    Matrix C{ _m, _n };

    for (size_t i = 0, s = _m * _n; i < s; ++i)
        C[i] = _data[i] * value;

    return C;
}
template<typename T>
Matrix<T>*
Matrix<T>::operator &()
{
    return this;
}

template<typename T>
Vector<T>
Matrix<T>::operator *(const Vector<T>& vec) const
{
#ifdef _DEBUG
    if (_n != vec.size())
        vectorDimensionMustAgree(_n, vec.size());
#endif // _DEBUG

    Vector v{ _n };

    for (size_t i = 0; i < _n; ++i)
        for (size_t j = 0; j < _m; ++j)
            v[i] += _data[i][j] * vec[i];

    return v;
}

template<typename T>
Matrix<T>
Matrix<T>::transpose() const
{
    Matrix C{ _n, _m };

    for (size_t lin = 0; lin < _m; ++lin)
        for (size_t col = 0; col < _n; ++col)
            C[col][lin] = _data[lin][col];

    return C;
}

template <typename T>
void
Matrix<T>::iterate(IterFunc f) const
{
  Element e;

  e.i = 0;
  for (int p = 0; e.i < _m; ++e.i)
    for (e.j = 0; e.j < _n; ++e.j)
    {
      e.value = _data[p++];
      f(e);
    }
}

template<typename T>
void 
Matrix<T>::print(calc::Writer& writer) const
{
    writer.beginBlock();
    for (size_t i = 0; i < _m; ++i)
    {
        for (size_t j = 0; j < _n; ++j)
        {
            std::cout << '\t';
            std::cout << _data[i * _n + j];
        }
            
        writer.endLine();
    }
    writer.endBlock();
}

} // end namespace math

template <typename T>
inline auto
operator *(const T& s, const math::Matrix<T>& m)
{
  return m * s;
}

#endif // __Matrix_h
