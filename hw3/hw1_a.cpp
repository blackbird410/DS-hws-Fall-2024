#include <iostream>
#include <sstream>
#include <string>

enum Boolean { FALSE, TRUE };
template <class T> class List;
template <class T> class ListIterator;
template <class T> class ListNode {
  T data;
  ListNode *link;

public:
  ListNode() : link(nullptr) {};
  ListNode(T d, ListNode *l = nullptr) : data(d), link(l) {};
  ~ListNode() {};

  friend class List<T>;
  friend class ListIterator<T>;

  friend std::ostream &operator<<(std::ostream &out, const ListNode<T> *node) {
    if (node)
      out << node->data;
    return out;
  };
};

template <class T> class ListIterator {
  const List<T> *list;
  ListNode<T> *current;

  friend class List<T>;

public:
  ListIterator(const List<T> &l) : list(&l), current(l.first) {};
  ListIterator(const List<T> *l) : list(l), current(l->first) {};
  ~ListIterator() {};

  ListIterator &operator++() {
    current = (current) ? current->link : nullptr;
    return *this;
  };

  bool NotNull() { return current != nullptr; };
  bool NextNotNull() { return NotNull() && current->link; };
  T *First() { return (list) ? &list->first->data : nullptr; };
  T *Last() { return (list) ? &list->last->data : nullptr; };
};

template <class T> class List {
  ListNode<T> *first, *last;
  friend class ListIterator<T>;

  void deleteNode(ListNode<T> *n) {
    if (n) {
      deleteNode(n->link);
      delete n;
    }
  };

  int count(ListNode<T> *n) const { return (n) ? count(n->link) + 1 : 0; };
  int count() const { return count(this->first); }

public:
  List() : first(nullptr), last(nullptr) {};
  ~List() { deleteNode(first); };

  void Attach(T k);
  void Invert();
  void Concatenate(List<T> b);
  bool appearM(T k, int m);
  int replaceALL(T a1, T a2);
  void rotate(char c, int k);
  List<T> subList(int start, int end);

  friend std::ostream &operator<<(std::ostream &out, const List<T> *l) {
    ListIterator<T> it(*l);

    while (it.NotNull()) {
      out << it.current->data;
      if (it.NextNotNull())
        out << " ";
      ++it;
    }
    return out;
  };

  friend std::ostream &operator<<(std::ostream &out, const List<T> &l) {
    ListIterator<T> it(l);

    while (it.NotNull()) {
      out << it.current->data;
      if (it.NextNotNull())
        out << " ";
      ++it;
    }
    return out;
  };
};

template <class T> void List<T>::Attach(T k) {
  ListNode<T> *newNode = new ListNode<T>(k);

  if (!first)
    first = last = newNode;
  else {
    last->link = newNode;
    last = newNode;
  }
};

template <class T> void List<T>::Invert() {
  ListNode<T> *p = first, *q = nullptr;

  while (p) {
    ListNode<T> *r = q;
    q = p;
    p = p->link;
    q->link = r;
  }
  first = q;
};

template <class T> void List<T>::Concatenate(List<T> b) {
  if (!first) {
    first = b.first;
    return;
  }

  if (b.first) {
    ListNode<T> *p;
    for (p = first; p->link; p = p->link)
      ;
    p->link = b.first;
  }
};

template <class T> bool List<T>::appearM(T k, int m) {
  int count = 0;
  ListIterator<T> it(*this);

  while (it.NotNull()) {
    count += !!(it.current->data == k);
    ++it;
  }

  return count == m;
};

template <class T> int List<T>::replaceALL(T a1, T a2) {
  int count = 0;
  ListIterator<T> it(*this);

  while (it.NotNull()) {
    if (it.current->data == a1) {
      it.current->data = a2;
      count++;
    }
    ++it;
  }

  return count;
};

template <class T> void List<T>::rotate(char c, int k) {
  if (c != 'R' && c != 'L')
    return;

  int n = count(), steps = 0, travelSteps = (c == 'R') ? n - (k % n) : (k % n);

  last->link = first;
  while (first && steps < travelSteps) {
    last = first;
    first = first->link;
    steps++;
  }

  last->link = nullptr;
};

template <class T> List<T> List<T>::subList(int start, int end) {
  int n = count(), i = 0;
  List<T> sList;
  ListNode<T> *p;

  if (start >= 0 && start <= end && end < n) {
    for (p = first; p && i <= end; p = p->link, i++)
      if (start <= i && i <= end)
        sList.Attach(p->data);
  } else if (start < 0 || start > n || end < 0 || end > n)
    std::cerr << "\nIndex out of range" << std::endl;
  else
    std::cerr << "\nInvalid parameters" << std::endl;

  return sList;
};

// int main (int argc, char *argv[]) {
//   std::string cmd;
//   char v, w;
//   int i, j;
//   List<char>* list = new List<char>();
//   std::cout << "List created!" << std::endl;
//
//   while (std::getline(std::cin, cmd)) {
//     std::stringstream ss(cmd);
//     ss >> cmd;
//
//     if (cmd == "attach") {
//       ss >> cmd;
//       v = cmd[0];
//       list->Attach(v);
//     } else if (cmd == "invert")
//       list->Invert();
//     else if (cmd == "appear") {
//       ss >> cmd;
//       v = cmd[0];
//       ss >> cmd;
//       w = cmd[0];
//
//       std::cout << v << " does";
//       if (!list->appearM(v, w)) std::cout << " not";
//       std::cout << " appear " << w << " time(s)." << std::endl;
//     } else if (cmd == "replace") {
//       ss >> cmd;
//       v = cmd[0];
//       ss >> cmd;
//       w = cmd[0];
//       std::cout << "Made " << list->replaceALL(v, w) << " replacement(s)" <<
//       std::endl;
//     } else if (cmd == "rotate") {
//       ss >> cmd;
//       v = cmd[0];
//       ss >> cmd;
//       i = std::stoi(cmd);
//       list->rotate(v, i);
//     } else if (cmd == "sublist") {
//       ss >> cmd;
//       i = std::stoi(cmd);
//       ss >> cmd;
//       j = std::stoi(cmd);
//       std::cout << "SubList: " << list->subList(i, j) << std::endl;
//     }
//     else if (cmd == "exit")
//       break;
//     std::cout << "List: " << list << "\n" << std::endl;
//   }
//
//   delete list;
//   return 0;
// }
