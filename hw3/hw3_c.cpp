#include <ctime>
#include <iostream>

template <class T> class Element {
public:
  T key;
};

template <class T> class BST;
template <class T> class BSTNode {
  friend class Tree;

public:
  BSTNode();
  BSTNode(const Element<T> &x) { data = x.key; };
  BSTNode(T &d) { data = d; }

private:
  BSTNode *LeftChild;
  T data;
  BSTNode *RightChild;

  friend class BST<T>;
  friend std::ostream &operator<<(std::ostream &out, const BSTNode<T> *node) {
    if (node)
      out << node->data;
    return out;
  };
};

template <class T> class BST {
public:
  BST() : root(nullptr), isReversed(false) {};
  ~BST() { deleteNode(root); };

  // Tree operations
  BSTNode<T> *Search(const Element<T> &x);
  BSTNode<T> *Search(BSTNode<T> *b, const Element<T> &x);
  BSTNode<T> *IterSearch(const Element<T> &x);
  void Insert(const Element<T> &x);
  int height() const;
  int weight() const;
  int heightBF() const;
  int weightBF() const;
  void reverse();

  void preOrder() const { preOrder(root); };
  void inOrder() const { inOrder(root); };
  void postOrder() const { postOrder(root); };

private:
  BSTNode<T> *root;
  bool isReversed;

  void preOrder(BSTNode<T> *node) const {
    if (!node)
      return;
    std::cout << node->data << " ";
    preOrder(node->LeftChild);
    preOrder(node->RightChild);
  };
  void inOrder(BSTNode<T> *node) const {
    if (!node)
      return;
    inOrder(node->LeftChild);
    std::cout << node->data << " ";
    inOrder(node->RightChild);
  };
  void postOrder(BSTNode<T> *node) const {
    if (!node)
      return;
    postOrder(node->LeftChild);
    postOrder(node->RightChild);
    std::cout << node->data << " ";
  };

  int height(BSTNode<T> *node) const {
    if (!node || (!node->LeftChild && !node->RightChild))
      return 0;

    int hLeft = height(node->LeftChild);
    int hRight = height(node->RightChild);

    return (hLeft > hRight) ? hLeft + 1 : hRight + 1;
  };

  int weight(BSTNode<T> *node) const {
    return (node) ? weight(node->LeftChild) + weight(node->RightChild) + 1 : 0;
  };
  int heightBF(BSTNode<T> *node) const {
    return height(node->LeftChild) - height(node->RightChild);
  };
  int weightBF(BSTNode<T> *node) const {
    return weight(node->LeftChild) - weight(node->RightChild);
  };

  void reverse(BSTNode<T> *node) {
    if (!node)
      return;

    reverse(node->LeftChild);
    reverse(node->RightChild);
    BSTNode<T> *tmp = node->LeftChild;
    node->LeftChild = node->RightChild;
    node->RightChild = tmp;
  };

  void deleteNode(BSTNode<T> *node) {
    if (!node)
      return;

    deleteNode(node->LeftChild);
    deleteNode(node->RightChild);
    delete node;
  };
};

template <class T> // Driver
BSTNode<T> *BST<T>::Search(const Element<T> &x)
/* Search the binary search tree (*this) for an element with key x. If such an
   element is found, return a pointer to the node that contains it. */
{
  return Search(root, x);
}

template <class T> // Workhorse
BSTNode<T> *BST<T>::Search(BSTNode<T> *b, const Element<T> &x) {
  if (!b)
    return 0;
  if (x.key == b->data)
    return b;

  if (isReversed)
    return (x.key > b->data) ? Search(b->LeftChild, x)
                             : Search(b->RightChild, x);
  else
    return (x.key < b->data) ? Search(b->LeftChild, x)
                             : Search(b->RightChild, x);
} // recursive version

template <class T>
BSTNode<T> *BST<T>::IterSearch(const Element<T> &x)
/* Search the binary search tree for an element with key x */
{
  for (BSTNode<T> *t = root; t;) {
    if (x.key == t->data.key)
      return t;
    if (x.key < t->data.key)
      t = t->LeftChild;
    else
      t = t->RightChild;
  }
  return 0;
} // Iterative version

template <class T> void BST<T>::Insert(const Element<T> &x) {
  BSTNode<T> *p = root, *pp = nullptr;

  while (p) {
    pp = p;
    if (isReversed)
      p = (x.key < p->data) ? p->RightChild : p->LeftChild;
    else {
      if (x.key < p->data)
        p = p->LeftChild;
      else if (x.key > p->data)
        p = p->RightChild;
      else
        return;
    }
  }

  p = new BSTNode<T>(x);
  if (root) {
    if (isReversed) {
      if (x.key < pp->data)
        pp->RightChild = p;
      else
        pp->LeftChild = p;
    } else {
      if (x.key < pp->data)
        pp->LeftChild = p;
      else
        pp->RightChild = p;
    }
  } else {
    root = p;
  }
}

template <class T> int BST<T>::height() const { return height(root); };
template <class T> int BST<T>::weight() const { return weight(root); };
template <class T> int BST<T>::heightBF() const { return heightBF(root); };
template <class T> int BST<T>::weightBF() const { return weightBF(root); };
template <class T> void BST<T>::reverse() {
  reverse(root);
  isReversed = !isReversed;
};

// int main() {
//   srand(time(NULL));
//   BST<int> tree;
//   int sample[] = { 10, 6, 15, 3, 9, 11, 19, 2, 4, 7, 12, 16, 20};
//   Element<int> e;
//
//   int i, v;
//   for (i = 0; i < sizeof(sample) / sizeof(sample[0]); i++) {
//     e.key = sample[i];
//     tree.Insert(e);
//   }
//
//   tree.preOrder();
//   std::cout << std::endl;
//   tree.inOrder();
//   std::cout << std::endl;
//   tree.postOrder();
//   std::cout << std::endl;
//
//   tree.reverse();
//   tree.inOrder();
//   std::cout << std::endl;
//
//   std::cout << "Height: " << tree.height() << std::endl;
//   std::cout << "Weight: " << tree.weight() << std::endl;
//   std::cout << "HeightBF: " << tree.heightBF() << std::endl;
//   std::cout << "WeightBF: " << tree.weightBF() << std::endl;
//
//   for (i = 0; i < 5; i++) {
//     e.key = rand() % 100;
//     std::cout << "Inserting: " << e.key << std::endl;
//     tree.inOrder();
//     std::cout << std::endl;
//     tree.Insert(e);
//   }
//
//
//   tree.reverse();
//   tree.inOrder();
//   std::cout << std::endl;
//
//   std::cout << "Enter the number to search: ";
//   std::cin >> v;
//   e.key = v;
//   BSTNode<int>* target = tree.Search(e);
//   if (!target)
//     std::cout << v << " not found" << std::endl;
//   else
//     std::cout << v << " found at " << target << std::endl;
//
//   return 0;
// };
//
