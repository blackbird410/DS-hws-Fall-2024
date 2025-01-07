template <class T>
class Element {
    public:
	T key;
};

template <class T>
class BST;
template <class T>
class BSTNode {
  friend class Tree;
    public:
    BSTNode();
    BSTNode(Element<T> &x) {data = x.key;};
    private:
    BSTNode *LeftChild;
    T data;
    BSTNode *RightChild;
};

template <class T>
class BST {
  public:
  // Tree operations
      BSTNode<T>* Search(const Element<T>& x);
      BSTNode<T>* Search(BSTNode<T>*b, const Element <T>&x);
      BSTNode<T>* IterSearch(const Element<T>& x);
      void Insert(const Element<T>& x) ;
  private:
    BSTNode<T> *root;
};


template <class T> //Driver
BSTNode<T>* BST<T>::Search(const Element<T>& x)
/* Search the binary search tree (*this) for an element with key x. If such an element is found, return a pointer to the node that contains it. */
{
  return Search(root, x);
}

template <class T> //Workhorse
BSTNode<T>* BST<T>::Search(BSTNode<T>*b,
const Element <T>&x)
{
  if(!b) return 0;
  if(x.key == b->data.key) return b;
  if(x.key < b->data.key) 
    return Search(b->LeftChild, x);
  return Search(b->RightChild, x);
}  //recursive version

template <class T>
BSTNode<T>* BST<T>::IterSearch(const Element<T>& x)
/* Search the binary search tree for an element with key x */
{
  for(BSTNode<T> *t = root; t; )
  {
    if(x.key == t->data.key) return t;
    if(x.key < t->data.key) t = t->LeftChild;
    else t = t->RightChild;
  }
  return 0;
}  //Iterative version
template <class T>
void BST<T>::Insert(const Element<T>& x) {
    BSTNode<T> *p = root, *pp = 0;
    while (p) {
	pp = p;
	if ( x.key < p->data) p = p->LeftChild;
	else if (x.key  > p->data) p = p->RightChild;
	else { return; }
    }
    p = new BSTNode<T>(x);
    if (root) {
	if (x.key < pp->data) pp->LeftChild = p;
	else pp->RightChild = p;
    }
    else {
	root = p;
    }
}

