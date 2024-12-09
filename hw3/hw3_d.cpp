#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>
#include <iomanip>

// Define a linked list binary tree class template

template <class T> class LinkedBinaryTree;
template <class T> class LinkedHeader;
template <class T> 
class TreeNode {
  TreeNode<T>* LeftChild;
  T data;
  TreeNode<T>* RightChild;
  friend LinkedBinaryTree<T>;

public:
  TreeNode() : LeftChild(nullptr), RightChild(nullptr) {};
  TreeNode(T d) : LeftChild(nullptr), RightChild(nullptr), data(d) {};
  ~TreeNode() {};

  friend std::ostream& operator<<(std::ostream& out, const TreeNode<T>* node) {
    if (node) out << node->data;
    return out;
  };
};


template <class T> class HeaderNode {
  TreeNode<T>* data;
  HeaderNode<T>* link;
public:
  HeaderNode() : data(nullptr), link(nullptr) {};
  ~HeaderNode() {};
};

template <class T> class HeaderList {
  HeaderNode<T>* first;
  HeaderNode<T>* last;
public:
  HeaderList() : first(nullptr), last(nullptr) {};
  ~HeaderList() {};
};

template <class T> class LinkedHeader {
  std::vector<HeaderList<T>*> list;
public:
  LinkedHeader() {};

};

template <class T> 
class LinkedBinaryTree {
  TreeNode<T>* root;
  std::vector<TreeNode<T>*> selfTree;


  void deleteNode(TreeNode<T>* node) {
    if (!node) return;

    deleteNode(node->LeftChild);
    deleteNode(node->RightChild);
    delete node;

    // Delete the element in the self tree
    // Find the parent and set the pointer to this child to null if can be found
    // Erase child from self tree list
  };

  void insert(TreeNode<T>* node, T data) {
    TreeNode<T>* newNode = new TreeNode<T>(data);
    // Use the self tree to insert data
    if (!node) {
      // You are at the root node
      selfTree.push_back(newNode);
      root = newNode;
      return;
    } else {
      // Find the parent of the new node
      int parentIndex = (selfTree.size() - 1) / 2;
      if (parentIndex >= 0) {
        TreeNode<T>* parent = selfTree[parentIndex];
        if (!parent->LeftChild) 
          parent->LeftChild = newNode;
        else 
          parent->RightChild = newNode;
        selfTree.push_back(newNode);
      }
    }
  };

  int height(const TreeNode<T> *node) const {
    if (!node)
      return 0;

    int hLeft = height(node->LeftChild);
    int hRight = height(node->RightChild);

    return (hLeft > hRight) ? hLeft + 1 : hRight + 1;
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
  LinkedBinaryTree() : root(nullptr) {};
  ~LinkedBinaryTree() { 
    deleteNode(root);
    selfTree.clear();
  };

  void insert(T data);

  friend std::ostream& operator<<(std::ostream& out, const LinkedBinaryTree<T>* tree) {
    if (tree) {
      tree->printTree();
    };
    return out;
  };

  friend std::ostream& operator<<(std::ostream& out, const LinkedBinaryTree<T>& tree) {
    if (tree.root) {
      tree.printTree();
    };
    return out;
  };
};

template <class T> void LinkedBinaryTree<T>::insert(T data) { insert(root, data);} ;

int main (int argc, char *argv[]) {
  srand(time(NULL));
  LinkedBinaryTree<int> tree;

  for (int i = 0; i < 10; i++)
    tree.insert(rand() % 10);

  std::cout << tree << std:: endl;

  return 0;
}
