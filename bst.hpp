#ifndef BST_HPP
#define BST_HPP
#include <cstddef>
#include <utility>

namespace zharov
{
  namespace detail
  {
    template< class K, class V >
    struct BstNode
    {
      K key;
      V val;
      BstNode< K, V >* left;
      BstNode< K, V >* right;
    };

    template< class K, class V >
    void clearNode(BstNode< K, V >* node);

    template< class K, class V >
    BstNode< K, V >* copyNode(const BstNode< K, V >* node);

    template< class K, class V >
    BstNode< K, V >* findNode(BstNode< K, V >* node, const K& key);

    template< class K, class V >
    BstNode< K, V >* minNode(BstNode< K, V >* node);

    template< class K, class V >
    BstNode< K, V >* insertNode(BstNode< K, V >* node, const K& key, const V& val, bool& inserted);

    template< class K, class V >
    BstNode< K, V >* removeNode(BstNode< K, V >* node, const K& key, bool& removed);
  }

  template< class K, class V >
  struct Bst
  {
    detail::BstNode< K, V >* root;
    size_t size;
  };

  template< class K, class V >
  Bst< K, V > makeBst();

  template< class K, class V >
  void clear(Bst< K, V >& bst);

  template< class K, class V >
  Bst< K, V > copy(const Bst< K, V >& src);

  template< class K, class V >
  void swap(Bst< K, V >& a, Bst< K, V >& b);

  template< class K, class V >
  detail::BstNode< K, V >* find(const Bst< K, V >& bst, const K& key);

  template< class K, class V >
  void insert(Bst< K, V >& bst, const K& key, const V& val);

  template< class K, class V >
  void remove(Bst< K, V >& bst, const K& key);
}

template< class K, class V >
void zharov::detail::clearNode(BstNode< K, V >* node)
{
  if (node == nullptr)
  {
    return;
  }
  clearNode(node->left);
  clearNode(node->right);
  delete node;
}

template< class K, class V >
zharov::detail::BstNode< K, V >* zharov::detail::copyNode(const BstNode< K, V >* node)
{
  if (node == nullptr)
  {
    return nullptr;
  }
  BstNode< K, V >* res = new BstNode< K, V >{node->key, node->val, nullptr, nullptr};
  try
  {
    res->left = copyNode(node->left);
    res->right = copyNode(node->right);
  }
  catch (...)
  {
    clearNode(res);
    throw;
  }
  return res;
}

template< class K, class V >
zharov::detail::BstNode< K, V >* zharov::detail::findNode(BstNode< K, V >* node, const K& key)
{
  if (node == nullptr)
  {
    return nullptr;
  }
  if (key < node->key)
  {
    return findNode(node->left, key);
  }
  if (node->key < key)
  {
    return findNode(node->right, key);
  }
  return node;
}

template< class K, class V >
zharov::detail::BstNode< K, V >* zharov::detail::minNode(BstNode< K, V >* node)
{
  while (node->left != nullptr)
  {
    node = node->left;
  }
  return node;
}

template< class K, class V >
zharov::detail::BstNode< K, V >* zharov::detail::insertNode(
  BstNode< K, V >* node,
  const K& key,
  const V& val,
  bool& inserted)
{
  if (node == nullptr)
  {
    inserted = true;
    return new BstNode< K, V >{key, val, nullptr, nullptr};
  }
  if (key < node->key)
  {
    node->left = insertNode(node->left, key, val, inserted);
  }
  else if (node->key < key)
  {
    node->right = insertNode(node->right, key, val, inserted);
  }
  else
  {
    node->val = val;
  }
  return node;
}

template< class K, class V >
zharov::detail::BstNode< K, V >* zharov::detail::removeNode(
  BstNode< K, V >* node,
  const K& key,
  bool& removed)
{
  if (node == nullptr)
  {
    return nullptr;
  }
  if (key < node->key)
  {
    node->left = removeNode(node->left, key, removed);
  }
  else if (node->key < key)
  {
    node->right = removeNode(node->right, key, removed);
  }
  else
  {
    removed = true;
    if (node->left == nullptr)
    {
      BstNode< K, V >* right = node->right;
      delete node;
      return right;
    }
    if (node->right == nullptr)
    {
      BstNode< K, V >* left = node->left;
      delete node;
      return left;
    }
    BstNode< K, V >* successor = minNode(node->right);
    node->key = successor->key;
    node->val = successor->val;
    node->right = removeNode(node->right, successor->key, removed);
  }
  return node;
}

template< class K, class V >
zharov::Bst< K, V > zharov::makeBst()
{
  return Bst< K, V >{nullptr, 0};
}

template< class K, class V >
void zharov::clear(Bst< K, V >& bst)
{
  detail::clearNode(bst.root);
  bst.root = nullptr;
  bst.size = 0;
}

template< class K, class V >
zharov::Bst< K, V > zharov::copy(const Bst< K, V >& src)
{
  return Bst< K, V >{detail::copyNode(src.root), src.size};
}

template< class K, class V >
void zharov::swap(Bst< K, V >& a, Bst< K, V >& b)
{
  std::swap(a.root, b.root);
  std::swap(a.size, b.size);
}

template< class K, class V >
zharov::detail::BstNode< K, V >* zharov::find(const Bst< K, V >& bst, const K& key)
{
  return detail::findNode(bst.root, key);
}

template< class K, class V >
void zharov::insert(Bst< K, V >& bst, const K& key, const V& val)
{
  bool inserted = false;
  bst.root = detail::insertNode(bst.root, key, val, inserted);
  if (inserted)
  {
    ++bst.size;
  }
}

template< class K, class V >
void zharov::remove(Bst< K, V >& bst, const K& key)
{
  bool removed = false;
  bst.root = detail::removeNode(bst.root, key, removed);
  if (removed)
  {
    --bst.size;
  }
}

#endif
