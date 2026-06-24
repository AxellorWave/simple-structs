#ifndef LIST_HPP
#define LIST_HPP
#include <cstddef>
#include <utility>

namespace zharov
{
  namespace detail
  {
    template< class T >
    struct Node
    {
      T val;
      Node< T >* next;
      Node< T >* prev;
    };
  }

  template< class T >
  struct List
  {
    detail::Node< T >* head;
    detail::Node< T >* tail;
    size_t size;
  };

  template< class T >
  List< T > makeList();

  template< class T >
  void clear(List< T >& lst);

  template< class T >
  void swap(List< T >& a, List< T >& b);

  template< class T >
  List< T > copy(const List< T >& src);

  template< class T >
  detail::Node< T >* insert(List< T >& lst, detail::Node< T >* pos, const T& val);

  template< class T >
  void remove(List< T >& lst, detail::Node< T >* pos);

  template< class T >
  void pushFront(List< T >& lst, const T& val);

  template< class T >
  void pushBack(List< T >& lst, const T& val);

  template< class T >
  void popFront(List< T >& lst);

  template< class T >
  void popBack(List< T >& lst);
}

template< class T >
zharov::List< T > zharov::makeList()
{
  return List< T >{nullptr, nullptr, 0};
}

template< class T >
void zharov::clear(List< T >& lst)
{
  detail::Node< T >* curr = lst.head;
  while (curr != nullptr)
  {
    detail::Node< T >* next = curr->next;
    delete curr;
    curr = next;
  }
  lst.head = nullptr;
  lst.tail = nullptr;
  lst.size = 0;
}

template< class T >
void zharov::swap(List< T >& a, List< T >& b)
{
  std::swap(a.head, b.head);
  std::swap(a.tail, b.tail);
  std::swap(a.size, b.size);
}

template< class T >
zharov::List< T > zharov::copy(const List< T >& src)
{
  List< T > res = makeList< T >();
  try
  {
    for (detail::Node< T >* curr = src.head; curr != nullptr; curr = curr->next)
    {
      pushBack(res, curr->val);
    }
  }
  catch (...)
  {
    clear(res);
    throw;
  }
  return res;
}

template< class T >
zharov::detail::Node< T >* zharov::insert(List< T >& lst, detail::Node< T >* pos, const T& val)
{
  detail::Node< T >* newNode = new detail::Node< T >{val, pos, pos ? pos->prev : lst.tail};
  if (newNode->prev)
  {
    newNode->prev->next = newNode;
  }
  else
  {
    lst.head = newNode;
  }
  if (newNode->next)
  {
    newNode->next->prev = newNode;
  }
  else
  {
    lst.tail = newNode;
  }
  ++lst.size;
  return newNode;
}

template< class T >
void zharov::remove(List< T >& lst, detail::Node< T >* pos)
{
  if (pos->prev)
  {
    pos->prev->next = pos->next;
  }
  else
  {
    lst.head = pos->next;
  }
  if (pos->next)
  {
    pos->next->prev = pos->prev;
  }
  else
  {
    lst.tail = pos->prev;
  }
  delete pos;
  --lst.size;
}

template< class T >
void zharov::pushFront(List< T >& lst, const T& val)
{
  insert(lst, lst.head, val);
}

template< class T >
void zharov::pushBack(List< T >& lst, const T& val)
{
  insert(lst, static_cast< detail::Node< T >* >(nullptr), val);
}

template< class T >
void zharov::popFront(List< T >& lst)
{
  remove(lst, lst.head);
}

template< class T >
void zharov::popBack(List< T >& lst)
{
  remove(lst, lst.tail);
}

#endif
