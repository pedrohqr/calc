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
matrixIndexOutOfRange(int i, size_t n)
{
    util::error<std::logic_error>("Matrix index out of range: index %d not in [0,%llu)",
        i,
        n);
}

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

  Matrix(size_t m, size_t n);

  Matrix(const Matrix&);
  Matrix(Matrix&&) noexcept;

  ~Matrix();

  inline auto rows() const;
  inline auto cols() const;

  inline auto& operator ()(int i, int j);
  inline auto operator ()(int i, int j) const;
  inline auto& operator ()(int i);
  inline auto operator ()(int i) const;

  Matrix& operator =(const Matrix&);
  Matrix& operator =(Matrix&&) noexcept;

  Matrix& operator +=(const Matrix&);
  Matrix& operator -=(const Matrix&);
  Matrix& operator *=(const Matrix&);
  Matrix& operator *=(const T&);

  template<typename U> Matrix<float> operator +(const Matrix<U>& other) const;
  template<typename U> Matrix<float> operator -(const Matrix<U>& other) const;
  template<typename U> Matrix<float> operator *(const Matrix<U>& other) const;

  Matrix operator +(const Matrix&) const;
  Matrix operator -(const Matrix&) const;
  Matrix operator -() const;
  Matrix operator *(const Matrix&) const;
  Matrix operator *(const T&) const;
  Vector<T> operator *(const Vector<T>&) const;

  Matrix transpose() const;
  Matrix linearize() const;
  static Matrix identity(size_t, size_t);
  static Matrix set_for_all(T, size_t, size_t);
  void print(calc::Writer&) const;

private:
  size_t _m{};
  size_t _n{};
  T* _data{};

}; // Matrix

using IntMatrix = Matrix<int>;
using FloatMatrix = Matrix<float>;

template<typename T>
Matrix<T>::Matrix(size_t m, size_t n) :
    _m{ m },
    _n{ n }
{
    _data = new T[m * n];
}

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

template<typename T>
Matrix<T>::~Matrix()
{
    delete[]_data;
}

template<typename T>
inline
auto Matrix<T>::rows() const
{
    return _m;
};

template<typename T>
inline
auto Matrix<T>::cols() const
{
    return _n;
};

template<typename T>
inline
auto& Matrix<T>::operator ()(int i, int j)
{
#ifdef _DEBUG
    if (i < 0 || i >= _m)
        matrixIndexOutOfRange(MatrixIndexType::Row, i, _m);
    if (j < 0 || j >= _n)
        matrixIndexOutOfRange(MatrixIndexType::Col, j, _n);
#endif // _DEBUG
    return _data[i * _n + j];
}

template<typename T>
inline
auto Matrix<T>::operator ()(int i, int j) const
{
    return const_cast<Matrix*>(this)->operator ()(i, j);
}

template<typename T>
inline
auto& Matrix<T>::operator ()(int i)
{
#ifdef _DEBUG
    if (i < 0 || i >= _m * _n)
        matrixIndexOutOfRange(i, _m);
#endif // _DEBUG
    return _data[i];
}

template<typename T>
inline
auto Matrix<T>::operator ()(int i) const
{
    return const_cast<Matrix*>(this)->operator ()(i);
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
#ifdef _DEBUG
    if (_m != b._m || _n != b._n)
        matrixDimensionMustAgree(_m, _n, b._m, b._n);
#endif // _DEBUG

  Matrix c{_m, _n};

  for (size_t s = _m * _n, i{}; i < s; ++i)
    c._data[i] = _data[i] + b._data[i];
  return c;
}

template<typename T>
template<typename U>
Matrix<float> 
Matrix<T>::operator +(const Matrix<U>& other) const
{
#ifdef _DEBUG
    if (_m != other.rows() || _n != other.cols())
        matrixDimensionMustAgree(_m, _n, other.rows(), other.cols());
#endif // _DEBUG

    Matrix<float> result(_m, _n);

    for (size_t i = 0; i < _m * _n; ++i)
        result(i) = static_cast<float>(_data[i]) + static_cast<float>(other(i));

    return result;
}

template <typename T>
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
template<typename U>
Matrix<float>
Matrix<T>::operator -(const Matrix<U>& other) const
{
#ifdef _DEBUG
    if (_m != other.rows() || _n != other.cols())
        matrixDimensionMustAgree(_m, _n, other.rows(), other.cols());
#endif // _DEBUG

    Matrix<float> result(_m, _n);

    for (size_t i = 0; i < _m * _n; ++i)
        result(i) = static_cast<float>(_data[i]) - static_cast<float>(other(i));

    return result;
}

template<typename T>
Matrix<T> 
Matrix<T>::operator -() const
{
    Matrix<T> result{_m, _n};

    for (size_t i = 0; i < _m; ++i)
        for (size_t j = 0; j < _n; ++j)
            result(i, j) = -(*this)(i, j);

    return result;
}

template<typename T>
Matrix<T>
Matrix<T>::operator *(const Matrix& other) const
{
#ifdef _DEBUG
    if (_n != other._m)
        matrixDimensionMustAgree(_m, _n, other._m, other._n);
#endif // _DEBUG

    Matrix<T> result{ _m, other._n };

    for (size_t i = 0; i < _m; ++i) {
        for (size_t j = 0; j < other._n; ++j) {
            T sum = 0;
            for (size_t k = 0; k < _n; ++k) {
                sum += (*this)(i, k) * other(k, j);
            }
            result(i, j) = sum;
        }
    }

    return result;
}

template<typename T>
template<typename U>
Matrix<float>
Matrix<T>::operator *(const Matrix<U>& other) const
{
#ifdef _DEBUG
    if (_n != other.rows())
        matrixDimensionMustAgree(_m, _n, other.rows(), other.cols());
#endif // _DEBUG

    Matrix<float> result{ _m, other.cols() };

    for (size_t i = 0; i < _m; ++i) {
        for (size_t j = 0; j < other.cols(); ++j) {
            float sum = 0;
            for (size_t k = 0; k < _n; ++k) {
                sum += static_cast<float>((*this)(i, k)) * static_cast<float>(other(k, j));
            }
            result(i, j) = sum;
        }
    }

    return result;
}

template<typename T>
Matrix<T>
Matrix<T>::operator *(const T& value) const
{
    Matrix result{ _m, _n };

    for (size_t i = 0, s = _m * _n; i < s; ++i)
        result[i] = _data[i] * value;

    return result;
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
    Matrix result{ _n, _m };

    for (size_t i = 0; i < _m; ++i)
        for (size_t j = 0; j < _n; ++j)
            result(j, i) = (*this)(i, j);

    return result;
}

template<typename T>
Matrix<T> 
Matrix<T>::linearize() const
{
    Matrix<T> result{1, _m * _n};

    for (size_t i = 0, s = _m * _n; i < s; ++i)
        result(i) = _data[i];

    return result;
}

template<typename T>
Matrix<T>
Matrix<T>::identity(size_t m, size_t n)
{
    Matrix<T> result{ m, n };

    for (size_t i = 0; i < m; ++i)
        for (size_t j = 0; j < n; ++j)
            if (i == j)
                result(i, j) = 1;
            else
                result(i, j) = 0;

    return result;
}

template<typename T>
Matrix<T>
Matrix<T>::set_for_all(T k, size_t m, size_t n)
{
    Matrix<T> result{ m, n };

    for (size_t i = 0; i < m; ++i)
        for (size_t j = 0; j < n; ++j)
                result(i, j) = k;

    return result;
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
