#ifndef LIST_HPP
#define LIST_HPP
#include <cstddef>
#include <utility>

namespace zharov
{
  namespace detail
  {
    template< class T >
    struct node
    {
      T val;
      node< T >* next;
      node< T >* prev;
    };
  }

  template< class T >
  struct list
  {
    detail::node< T >* head;
    detail::node< T >* tail;
    size_t size;
  };

  template< class T >
  list< T > makeList();

  template< class T >
  void clear(list< T >& lst);

  template< class T >
  void swap(list< T >& a, list< T >& b);

  template< class T >
  list< T > copy(const list< T >& src);

  template< class T >
  detail::node< T >* insert(list< T >& lst, detail::node< T >* pos, const T& val);

  template< class T >
  void remove(list< T >& lst, detail::node< T >* pos);

  template< class T >
  void pushFront(list< T >& lst, const T& val);

  template< class T >
  void pushBack(list< T >& lst, const T& val);

  template< class T >
  void popFront(list< T >& lst);

  template< class T >
  void popBack(list< T >& lst);
}

template< class T >
zharov::list< T > zharov::makeList()
{
  return list< T >{nullptr, nullptr, 0};
}

template< class T >
void zharov::clear(list< T >& lst)
{
  detail::node< T >* curr = lst.head;
  while (curr != nullptr)
  {
    detail::node< T >* next = curr->next;
    delete curr;
    curr = next;
  }
  lst.head = nullptr;
  lst.tail = nullptr;
  lst.size = 0;
}

template< class T >
void zharov::swap(list< T >& a, list< T >& b)
{
  std::swap(a.head, b.head);
  std::swap(a.tail, b.tail);
  std::swap(a.size, b.size);
}

template< class T >
zharov::list< T > zharov::copy(const list< T >& src)
{
  list< T > res = makeList< T >();
  try
  {
    for (detail::node< T >* curr = src.head; curr != nullptr; curr = curr->next)
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
zharov::detail::node< T >* zharov::insert(list< T >& lst, detail::node< T >* pos, const T& val)
{
  detail::node< T >* newNode = new detail::node< T >{val, pos, pos ? pos->prev : lst.tail};
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
void zharov::remove(list< T >& lst, detail::node< T >* pos)
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
void zharov::pushFront(list< T >& lst, const T& val)
{
  insert(lst, lst.head, val);
}

template< class T >
void zharov::pushBack(list< T >& lst, const T& val)
{
  insert(lst, static_cast< detail::node< T >* >(nullptr), val);
}

template< class T >
void zharov::popFront(list< T >& lst)
{
  remove(lst, lst.head);
}

template< class T >
void zharov::popBack(list< T >& lst)
{
  remove(lst, lst.tail);
}

#endif
