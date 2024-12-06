#include <iostream>

#define DEFAULT_SIZE 10

template <class T> class Stack;
template <class T>
class Node {
  Node<T>* next;
  T data;
public:
  Node() : next(nullptr) {};
  Node(const T& d) : next(nullptr), data(d) {};
  Node(const T& d, Node<T>* next) : next(next), data(d) {};

  friend class Stack<T>;

  friend std::ostream& operator<<(std::ostream& out, const Node<T>* n) {
    if (n) out << n-> data;
    return out;
  };
};

template <class T> 
class Stack {
private:
  int MaxStackSize;
  Node<T>* top;
public:
  Stack(int MaxStackSize=DEFAULT_SIZE) : MaxStackSize(MaxStackSize), top(nullptr) {};
  ~Stack() {
    deleteNode(top);
  };

  void deleteNode(Node<T>* n) {
    if (!n) return;

    deleteNode(n->next);
    deleteNode(n);
  };

  int count(Node<T>* n) const {
    if (!n) return 0;

    return count(n->next) + 1;
  };

  int count() const {
    return count(top);
  };

  bool IsFull() { return count() == MaxStackSize; };

  void Add(const T& item) {
    if (IsFull()) {
      std::cerr << "Stack is full" << std::endl;
      exit(1);
    }

    Node<T>* newNode = new Node<T>(item, top);
    top = newNode;
  };

  bool IsEmpty() { return top == nullptr; };

  T* Delete(T&) {
    if (IsEmpty()) {
      std::cerr << "Stack is empty" << std::endl;
      exit(1);
    }

    T* item = new T(top->data);
    Node<T>* tmp = top->next;
    delete top;
    top = tmp;
    return item;
  };

  void moveToTop(const T& x) {
    Node<T>* tmp = top;
    Node<T>* prev = nullptr;

    while (tmp && tmp->data != x) {
      prev = tmp;
      tmp = tmp->next;
    }

    if (tmp && tmp->data == x) {
      // If the element is found
      prev->next = tmp->next;
      tmp->next = top;
      top = tmp;
    }
  };

  friend std::ostream& operator<<(std::ostream& out, const Stack<T>& s) {
    Node<T>* tmp = s.top;
    while (tmp) {
      out << tmp;
      if (tmp->next) out << " ";
      tmp = tmp->next;
    }

    return out;
  };
};

int main() {
  Stack<int> s;
  s.Add(1);
  s.Add(2);
  s.Add(3);
  s.Add(4);
  s.Add(5);

  std::cout << s << std::endl;
  s.moveToTop(3);

  std::cout << s << std::endl;

  while (!s.IsEmpty()) {
    int* item = s.Delete(*item);
    std::cout << *item << std::endl;
  }

  return 0;
}
