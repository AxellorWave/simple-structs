#ifndef ARRAY_HPP
#define ARRAY_HPP
#include <cstddef>
#include <stdexcept>
#include <utility>

namespace zharov
{
  template< class T >
  struct Array
  {
    T* data;
    size_t size;
    size_t cap;
  };

  template< class T >
  Array< T > makeArray(size_t cap);

  template< class T >
  void clear(Array< T >& arr);

  template< class T >
  void swap(Array< T >& a, Array< T >& b);

  template< class T >
  Array< T > copy(const Array< T >& src, size_t newCap = 0);

  template< class T >
  void insert(Array< T >& arr, size_t pos, const T& val);

  template< class T >
  void remove(Array< T >& arr, size_t pos);

  template< class T >
  void pushBack(Array< T >& arr, const T& val);
}

template< class T >
zharov::Array< T > zharov::makeArray(size_t cap)
{
  Array< T > res{new T[cap], 0, cap};
  return res;
}

template< class T >
void zharov::clear(Array< T >& arr)
{
  delete[] arr.data;
  arr.data = nullptr;
  arr.size = 0;
  arr.cap = 0;
}

template< class T >
void zharov::swap(Array< T >& a, Array< T >& b)
{
  std::swap(a.data, b.data);
  std::swap(a.size, b.size);
  std::swap(a.cap, b.cap);
}

template< class T >
zharov::Array< T > zharov::copy(const Array< T >& src, size_t newCap)
{
  if (newCap < src.size)
  {
    newCap = src.cap;
  }
  Array< T > res = makeArray< T >(newCap);
  try
  {
    for (size_t i = 0; i < src.size; ++i)
    {
      res.data[i] = src.data[i];
    }
  }
  catch (...)
  {
    clear(res);
    throw;
  }
  res.size = src.size;
  return res;
}

template< class T >
void zharov::insert(Array< T >& arr, size_t pos, const T& val)
{
  if (pos > arr.size)
  {
    throw std::out_of_range("pos out of range");
  }
  const size_t newCap = (arr.size == arr.cap) ? arr.cap * 2 + 1 : arr.cap;
  Array< T > tmp = copy(arr, newCap);
  try
  {
    for (size_t i = tmp.size; i > pos; --i)
    {
      tmp.data[i] = tmp.data[i - 1];
    }
    tmp.data[pos] = val;
    ++tmp.size;
  }
  catch (...)
  {
    clear(tmp);
    throw;
  }
  swap(arr, tmp);
  clear(tmp);
}

template< class T >
void zharov::remove(Array< T >& arr, size_t pos)
{
  if (pos >= arr.size)
  {
    throw std::out_of_range("remove: pos out of range");
  }
  Array< T > tmp = copy(arr);
  try
  {
    for (size_t i = pos; i + 1 < tmp.size; ++i)
    {
      tmp.data[i] = tmp.data[i + 1];
    }
    --tmp.size;
  }
  catch (...)
  {
    clear(tmp);
    throw;
  }
  swap(arr, tmp);
  clear(tmp);
}

template< class T >
void zharov::pushBack(Array< T >& arr, const T& val)
{
  insert(arr, arr.size, val);
}

#endif
