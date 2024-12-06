#include <bits/stdc++.h>
#include <iostream>
#include <string>

#define DEFAULT_SIZE 10
#define EMPTY -1

template <class T> class Stack {
private:
  int MaxStackSize;
  int top;
  T *stack;

public:
  Stack(int MaxStackSize = DEFAULT_SIZE)
      : MaxStackSize(MaxStackSize > 0 ? MaxStackSize : DEFAULT_SIZE),
        top(EMPTY), stack(new T[MaxStackSize]) {};
  ~Stack() { delete[] stack; };

  inline bool IsFull() const { return top == MaxStackSize - 1; };
  inline bool IsEmpty() const { return top == EMPTY; };
  void StackEmpty() const { std::cerr << "Stack is empty" << std::endl; };
  void StackFull() { std::cerr << "Stack is full" << std::endl; };

  void Add(const T &item) {
    if (IsFull()) {
      StackFull();
      exit(1);
    }

    stack[++top] = item;
  };

  T *Delete(T &x) {
    if (IsEmpty()) {
      StackEmpty();
      exit(1);
    }

    x = stack[top--];
    return &x;
  };

  T &getTop() const { return stack[top]; };

  Stack<T> &operator=(const Stack<T> &other) {
    if (this != &other) {
      delete[] stack;

      MaxStackSize = other.MaxStackSize;
      top = other.top;
      stack = new T[MaxStackSize];
      for (int i = 0; i <= top; ++i)
        stack[i] = other.stack[i];
    }

    return *this;
  };

  void moveToTop(const T &x) {
    if (IsEmpty()) {
      StackEmpty();
      exit(1);
    } else if (!top)
      return;

    Stack<T> *newStack = new Stack<T>(MaxStackSize);
    bool found = false;

    for (int i = 0; i <= top; ++i) {
      if (stack[i] == x)
        found = true;
      else
        newStack->Add(stack[i]);
    };

    if (found) {
      newStack->Add(x);
      *this = *newStack;
    }

    delete newStack;
  };

  friend std::ostream &operator<<(std::ostream &out, const Stack<T> &s) {
    if (!s.IsEmpty()) {
      int i = s.top;
      while (i >= 0) {
        out << s.stack[i--];
        if (i >= 0)
          out << " ";
      }
    }

    return out;
  };
};

template <class T> class Queue {
  int front;
  int rear;
  T *queue;
  int MaxSize;

public:
  Queue(int MaxQueueSize = DEFAULT_SIZE)
      : front(0), rear(0),
        MaxSize(MaxQueueSize > 0 ? MaxQueueSize + 1 : DEFAULT_SIZE + 1),
        queue(new T[MaxSize]) {};
  ~Queue() { delete[] queue; };

  inline bool IsFull() const { return (rear + 1) % MaxSize == front; };
  inline bool IsEmpty() const { return front == rear; };
  void QueueEmpty() const { std::cerr << "Queue is empty" << std::endl; };
  void QueueFull() const { std::cerr << "Queue is full" << std::endl; };

  void Add(const T &x) {
    if (IsFull()) {
      QueueFull();
      return;
    }

    queue[rear] = x;
    rear = (rear + 1) % MaxSize;
  };

  T *Delete(T &x) {
    if (IsEmpty()) {
      QueueEmpty();
      return 0;
    }

    x = queue[front];
    front = (front + 1) % MaxSize;
    return &x;
  };

  void moveToFront(const T &x) {
    if (IsEmpty()) {
      QueueEmpty();
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
      QueueEmpty();
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

  friend std::ostream &operator<<(std::ostream &out, const Queue<T> &q) {
    if (!q.IsEmpty()) {
      int index = q.front;
      // std::cout << "Front: " << q.front << " | Rear: " << q.rear <<
      // std::endl;
      while (index != q.rear) {
        out << q.queue[index] << " ";
        index = (index + 1) % q.MaxSize;
      }
    }
    return out;
  }
};
