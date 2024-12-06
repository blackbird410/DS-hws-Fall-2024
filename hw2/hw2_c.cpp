#include <bits/stdc++.h>
#include <iostream>
#include <string>

#define DEFAULT_SIZE 10
#define EMPTY -1

template <class T> class StackQueue {
  int front;
  int rear;
  T *queue;
  int MaxSize;

public:
  StackQueue(int MaxStackQueueSize = DEFAULT_SIZE)
      : front(0), rear(0), MaxSize(MaxStackQueueSize > 0 ? MaxStackQueueSize + 1
                                                         : DEFAULT_SIZE + 1),
        queue(new T[MaxSize]) {};
  ~StackQueue() { delete[] queue; };

  inline bool IsFull() const { return (rear + 1) % MaxSize == front; };
  inline bool IsEmpty() const { return front == rear; };
  void StackQueueEmpty() const {
    std::cerr << "StackQueue is empty" << std::endl;
  };
  void StackQueueFull() const {
    std::cerr << "StackQueue is full" << std::endl;
  };

  void push(const T &x) {
    if (IsFull()) {
      StackQueueFull();
      return;
    }

    addRear(x);
    moveToFront(x);
  };

  T *pop(T &x) {
    if (IsEmpty()) {
      StackQueueEmpty();
      return nullptr;
    }

    return deleteFront(x);
  };

  void addRear(const T &x) {
    if (IsFull()) {
      StackQueueFull();
      return;
    }

    queue[rear] = x;
    rear = (rear + 1) % MaxSize;
  };

  T *deleteFront(T &x) {
    if (IsEmpty()) {
      StackQueueEmpty();
      return 0;
    }

    x = queue[front];
    front = (front + 1) % MaxSize;
    return &x;
  };

  void moveToFront(const T &x) {
    if (IsEmpty()) {
      StackQueueEmpty();
      return;
    }

    int i = front, prev;
    bool found = false;
    while (i != rear) {
      if (queue[i] == x) {
        found = true;
        break;
      }
      i = (i + 1) % MaxSize;
    }

    if (!found) {
      std::cerr << x << " not found in the queue" << std::endl;
      return;
    }

    while (i != front) {
      prev = (i - 1 + MaxSize) % MaxSize;
      queue[i] = queue[prev];
      i = prev;
    }
    queue[front] = x;
  }

  void moveToRear(const T &x) {
    if (IsEmpty()) {
      StackQueueEmpty();
      return;
    }

    int i, j;
    bool found = false;
    for (i = front; i != rear; i = (i + 1) % MaxSize) {
      if (queue[i] == x)
        found = true;
      if (found)
        queue[i] = queue[(i + 1) % MaxSize];
    }

    if (found)
      queue[(i - 1 + MaxSize) % MaxSize] = x;
    else
      std::cerr << x << " not found in the queue" << std::endl;
  };

  friend std::ostream &operator<<(std::ostream &out, const StackQueue<T> &q) {
    // std::cout << "Front: " << q.front << " | Rear: " << q.rear << std::endl;
    if (!q.IsEmpty()) {
      int index = q.front;
      while (index != q.rear) {
        out << q.queue[index] << " ";
        index = (index + 1) % q.MaxSize;
      }
    }
    return out;
  }
};

// void test();
// 
// int main() {
//   test();
// 
//   return 0;
// }
// 
// void test() {
//   int n, tmp;
//   std::string cmd;
// 
//   std::cout << "Queue Size: ";
//   std::cin >> n;
//   StackQueue<int> q(n);
//   // for (int i = 0; i < n; i++)
//   //   q.addRear(i);
//   // std::cout << q << std::endl;
//   std::cin.ignore();
// 
//   while (std::getline(std::cin, cmd)) {
//     std::stringstream ss(cmd);
//     ss >> cmd;
//     if (cmd == "enqueue") {
//       ss >> cmd;
//       tmp = std::stoi(cmd);
//       q.addRear(tmp);
//       std::cout << q << std::endl;
//     } else if (cmd == "print")
//       std::cout << q << std::endl;
//     else if (cmd == "dequeue") {
//       q.deleteFront(tmp);
//       std::cout << q << std::endl;
//     } else if (cmd == "moveFront") {
//       ss >> cmd;
//       std::cout << cmd << std::endl;
//       tmp = std::stoi(cmd);
//       q.moveToFront(tmp);
//       std::cout << q << std::endl;
//     } else if (cmd == "moveRear") {
//       ss >> cmd;
//       std::cout << cmd << std::endl;
//       tmp = std::stoi(cmd);
//       q.moveToRear(tmp);
//       std::cout << q << std::endl;
//     } else if (cmd == "push") {
//       ss >> cmd;
//       tmp = std::stoi(cmd);
//       q.push(tmp);
//       std::cout << q << std::endl;
//     } else if (cmd == "pop") {
//       q.pop(tmp);
//       std::cout << q << std::endl;
//     } else if (cmd == "exit")
//       return;
//     else
//       continue;
//   }
// };
