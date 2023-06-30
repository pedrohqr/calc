#ifndef __Vector_h
#define __Vector_h

#include "util/Exception.h"
#include "util/SharedObject.h"

// An *academic* example of generic dynamic vector by Paulo Pagliosa

/*
The template class Vector below *is not* designed for efficiency and
its use in a real-world program is not encouraged. The purpose of
this example is just to demonstrate some C++ features such as
exception throwing and class-specific placement allocation and
deallocation functions (new and delete). Also, it illustrates the
use of "smart" pointers and managed objects (via reference counting)
in the handle/body idiom.
*/

namespace math
{ // begin namespace math

inline auto
vectorIndexOutOfRange(int i, size_t n)
{
  util::error<std::runtime_error>("Vector index out of range: "
    "%d not in [0,%llu)",
    i,
    n);
}

inline auto
vectorDimensionMustAgree(size_t n1, size_t n2)
{
  util::error<std::runtime_error>("Vector dimensions must agree: "
    "%llu, %llu",
    n1,
    n2);
}

template <typename T> class Vector;

template <typename T>
class VectorBody: public util::SharedObject
{
private:
  static constexpr auto minSize = (int)sizeof(void*);

  static constexpr auto roundup(long long size)
  {
    return (size + minSize - 1) & -minSize;
  }

public:
  void* operator new(size_t size, size_t n)
  {
    return ::operator new(roundup(size + n * sizeof(T)));
  }

  void operator delete(void* ptr, size_t)
  {
    ::operator delete(ptr);
  }

  void operator delete(void* ptr)
  {
    ::operator delete(ptr);
  }

private:
  using body_type = VectorBody<T>;

  static auto New(size_t n)
  {
    return new (n) body_type{n};
  }

  VectorBody() = default;

  VectorBody(size_t n):
    _n{n}
  {
    _data = reinterpret_cast<T*>(this + 1);
  }

  VectorBody(const body_type&);

  auto clone() const
  {
    return new (_n) body_type{*this};
  }

  body_type* add(const body_type*) const;
  body_type* sub(const body_type*) const;
  body_type* mul(const T&) const;

  void add_eq(const body_type*);
  void sub_eq(const body_type*);
  void mul_eq(const T&);

private:
  size_t _n{};
  T* _data{};

  static body_type* null();

  friend Vector;

};  // VectorBody

template <typename T>
VectorBody<T>*
VectorBody<T>::null()
{
  static body_type _null;

  if (_null.useCount() == 0)
    body_type::makeUse(&_null);
  return &_null;
}

template <typename T>
VectorBody<T>::VectorBody(const body_type& other):
  VectorBody{other._n}
{
  for (auto i = 0; i < _n; ++i)
    _data[i] = other._data[i];
}

template <typename T>
VectorBody<T>*
VectorBody<T>::add(const body_type* b) const
{
  if (_n != b->_n)
    vectorDimensionMustAgree(_n, b->_n);

  auto c = New(_n);

  for (auto i = 0; i < _n; ++i)
    c->_data[i] = _data[i] + b->_data[i];
  return c;
}

template <typename T>
VectorBody<T>*
VectorBody<T>::sub(const body_type* b) const
{
  if (_n != b->_n)
    vectorDimensionMustAgree(_n, b->_n);

  auto c = New(_n);

  for (auto i = 0; i < _n; ++i)
    c->_data[i] = _data[i] - b->_data[i];
  return c;
}

template <typename T>
VectorBody<T>*
VectorBody<T>::mul(const T& s) const
{
  auto c = New(_n);

  for (auto i = 0; i < _n; ++i)
    c->_data[i] = _data[i] * s;
  return c;
}

template <typename T>
void
VectorBody<T>::add_eq(const body_type* b)
{
  if (_n != b->_n)
    vectorDimensionMustAgree(_n, b->_n);
  for (auto i = 0; i < _n; ++i)
    _data[i] += b->_data[i];
}

template <typename T>
void
VectorBody<T>::sub_eq(const body_type* b)
{
  if (_n != b->_n)
    vectorDimensionMustAgree(_n, b->_n);
  for (auto i = 0; i < _n; ++i)
    _data[i] -= b->_data[i];
}

template <typename T>
void
VectorBody<T>::mul_eq(const T& s)
{
  for (auto i = 0; i < _n; ++i)
    _data[i] *= s;
}

template <typename T>
class Vector
{
public:
  using value_type = T;

  Vector():
    Vector{VectorBody<T>::null()}
  {
    // do nothing
  }

  Vector(int n):
    Vector{VectorBody<T>::New(n)}
  {
    // do nothing
  }

  Vector(const Vector&) = default;
  Vector(Vector&&) noexcept = default;

  auto clone() const
  {
    return Vector{_body->clone()};
  }

  auto size() const
  {
    return _body->_n;
  }

  auto operator ()(int i) const
  {
    if (i < 0 || i >= _body->_n)
      vectorIndexOutOfRange(i, _body->_n);
    return _body->_data[i];
  }

  auto& operator ()(int i)
  {
    if (i < 0 || i >= _body->_n)
      vectorIndexOutOfRange(i, _body->_n);
    checkCopyOnWrite();
    return _body->_data[i];
  }

  Vector& operator =(const Vector&) = default;
  Vector& operator =(Vector&&) noexcept = default;

  auto operator +(const Vector& b) const
  {
    return Vector{_body->add(b._body)};
  }

  auto operator -(const Vector& b) const
  {
    return Vector{_body->sub(b._body)};
  }

  auto operator *(const T& s) const
  {
    return Vector{_body->mul(s)};
  }

  auto& operator +=(const Vector& b)
  {
    checkCopyOnWrite();
    _body->add_eq(b._body);
    return *this;
  }

  auto& operator -=(const Vector& b)
  {
    checkCopyOnWrite();
    _body->sub_eq(b._body);
    return *this;
  }

  auto& operator *=(const T& s)
  {
    checkCopyOnWrite();
    _body->mul_eq(s);
    return *this;
  }

private:
  util::ObjectPtr<VectorBody<T>> _body;

  Vector(const VectorBody<T>* body):
    _body{body}
  {
    // do nothing
  }

  void checkCopyOnWrite()
  {
    if (_body->useCount() > 1)
      _body = _body->clone();
  }

};  // Vector

} // end namespace math

template <typename T>
inline auto
operator *(const T& s, const math::Vector<T>& v)
{
  return v * s;
}

#endif // __Vector_h
