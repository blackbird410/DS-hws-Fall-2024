#include <cmath>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

enum Order { PREORDER, INORDER, POSTORDER };

template <class T> class ParentChild;
template <class T> class Tree;
template <class T> class TreeNode {
  TreeNode *LeftChild;
  TreeNode *RightChild;
  T data;

  friend class ParentChild<T>;
  friend class Tree<T>;

public:
  TreeNode() {};
  TreeNode(T d, TreeNode *l = nullptr, TreeNode *r = nullptr)
      : LeftChild(l), RightChild(r), data(d) {};
  ~TreeNode() {};

  bool operator<(const TreeNode<T> *other) const {
    return (other) ? data < other->data : false;
  };

  bool operator>(const TreeNode<T> *other) const {
    return (other) ? data > other->data : false;
  };

  friend std::ostream &operator<<(std::ostream &out, const TreeNode<T> *node) {
    if (node)
      out << node->data;
    return out;
  };
};

template <class T> class ParentChild {
  TreeNode<T> *node;
  TreeNode<T> *parent;

  friend class Tree<T>;

public:
  ParentChild() : node(nullptr), parent(nullptr) {};
  ~ParentChild() {};
};

template <class T> class Tree {
  TreeNode<T> *root;
  bool isReversed;

  void deleteNode(TreeNode<T> *node) {
    if (!node)
      return;

    deleteNode(node->LeftChild);
    deleteNode(node->RightChild);
    delete node;
  };

  void insertNode(TreeNode<T> *&parent, TreeNode<T> *newNode) {
    if (!parent) {
      parent = newNode;
      return;
    }

    if (*newNode < parent)
      insertNode(parent->LeftChild, newNode);
    else
      insertNode(parent->RightChild, newNode);
  };

  void traverse(TreeNode<T> *node, int order = INORDER) const {
    if (!node)
      return;

    switch (order) {
    case PREORDER:
      std::cout << node << " ";
      traverse(node->LeftChild, order);
      traverse(node->RightChild, order);
      break;
    case INORDER:
      traverse(node->LeftChild, order);
      std::cout << node << " ";
      traverse(node->RightChild, order);
      break;
    case POSTORDER:
      traverse(node->LeftChild, order);
      traverse(node->RightChild, order);
      std::cout << node << " ";
      break;
    default:
      break;
    }
  };

  int height(const TreeNode<T> *node) const {
    if (!node)
      return 0;

    int hLeft = height(node->LeftChild);
    int hRight = height(node->RightChild);

    return (hLeft > hRight) ? hLeft + 1 : hRight + 1;
  };

  int heightBF(const TreeNode<T> *node) const {
    return (!node) ? 0 : height(node->LeftChild) - height(node->RightChild);
  };
  int weightBF(const TreeNode<T> *node) const {
    return (!node) ? 0 : weight(node->LeftChild) - weight(node->RightChild);
  };

  void reverse(TreeNode<T> *node) {
    if (!node || (!node->LeftChild && !node->RightChild))
      return;

    reverse(node->LeftChild);
    reverse(node->RightChild);

    TreeNode<T> *tmp = node->RightChild;
    node->RightChild = node->LeftChild;
    node->LeftChild = tmp;
  };

  int weight(TreeNode<T> *node) const {
    if (!node)
      return 0;
    if (!node->LeftChild && !node->RightChild)
      return 1;

    return weight(node->LeftChild) + weight(node->RightChild) + 1;
  };

  bool isTreeReversed() const { return isReversed; };

  T *Get(TreeNode<T> *node, T target) const {
    if (!node)
      return nullptr;
    if (target == node->data)
      return &node->data;

    if (isTreeReversed())
      return (node->data > target) ? Get(node->RightChild, target)
                                   : Get(node->LeftChild, target);
    else
      return (node->data < target) ? Get(node->RightChild, target)
                                   : Get(node->LeftChild, target);
  };

  bool deleteNode(TreeNode<T> *parent, TreeNode<T> *node, T target) {
    if (!node)
      return false;

    if (node->data == target) {
      TreeNode<T> *successor = nullptr;
      ParentChild<T> *pc = nullptr;

      if (node->LeftChild && !node->RightChild)
        successor = node->LeftChild;
      else if (node->RightChild && !node->LeftChild)
        successor = node->RightChild;
      else if (node->LeftChild && node->RightChild)
        pc = findRightMostNode(node->LeftChild);

      if (!successor && !pc) {
        if (parent) {
          if (parent->LeftChild == node)
            parent->LeftChild = nullptr;
          else
            parent->RightChild = nullptr;
        } else
          root = nullptr;

        delete node;
      } else if (successor) {
        swap(successor, node);
        node->LeftChild = successor->LeftChild;
        node->RightChild = successor->RightChild;
        delete successor;
      } else {
        swap(pc->node, node);
        if (pc->parent)
          pc->parent->RightChild = nullptr;
        else
          node->LeftChild = pc->node->LeftChild;
        delete pc->node;
      }
      return true;
    } else {
      if (isTreeReversed())
        return (target < node->data)
                   ? deleteNode(node, node->RightChild, target)
                   : deleteNode(node, node->LeftChild, target);
      else
        return (target > node->data)
                   ? deleteNode(node, node->RightChild, target)
                   : deleteNode(node, node->LeftChild, target);
    }
  }

  ParentChild<T> *findRightMostNode(TreeNode<T> *node) {
    if (!node)
      return nullptr;

    ParentChild<T> *pc = new ParentChild<T>();
    if (!node->RightChild) {
      pc->node = node;
      return pc;
    }

    if (!node->RightChild->RightChild) {
      pc->parent = node;
      pc->node = node->RightChild;
      return pc;
    } else
      return findRightMostNode(node->RightChild);
  };

  void swap(TreeNode<T> *a, TreeNode<T> *b) {
    if (!a || !b || a == b)
      return;

    T tmp = a->data;
    a->data = b->data;
    b->data = tmp;
  };

  void printTree() const {
    if (!root) {
      std::cout << "The tree is empty." << std::endl;
      return;
    }

    int h = height(root);
    int maxWidth = std::pow(2, h) - 1;

    std::vector<TreeNode<T> *> currentLevel;
    currentLevel.push_back(root);
    std::vector<TreeNode<T> *> nextLevel;

    int depth = 0;
    while (!currentLevel.empty() && depth < h) {
      int indent = std::pow(2, h - depth - 1) - 1;
      int betweenSpaces = std::pow(2, h - depth) - 1;

      std::cout << std::string(indent * 2, ' ');

      for (auto node : currentLevel) {
        if (node) {
          std::cout << std::setw(2) << node->data;
          nextLevel.push_back(node->LeftChild);
          nextLevel.push_back(node->RightChild);
        } else {
          std::cout << "  ";
          nextLevel.push_back(nullptr);
          nextLevel.push_back(nullptr);
        }

        std::cout << std::string(betweenSpaces * 2, ' ');
      }
      std::cout << std::endl;

      currentLevel = nextLevel;
      nextLevel.clear();
      depth++;
    }

    for (int i = 0; i < maxWidth; i++)
      std::cout << "__";
    std::cout << std::endl;
  }

public:
  Tree() : root(nullptr), isReversed(false) {};
  ~Tree() { deleteNode(root); };

  void insert(T k);
  void preOrder() const;
  void inOrder() const;
  void postOrder() const;

  int height() const;
  int weight() const;
  int heightBF() const;
  int weightBF() const;
  void reverse();
  T *Get(T k) const;
  bool deleteNode(T k);

  friend std::ostream &operator<<(std::ostream &out, const Tree<T> *tree) {
    tree->printTree();
    return out;
  };

  friend std::ostream &operator<<(std::ostream &out, const Tree<T> &tree) {
    tree.printTree();
    return out;
  };
};

template <class T> void Tree<T>::insert(T k) {
  TreeNode<T> *newNode = new TreeNode<T>(k);
  insertNode(root, newNode);
};

template <class T> void Tree<T>::preOrder() const { traverse(root, PREORDER); };
template <class T> void Tree<T>::inOrder() const { traverse(root); };
template <class T> void Tree<T>::postOrder() const {
  traverse(root, POSTORDER);
};
template <class T> int Tree<T>::height() const { return height(root) - 1; };
template <class T> int Tree<T>::weight() const { return weight(root); };
template <class T> int Tree<T>::heightBF() const { return heightBF(root); };
template <class T> int Tree<T>::weightBF() const { return weightBF(root); };
template <class T> void Tree<T>::reverse() {
  reverse(root);
  isReversed = !isReversed;
};
template <class T> T *Tree<T>::Get(T value) const { return Get(root, value); };
template <class T> bool Tree<T>::deleteNode(T value) {
  return deleteNode(nullptr, root, value);
};

// int main() {
//   srand(time(NULL));
//   Tree<int> bst;
//   std::string cmd;
//   int v;
//   int *searchResult;
//   int sample[] = {10, 7, 15, 3, 9, 24, 1, 8, 20, 27, 4, 6, 17, 22, 25, 30};
//   int n = sizeof(sample) / sizeof(sample[0]);
//   for (int i = 0; i < n; i++)
//     bst.insert(sample[i]);
//   std::cout << bst << std::endl;
// 
//   while (std::getline(std::cin, cmd)) {
//     std::stringstream ss(cmd);
//     ss >> cmd;
//     if (cmd == "insert") {
//       ss >> cmd;
//       v = std::stoi(cmd);
//       bst.insert(v);
//     } else if (cmd == "preorder")
//       bst.preOrder();
//     else if (cmd == "inorder")
//       bst.inOrder();
//     else if (cmd == "postorder")
//       bst.postOrder();
//     else if (cmd == "height")
//       std::cout << "Height: " << bst.height() << std::endl;
//     else if (cmd == "weight")
//       std::cout << "Weight: " << bst.weight() << std::endl;
//     else if (cmd == "heightBF")
//       std::cout << "HeightBF: " << bst.heightBF() << std::endl;
//     else if (cmd == "weightBF")
//       std::cout << "WeightBF: " << bst.weightBF() << std::endl;
//     else if (cmd == "reverse")
//       bst.reverse();
//     else if (cmd == "get") {
//       ss >> cmd;
//       v = std::stoi(cmd);
//       searchResult = bst.Get(v);
//       if (searchResult)
//         std::cout << *searchResult << " found" << std::endl;
//       else
//         std::cout << v << " not found" << std::endl;
//     } else if (cmd == "delete") {
//       ss >> cmd;
//       v = std::stoi(cmd);
//       if (bst.deleteNode(v))
//         std::cout << v << " was removed from tree" << std::endl;
//       else
//         std::cout << "Could not find " << v << std::endl;
//     } else if (cmd == "print")
//       std::cout << bst << std::endl;
//     else if (cmd == "exit")
//       break;
//     std::cout << "\n" << bst << std::endl;
//   }
// 
//   return 0;
// }
