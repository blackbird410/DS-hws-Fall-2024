#include <cmath>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

// Areas of application of the Linked Binary Tree class
// ----------------------------------------------------
// 1- Character matching
//    All characters having the same values can be found instantly by using the
//    header list. This allows us to find the number of occurences of character
//    in a text, also have a character frequency chart that might be useful in
//    text analysis.
//
// 2- Fast Jump Reading
//    Because all the nodes in the tree have pointers associated with them and
//    we use those pointers to build the LinkedHeaderList that gives us a list
//    of all the nodes for a unique character, this could be used to JUMP from
//    one character to the next occurence without using a loop or another
//    iterative method.
//
// 3- Course Utility Management
//    We could modify this class to hand other kinds of data that would still
//    have the same functionality where the tree could store information for
//    courses in a University X and a list of students N. We could actually use
//    the HeaderLinkedList with some additional parameters and find for example:
//      - The number of course attended by a student Y
//      - Efficiently find the courses that are being attend by the student Y,
//      other related informations such as grades, attendances ...and others.
//
// 4- Task Manager
//    Let's say we are building an app that has this task manager utility, and
//    we need to quickly find a list of related tasks based for example on
//    priorities, subject or something else in common. This Linked Binary Tree
//    class could also help in that case, and be even more efficient than
//    conventional iterative methods.
//      - We could create the tree based on a criteria, priority for example.
//      - Add the tasks to the tree and set up the LinkedHeaderList to be build
//      using the same priority criteria.
//      - After the tree is successfully built, the tasks having the same
//      priority would be available instantly without needing to traverse the
//      whole tree to find them.
//      - This would be a major plus in terms of efficiency, insertion would
//      still take a bit of time because it needs to search for the right header
//      node list, but it is a fair trade.
//

// Define a linked list binary tree class template
template <class T> class LinkedBinaryTree;
template <class T> class HeaderNode;
template <class T> class HeaderNodeList;
template <class T> class LinkedHeaderList;
template <class T> class TreeNode {
  TreeNode<T> *LeftChild;
  T data;
  TreeNode<T> *RightChild;
  friend LinkedBinaryTree<T>;
  friend HeaderNode<T>;
  friend HeaderNodeList<T>;
  friend LinkedHeaderList<T>;

public:
  TreeNode() : LeftChild(nullptr), RightChild(nullptr) {};
  TreeNode(T d) : LeftChild(nullptr), RightChild(nullptr), data(d) {};
  ~TreeNode() {};

  friend std::ostream &operator<<(std::ostream &out, const TreeNode<T> *node) {
    if (node)
      out << node->data;
    return out;
  };
};

template <class T> class HeaderNode {
  TreeNode<T> *data;
  HeaderNode<T> *link;

  friend HeaderNodeList<T>;
  friend LinkedHeaderList<T>;

public:
  HeaderNode() : data(nullptr), link(nullptr) {};
  HeaderNode(TreeNode<T> *d) : data(d), link(nullptr) {};
  ~HeaderNode() {};

  friend std::ostream &operator<<(std::ostream &out,
                                  const HeaderNode<T> *node) {
    if (node)
      out << node->data;
    return out;
  };
};

template <class T> class HeaderNodeList {
  HeaderNode<T> *first;
  HeaderNode<T> *last;

  friend LinkedHeaderList<T>;

public:
  HeaderNodeList() : first(nullptr), last(nullptr) {};
  HeaderNodeList(TreeNode<T> *newNode) {
    first = last = new HeaderNode(newNode);
  };
  ~HeaderNodeList() { deleteNode(first); };

  void deleteNode(HeaderNode<T> *node) {
    if (!node)
      return;

    deleteNode(node->link);
    delete node;
  };

  void insert(TreeNode<T> *d) {
    HeaderNode<T> *newNode = new HeaderNode<T>(d);
    if (!first) {
      first = last = newNode;
      return;
    }
    last->link = newNode;
    last = newNode;
  };

  friend std::ostream &operator<<(std::ostream &out,
                                  const HeaderNodeList<T> *list) {
    if (list) {
      HeaderNode<T> *tmp = list->first;
      while (tmp) {
        out << tmp;
        if (tmp->link)
          out << " ";
        tmp = tmp->link;
      }
    }

    return out;
  };
};

template <class T> class LinkedHeaderList {
  std::vector<HeaderNodeList<T> *> lists;

public:
  LinkedHeaderList() {};
  ~LinkedHeaderList() {
    for (auto &e : lists)
      delete e;
    lists.clear();
  };

  HeaderNodeList<T> *findHeaderList(TreeNode<T> *node) {
    // Returns the header list pointer for the data if it exist or nullptr if
    // not

    for (auto &list : lists)
      if (list->first->data->data == node->data)
        return list;

    return nullptr;
  };

  void insert(TreeNode<T> *treeNode) {
    // Insert the node in an existing header list that has the same data value
    // or create a new one if not found

    if (!treeNode)
      return;

    HeaderNodeList<T> *list = findHeaderList(treeNode);
    if (list)
      list->insert(treeNode);
    else {
      list = new HeaderNodeList<T>(treeNode);
      lists.push_back(list);
    }
  };

  friend std::ostream &operator<<(std::ostream &out,
                                  const LinkedHeaderList<T> *hList) {
    if (hList) {
      for (auto &list : hList->lists)
        out << list << std::endl;
    }

    return out;
  };
};

template <class T> class LinkedBinaryTree {
  TreeNode<T> *root;
  std::vector<TreeNode<T> *> selfTree;
  LinkedHeaderList<T> headerList;

  void deleteNode(TreeNode<T> *node) {
    if (!node)
      return;

    deleteNode(node->LeftChild);
    deleteNode(node->RightChild);
    delete node;
  };

  void insert(TreeNode<T> *node, T data) {
    // Use the self tree to insert data in level order traversal
    TreeNode<T> *newNode = new TreeNode<T>(data);
    headerList.insert(newNode);

    if (!node) {
      // Root node
      selfTree.push_back(newNode);
      root = newNode;
      return;
    } else {
      // Find the parent of the new node
      int parentIndex = (selfTree.size() - 1) / 2;
      if (parentIndex >= 0) {
        TreeNode<T> *parent = selfTree[parentIndex];
        if (!parent->LeftChild)
          parent->LeftChild = newNode;
        else
          parent->RightChild = newNode;
        selfTree.push_back(newNode);
      }
    }
  };

  int height(const TreeNode<T> *node) const {
    // Height of any node in the tree + 1
    if (!node)
      return 0;

    int hLeft = height(node->LeftChild);
    int hRight = height(node->RightChild);

    return (hLeft > hRight) ? hLeft + 1 : hRight + 1;
  };

  void printTree() const {
    // Displays the tree as it would be drawn on paper
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

  LinkedHeaderList<T> *getHeaderList() { return &headerList; };
  void insert(T data);

  friend std::ostream &operator<<(std::ostream &out,
                                  const LinkedBinaryTree<T> *tree) {
    if (tree)
      tree->printTree();
    return out;
  };

  friend std::ostream &operator<<(std::ostream &out,
                                  const LinkedBinaryTree<T> &tree) {
    if (tree.root)
      tree.printTree();
    return out;
  };
};

template <class T> void LinkedBinaryTree<T>::insert(T data) {
  insert(root, data);
};

// int main() {
//   std::string s;
//   LinkedBinaryTree<char> tree;
// 
//   std::cout << "Enter a text: ";
//   std::getline(std::cin, s);
//   int n = s.length();
//   char arr[n + 1];
//   std::strcpy(arr, s.c_str());
// 
//   for (int i = 0; i < n; i++)
//     tree.insert(arr[i]);
// 
//   std::cout << tree << std::endl;
//   std::cout << "Header List:\n" << tree.getHeaderList() << std::endl;
// 
//   return 0;
// }
// 
