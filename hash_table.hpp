#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP
#include <cstddef>
#include <functional>
#include <utility>
#include "list.hpp"

namespace zharov
{
  template< class K, class V >
  struct Entry
  {
    K key;
    V val;
  };

  template< class K, class V >
  struct HashTable
  {
    List< Entry< K, V > >* slots;
    size_t size;
    size_t cap;
  };

  template< class K, class V >
  HashTable< K, V > makeHashTable(size_t cap);

  template< class K, class V >
  void clear(HashTable< K, V >& ht);

  template< class K, class V >
  void swap(HashTable< K, V >& a, HashTable< K, V >& b);

  template< class K, class V >
  HashTable< K, V > copy(const HashTable< K, V >& src);

  template< class K, class V >
  detail::Node< Entry< K, V > >* find(const HashTable< K, V >& ht, const K& key);

  template< class K, class V >
  void rehash(HashTable< K, V >& ht, size_t newCap);

  template< class K, class V >
  void insert(HashTable< K, V >& ht, const K& key, const V& val);

  template< class K, class V >
  void remove(HashTable< K, V >& ht, const K& key);
}

template< class K, class V >
zharov::HashTable< K, V > zharov::makeHashTable(size_t cap)
{
  return HashTable< K, V >{new List< Entry< K, V > >[cap](), 0, cap};
}

template< class K, class V >
void zharov::clear(HashTable< K, V >& ht)
{
  for (size_t i = 0; i < ht.cap; ++i)
  {
    clear(ht.slots[i]);
  }
  delete[] ht.slots;
  ht.slots = nullptr;
  ht.size = 0;
  ht.cap = 0;
}

template< class K, class V >
void zharov::swap(HashTable< K, V >& a, HashTable< K, V >& b)
{
  std::swap(a.slots, b.slots);
  std::swap(a.size, b.size);
  std::swap(a.cap, b.cap);
}

template< class K, class V >
zharov::HashTable< K, V > zharov::copy(const HashTable< K, V >& src)
{
  HashTable< K, V > res = makeHashTable< K, V >(src.cap);
  try
  {
    for (size_t i = 0; i < src.cap; ++i)
    {
      for (detail::Node< Entry< K, V > >* curr = src.slots[i].head; curr != nullptr; curr = curr->next)
      {
        insert(res, curr->val.key, curr->val.val);
      }
    }
  }
  catch (...)
  {
    clear(res);
    throw;
  }
  return res;
}

template< class K, class V >
zharov::detail::Node< zharov::Entry< K, V > >* zharov::find(const HashTable< K, V >& ht, const K& key)
{
  const size_t idx = std::hash< K >{}(key) % ht.cap;
  for (detail::Node< Entry< K, V > >* curr = ht.slots[idx].head; curr != nullptr; curr = curr->next)
  {
    if (curr->val.key == key)
    {
      return curr;
    }
  }
  return nullptr;
}

template< class K, class V >
void zharov::rehash(HashTable< K, V >& ht, size_t newCap)
{
  HashTable< K, V > tmp = makeHashTable< K, V >(newCap);
  try
  {
    for (size_t i = 0; i < ht.cap; ++i)
    {
      for (detail::Node< Entry< K, V > >* curr = ht.slots[i].head; curr != nullptr; curr = curr->next)
      {
        insert(tmp, curr->val.key, curr->val.val);
      }
    }
  }
  catch (...)
  {
    clear(tmp);
    throw;
  }
  swap(ht, tmp);
  clear(tmp);
}

template< class K, class V >
void zharov::insert(HashTable< K, V >& ht, const K& key, const V& val)
{
  detail::Node< Entry< K, V > >* existing = find(ht, key);
  if (existing)
  {
    existing->val.val = val;
    return;
  }
  if (ht.size >= ht.cap)
  {
    rehash(ht, ht.cap * 2 + 1);
  }
  const size_t idx = std::hash< K >{}(key) % ht.cap;
  pushBack(ht.slots[idx], Entry< K, V >{key, val});
  ++ht.size;
}

template< class K, class V >
void zharov::remove(HashTable< K, V >& ht, const K& key)
{
  const size_t idx = std::hash< K >{}(key) % ht.cap;
  for (detail::Node< Entry< K, V > >* curr = ht.slots[idx].head; curr != nullptr; curr = curr->next)
  {
    if (curr->val.key == key)
    {
      remove(ht.slots[idx], curr);
      --ht.size;
      return;
    }
  }
}

#endif
