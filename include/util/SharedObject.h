#ifndef __SharedObject_h
#define __SharedObject_h

#include <type_traits>

namespace util
{ // begin namespace util

//
// Forward definition
//
class SharedObject;

template <typename T>
inline constexpr auto
isSharedObject()
{
  return std::is_assignable_v<SharedObject, T>;
}

#define ASSERT_SHARED(T, msg) static_assert(isSharedObject<T>(), msg)

template <typename T>
inline void
destroy(T* ptr)
{
  delete ptr;
}

class SharedObject
{
public:
  virtual ~SharedObject() = default;

  auto useCount() const
  {
    return _useCount;
  }

  template <typename T>
  static auto makeUse(const T* object)
  {
    ASSERT_SHARED(T, "Pointer to shared object expected");
    if (object != nullptr)
      ++object->_useCount;
    return (T*)object;
  }

  template <typename T>
  static void release(T* object)
  {
    ASSERT_SHARED(T, "Pointer to shared object expected");
    if (object != nullptr && --object->_useCount <= 0)
      destroy(object);
  }

private:
  mutable int _useCount{};

}; // SharedObject

template <typename T>
class ObjectPtr
{
public:
  using pointer = ObjectPtr<T>;

  ObjectPtr() = default;

  ObjectPtr(const pointer& other):
    _ptr{T::makeUse(other._ptr)}
  {
    // do nothing
  }

  ObjectPtr(pointer&& other):
    _ptr{other._ptr}
  {
    other._ptr = nullptr;
  }

  ObjectPtr(const T* ptr):
    _ptr{T::makeUse(ptr)}
  {
    // do nothing
  }

  ~ObjectPtr()
  {
    T::release(_ptr);
  }

  auto& operator =(const pointer& other)
  {
    return operator =(other._ptr);
  }

  auto& operator =(pointer&& other)
  {
    std::swap(_ptr, other._ptr);
    return *this;
  }

  auto& operator =(const T* ptr)
  {
    if (_ptr != ptr)
    {
      T::release(_ptr);
      _ptr = T::makeUse(ptr);
    }
    return *this;
  }

  auto operator ==(const pointer& other) const
  {
    return operator ==(other._ptr);
  }

  auto operator ==(const T* ptr) const
  {
    return _ptr == ptr;
  }

  auto operator !=(const pointer& other) const
  {
    return !operator ==(other);
  }

  auto operator !=(const T* ptr) const
  {
    return !operator ==(ptr);
  }

  operator T* () const
  {
    return _ptr;
  }

  auto operator->() const
  {
    return _ptr;
  }

  auto get() const
  {
    return _ptr;
  }

  auto& operator *() const
  {
    assert(_ptr != nullptr);
    return *_ptr;
  }

private:
  T* _ptr{};

}; // ObjectPtr

} // end namespace util

#endif // __SharedObject_h
