#include <ctime>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

template <class T> class List;

template <class T> class ListNode {
public:
  ListNode(T d) : data(d), link(nullptr) {};
  friend class List<T>;

private:
  T data;
  ListNode<T> *link;
};

template <class T> class List {
public:
  List() { first = last = 0; }
  void Attach(T k);
  int appearM(T k, int m) const;
  int replaceALL(T a1, T a2);
  void rotate(char direction, int n);
  List<T> subList(int start, int end);

private:
  ListNode<T> *first;
  ListNode<T> *last;

  int appear(ListNode<T> *node, T k) const {
    return (node) ? (node->data == k) + appear(node->link, k) : 0;
  };

  int replaceALL(ListNode<T> *node, T a, T b) {
    if (!node)
      return 0;
    if (node->data == a) {
      node->data = b;
      return replaceALL(node->link, a, b) + 1;
    }

    return replaceALL(node->link, a, b);
  };

  int count(ListNode<T> *node) { return (node) ? count(node->link) + 1 : 0; };

  friend std::ostream &operator<<(std::ostream &out, const List<T> &list) {
    ListNode<T> *tmp = list.first;
    while (tmp) {
      out << tmp->data;
      if (tmp->link)
        out << " ";
      tmp = tmp->link;
    }

    return out;
  };
};

template <class T> void List<T>::Attach(T k) {
  ListNode<T> *newnode = new ListNode<T>(k);
  if (first == 0)
    first = last = newnode;
  else {
    last->link = newnode;
    last = newnode;
  }
};

template <class T> int List<T>::appearM(T k, int m) const {
  return appear(first, k) == m;
};

template <class T> int List<T>::replaceALL(T a, T b) {
  return (a == b) ? 0 : replaceALL(first, a, b);
};

template <class T> void List<T>::rotate(char direction, int k) {
  if (direction != 'L' && direction != 'R') {
    std::cerr << "Invalid rotation direction" << std::endl;
    return;
  }

  std::cout << "Rotating list " << k << " time(s)" << std::endl;

  ListNode<T> *pTrav;
  int n = count(first);
  int travelSteps = (direction == 'R') ? n - (k % n) : k % n;

  last->link = first;

  while (travelSteps--) {
    last = first;
    first = first->link;
  }
  last->link = nullptr;
};

template <class T> List<T> List<T>::subList(int start, int end) {
  int n = count(first), i = 0;
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

// int main() {
//   srand(time(NULL));
//   List<int> list;
//   std::string s;
//   int i, m, result;
//   char direction;
// 
//   for (i = 0; i < 10; i++)
//     list.Attach(rand() % 5);
//   std::cout << list << std::endl;
// 
//   std::cout << "Sublist: " << std::endl;
//   std::getline(std::cin, s);
//   std::stringstream sss(s);
//   sss >> s;
//   i = std::stoi(s);
//   sss >> s;
//   m = std::stoi(s);
//   std::cout << "Sublist: " << list.subList(i, m) << std::endl;
// 
//   std::cout << "Rotate: " << std::endl;
//   std::getline(std::cin, s);
//   std::stringstream ss(s);
//   ss >> s;
//   direction = s[0];
//   ss >> s;
//   m = std::stoi(s);
//   list.rotate(direction, m);
//   std::cout << list << std::endl;
// 
//   std::cout << "ReplaceALL: " << std::endl;
//   std::getline(std::cin, s);
//   std::stringstream sss(s);
//   sss >> s;
//   i = std::stoi(s);
//   sss >> s;
//   m = std::stoi(s);
//   int count = list.replaceALL(i, m);
//   if (count)
//     std::cout << "Made " << count << " replacements " << std::endl;
//   else
//     std::cout << "Made no replacements " << std::endl;
//   std::cout << list << std::endl;
// 
//   return 0;
// };
