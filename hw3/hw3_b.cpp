#include <cmath>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

enum Order { PREORDER, INORDER, POSTORDER };

template <class T> class Tree;
template <class T> class TreeNode {
  TreeNode *LeftChild;
  TreeNode *RightChild;
  T data;

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

template <class T> class Tree {
  TreeNode<T> *root;

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

  void printTree() const {
    if (!root) {
      std::cout << "The tree is empty." << std::endl;
      return;
    }

    int h = height(root);
    int maxWidth = std::pow(2, h) - 1;

    // Use a queue-like approach to store nodes for each level
    std::vector<TreeNode<T> *> currentLevel;
    currentLevel.push_back(root);
    std::vector<TreeNode<T> *> nextLevel;

    int depth = 0;
    while (!currentLevel.empty() && depth < h) {
      int indent = std::pow(2, h - depth - 1) - 1;
      int betweenSpaces = std::pow(2, h - depth) - 1;

      // Print leading indent
      std::cout << std::string(indent * 2, ' ');

      // Print the nodes at the current level
      for (auto node : currentLevel) {
        if (node) {
          std::cout << std::setw(2) << node->data;
          nextLevel.push_back(node->LeftChild);
          nextLevel.push_back(node->RightChild);
        } else {
          std::cout << "  "; // Placeholder for an empty node
          nextLevel.push_back(nullptr);
          nextLevel.push_back(nullptr);
        }

        // Print spaces between nodes
        std::cout << std::string(betweenSpaces * 2, ' ');
      }
      std::cout << std::endl;

      // Move to the next level
      currentLevel = nextLevel;
      nextLevel.clear();
      depth++;
    }

    for (int i = 0; i < maxWidth; i++)
      std::cout << "__";
    std::cout << std::endl;
  }

  int weight(TreeNode<T> *node) const {
    if (!node)
      return 0;
    if (!node->LeftChild && !node->RightChild)
      return 1;

    return weight(node->LeftChild) + weight(node->RightChild) + 1;
  };

public:
  Tree() : root(nullptr) {};
  ~Tree() { deleteNode(root); };

  void insert(T k);
  void preOrder() const;
  void inOrder() const;
  void postOrder() const;

  int height() const;
  int weight() const;
  int heightBF() const;
  int weightBF() const;

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

template <class T> void Tree<T>::preOrder() const { traverse(root, PREORDER);};
template <class T> void Tree<T>::inOrder() const { traverse(root);};
template <class T> void Tree<T>::postOrder() const { traverse(root, POSTORDER);};
template <class T> int Tree<T>::height() const { return height(root) - 1; };
template <class T> int Tree<T>::weight() const { return weight(root); };
template <class T> int Tree<T>::heightBF() const { return heightBF(root); };
template <class T> int Tree<T>::weightBF() const { return weightBF(root); };

int main() {
  srand(time(NULL));
  Tree<int> bst;
  std::string cmd;
  int v;

  while (std::getline(std::cin, cmd)) {
    std::stringstream ss(cmd);
    ss >> cmd;
    if (cmd == "insert") {
      ss >> cmd;
      v = std::stoi(cmd);
      bst.insert(v);
    } else if (cmd == "preorder")
      bst.preOrder();
    else if (cmd == "inorder")
      bst.inOrder();
    else if (cmd == "postorder")
      bst.postOrder();
    else if (cmd == "height")
      std::cout << "Height: " << bst.height() << std::endl;
    else if (cmd == "weight")
      std::cout << "Weight: " << bst.weight() << std::endl;
    else if (cmd == "heightBF")
      std::cout << "HeightBF: " << bst.heightBF() << std::endl;
    else if (cmd == "weightBF")
      std::cout << "WeightBF: " << bst.weightBF() << std::endl;
    else if (cmd == "print")
      std::cout << bst << std::endl;
    else if (cmd == "exit")
      break;
    std::cout << "\n" << bst << std::endl;
  }

  return 0;
}
