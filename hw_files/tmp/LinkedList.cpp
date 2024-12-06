#include <iostream>
#include <sstream>
#include <string>

template <class T> class LinkedList;
template <class T> class ListIterator;
template <class T> class Node {
  T data;
  Node<T> *next;

public:
  Node() : next(nullptr) {};
  ~Node() {};
  explicit Node<T>(const T &d, Node<T> *next = nullptr) : data(d), next(next){};

  friend class LinkedList<T>;
  friend class ListIterator<T>;

  friend std::ostream &operator<<(std::ostream &out, const Node<T> *n) {
    if (n)
      out << n->data;
    return out;
  };
};

template <class T> class ListIterator {
  Node<T> *current;
  const LinkedList<T> *list;

public:
  ListIterator() : list(nullptr), current(nullptr) {};
  ListIterator(const LinkedList<T> *l)
      : list(l), current(list ? list->head : nullptr) {};
  ~ListIterator() {};
  bool NotNull() { return !!current; };
  Node<T> *NextNotNull() {
    if (current && current->next) {
      current = current->next;
      return current;
    }

    return nullptr;
  };
  T *First() { return (list) ? &(list->head->data) : nullptr; };
  T *Next() {
    if (NotNull()) {
      current = current->next;
      return (current) ? &(current->data) : nullptr;
    }
    return nullptr;
  };

  friend LinkedList<T>;
};

template <class T> class LinkedList {
  Node<T> *head;
  Node<T> *tail;

public:
  LinkedList() : head(nullptr), tail(nullptr) {};
  ~LinkedList() { deleteNode(head); };

  void deleteNode(Node<T> *n) {
    if (!n)
      return;
    deleteNode(n->next);
    delete n;
  };

  void insert(const T &data) {
    Node<T> *newNode = new Node<T>(data);
    if (!head) {
      head = newNode;
      tail = head;
    } else {
      tail->next = newNode;
      tail = newNode;
    }
  };

  LinkedList<T> *operator+=(const LinkedList<T> *other) {
    if (!other || !other->head)
      return this; // Handle empty list
    Node<T> *temp = other->head;
    while (temp) {
      this->insert(temp->data); // Deep copy
      temp = temp->next;
    }
    return this;
  }

  LinkedList<T> *operator+(const LinkedList<T> *other) {
    LinkedList<T> *res = new LinkedList<T>();
    Node<T> *temp = this->head;
    while (temp) {
      res->insert(temp->data); // Copy nodes from `this`
      temp = temp->next;
    }
    temp = other->head;
    while (temp) {
      res->insert(temp->data); // Copy nodes from `other`
      temp = temp->next;
    }
    return res;
  }

  LinkedList<T> &operator=(const LinkedList<T> *other) {
    if (this == other)
      return *this; // Handle self-assignment

    // Clear current list
    deleteNode(this->head);
    this->head = this->tail = nullptr;

    // Copy contents from `other`
    Node<T> *temp = other->head;
    while (temp) {
      this->insert(temp->data); // Deep copy
      temp = temp->next;
    }
    return *this;
  }

  bool remove(const T &x) {
    if (!head) {
      std::cerr << "List is empty" << std::endl;
      return false;
    }

    Node<T> *tmp = head, *prev = nullptr;
    while (tmp && tmp->data != x) {
      prev = tmp;
      tmp = tmp->next;
    }

    if (tmp) {
      if (!prev)
        head = head->next;
      else if (!tmp->next)
        tail = prev;
      else
        prev->next = tmp->next;
      delete tmp;
      return true;
    }

    std::cout << "Target not found" << std::endl;
    return false;
  };

  friend ListIterator<T>;

  friend std::ostream &operator<<(std::ostream &out,
                                  const LinkedList<T> *list) {
    if (list && list->head) {
      ListIterator<T> it(list);
      T *tmp;
      for (tmp = it.First(); tmp; tmp = it.Next()) {
        out << tmp;
        if (it.NotNull())
          out << " -> ";
      }
    }

    return out;
  };
};

class ThreeLetterNode {
  char letters[4];

public:
  ThreeLetterNode() {};
  ThreeLetterNode(const char *l) {
    for (int i = 0; i < 3; i++) {
      letters[i] = l[i];
    }
    letters[3] = '\0';
  };

  bool operator!=(const ThreeLetterNode &other) {
    for (int i = 0; i < 3; i++)
      if (letters[i] != other.letters[i])
        return true;
    return false;
  };

  friend std::ostream &operator<<(std::ostream &out, const ThreeLetterNode &n) {
    out << n.letters;
    return out;
  };

  friend std::ostream &operator<<(std::ostream &out, const ThreeLetterNode *n) {
    out << n->letters;
    return out;
  };
};

void testLinkedList();
void testConcatenation();

int main() {
  testConcatenation();

  return 0;
}

void takeLetters(std::string v, char *input) {
  for (int i = 0; i < 3; i++)
    input[i] = v[i];
};

void testConcatenation() {
  LinkedList<ThreeLetterNode> list1, list2;
  std::string cmd;
  char input[4];

  while (std::getline(std::cin, cmd)) {
    std::stringstream ss(cmd);
    ss >> cmd;
    if (cmd == "insert") {
      ss >> cmd;
      takeLetters(cmd, input);
      list1.insert(input);
    } else if (cmd == "delete") {
      ss >> cmd;
      takeLetters(cmd, input);
      list1.remove(input);
    } else if (cmd == "insert2") {
      ss >> cmd;
      takeLetters(cmd, input);
      list2.insert(input);
    } else if (cmd == "delete2") {
      ss >> cmd;
      takeLetters(cmd, input);
      list2.remove(input);
    } else if (cmd == "concat") {
      list1 += &list2;
    } else if (cmd == "exit")
      return;
    else
      continue;
    std::cout << &list1 << std::endl;
  }
};

void testLinkedList() {
  LinkedList<ThreeLetterNode> list;
  std::string cmd;
  char input[4];

  while (std::getline(std::cin, cmd)) {
    std::stringstream ss(cmd);
    ss >> cmd;
    if (cmd == "insert") {
      ss >> cmd;
      takeLetters(cmd, input);
      list.insert(input);
    } else if (cmd == "delete") {
      ss >> cmd;
      takeLetters(cmd, input);
      list.remove(input);
    } else if (cmd == "exit")
      return;
    else
      continue;
    std::cout << &list << std::endl;
  }
};
