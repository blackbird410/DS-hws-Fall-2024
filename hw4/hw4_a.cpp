#include <array>
#include <fstream>
#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

template <class T> class Node {
public:
  Node() : next(nullptr) {};
  Node(const T &d) : data(d), next(nullptr) {};
  ~Node() {};

  bool operator==(const Node<T> *other) const {
    return (other && getData() == other->getData());
  };

  T getData() const { return data; };
  void setData(const T &newData) { data = newData; };

  Node<T> *getNext() const { return next; };
  void setNext(Node<T> *newLink) { next = newLink; };

  friend std::ostream &operator<<(std::ostream &out, const Node *n) {
    if (n)
      out << n->getData();
    return out;
  };

private:
  T data;
  Node<T> *next;
};

template <class T> class NodeList {
public:
  NodeList() : head(nullptr), tail(nullptr), count(0) {};
  ~NodeList() { deleteNode(head); };

  // T could be a Vertex or an Edge
  Node<T> *exists(const T &d) {
    Node<T> *p = head;

    while (p) {
      if constexpr (std::is_pointer<T>::value) {
        if (*(p->getData()) == *d)
          return p;
      } else {
        if (p->getData() == d)
          return p;
      }
      p = p->getNext();
    }

    return nullptr;
  }

  void insert(const T &data) {
    if (exists(data))
      return;

    Node<T> *newNode = new Node<T>(data);
    if (!head)
      head = tail = newNode;
    else {
      tail->setNext(newNode);
      tail = newNode;
    }
    count++;
  };

  int getSize() const { return count; };
  Node<T> *getHead() const { return head; };

  friend std::ostream &operator<<(std::ostream &out, const NodeList *l) {
    if (l) {
      Node<T> *p = l->getHead();
      while (p) {
        out << p;
        if (p->getNext())
          out << " ";
        p = p->getNext();
      }
    }

    return out;
  };

private:
  Node<T> *head;
  Node<T> *tail;
  int count;

  void deleteNode(Node<T> *node) {
    if (!node)
      return;
    deleteNode(node->getNext());
    delete node;
  };
};

class Vertex;
class Edge;

class Vertex {
public:
  Vertex(int d) : data(d), connectedEdges(new NodeList<Edge *>()) {};
  ~Vertex() { delete connectedEdges; };
  bool operator==(const Vertex *other) const {
    return (other && data == other->data);
  };

  bool operator==(const Vertex &other) const { return (data == other.data); };

  bool operator!=(const Vertex &other) const { return !(*this == other); };

  int getData() const { return data; };
  void setData(const int &d) { data = d; };

  void addConnectedEdge(Edge *e) { connectedEdges->insert(e); };
  const NodeList<Edge *> *getConnectedEdges() const { return connectedEdges; };

  friend std::ostream &operator<<(std::ostream &out, const Vertex *v) {
    if (v)
      out << v->getData();
    return out;
  };

private:
  int data;
  NodeList<Edge *> *connectedEdges;
};

class Edge {
public:
  Edge(Vertex *u, Vertex *v, int w = 1) : weight(w), vertices({u, v}) {};
  ~Edge() {};
  bool operator==(const Edge *other) const {
    return (getWeight() == other->getWeight() &&
            ((vertices[0] == other->vertices[0] &&
              vertices[1] == other->vertices[1]) ||
             (vertices[0] == other->vertices[1] &&
              vertices[1] == other->vertices[0])));
  };

  bool operator==(const Edge &other) const {
    return (getWeight() == other.getWeight() &&
            ((vertices[0] == other.vertices[0] &&
              vertices[1] == other.vertices[1]) ||
             (vertices[0] == other.vertices[1] &&
              vertices[1] == other.vertices[0])));
  };

  bool operator!=(const Edge &other) const { return !(*this == other); };

  Vertex *getAnotherEnd(const Vertex *v) const {
    // Return the second edge only when trying to access the vertices because of
    // directed edge property
    return (v == vertices[0]) ? vertices[1] : nullptr;
  };

  int getWeight() const { return weight; };

  friend std::ostream &operator<<(std::ostream &out, const Edge *e) {
    if (e)
      out << "(" << e->vertices[0] << ", " << e->vertices[1] << ", "
          << e->getWeight() << ")";
    return out;
  };

private:
  int weight;
  std::array<Vertex *, 2> vertices;
};

class Graph {
public:
  Graph()
      : vertexList(new NodeList<Vertex *>()),
        edgeList(new NodeList<Edge *>()) {};

  ~Graph() {
    Node<Vertex *> *v = vertexList->getHead();
    while (v) {
      delete v->getData();
      v = v->getNext();
    }
    Node<Edge *> *e = edgeList->getHead();
    while (e) {
      delete e->getData();
      e = e->getNext();
    }
    delete vertexList;
    delete edgeList;
  }

  void insertVertex(int &d) {
    Vertex *v = new Vertex(d);
    if (!vertexList->exists(v)) {
      vertexList->insert(v);
    } else
      delete v;
  };

  void insertEdge(const int &u, const int &v, int w = 1) {
    Vertex v1(u), v2(v);
    Vertex *uExists = vertexList->exists(&v1)->getData();
    Vertex *vExists = vertexList->exists(&v2)->getData();

    if (!vExists || !uExists) {
      std::cerr << "Vertex not found for inserting new edge" << std::endl;
      return;
    };

    Edge *newEdge = new Edge(uExists, vExists, w);
    edgeList->insert(newEdge);
    uExists->addConnectedEdge(newEdge);
    // vExists->addConnectedEdge(newEdge);
  };

  void printAdjacentList() const {
    Node<Vertex *> *v = vertexList->getHead();
    Node<Edge *> *e = nullptr;
    Vertex *otherEnd = nullptr;

    while (v) {
      std::cout << v << ":";

      e = v->getData()->getConnectedEdges()->getHead();
      while (e) {
        otherEnd = e->getData()->getAnotherEnd(v->getData());
        if (otherEnd)
          std::cout << " " << otherEnd;
        e = e->getNext();
      }
      std::cout << std::endl;

      v = v->getNext();
    }
  };

  void BFS(int start = 0) const {
    // std::cout << "Vertices: " << vertexList << std::endl;
    // std::cout << "Edges: " << edgeList << std::endl;

    Vertex *v = new Vertex(start);
    Node<Vertex *> *startVertex = vertexList->exists(v);
    if (!startVertex) {
      std::cout << "Invalid vertex for BFS" << std::endl;
      delete v;
      return;
    }
    delete v;

    std::unordered_map<Vertex *, bool> visited;
    std::queue<Node<Vertex *> *> q, copy;
    Node<Vertex *> *currentVertex;
    Node<Edge *> *currentEdge;
    Vertex *otherEnd;

    q.push(startVertex);

    while (!q.empty()) {
      currentVertex = q.front();
      q.pop();

      std::cout << currentVertex << std::endl;
      visited[currentVertex->getData()] = true;

      currentEdge = currentVertex->getData()->getConnectedEdges()->getHead();
      while (currentEdge) {
        otherEnd =
            currentEdge->getData()->getAnotherEnd(currentVertex->getData());

        if (otherEnd && !visited[otherEnd]) {
          // Check if not enqueued
          copy = q;
          while (!copy.empty()) {
            if (copy.front()->getData() == otherEnd)
              break;
            copy.pop();
          }

          if (copy.empty())
            q.push(vertexList->exists(otherEnd));
        }

        currentEdge = currentEdge->getNext();
      }
    }
  };

  void DFS(int start = 0) const {
    Vertex *v = new Vertex(start);
    Node<Vertex *> *startVertex = vertexList->exists(v);
    if (!startVertex) {
      std::cout << "Invalid vertex for DFS" << std::endl;
      delete v;
      return;
    }
    delete v;

    std::stack<Node<Vertex *> *> s, copy;
    std::unordered_map<Vertex *, bool> visited;
    Node<Edge *> *currentEdge;
    Vertex *otherEnd;
    bool depthReached = false;

    Node<Vertex *> *currentVertex = vertexList->getHead();
    while (currentVertex) {
      visited[currentVertex->getData()] = false;
      currentVertex = currentVertex->getNext();
    }

    s.push(startVertex);

    while (!s.empty()) {
      currentVertex = s.top();

      // Check if there are any unvisited children
      currentEdge = currentVertex->getData()->getConnectedEdges()->getHead();
      depthReached = true;
      while (currentEdge) {
        otherEnd =
            currentEdge->getData()->getAnotherEnd(currentVertex->getData());
        if (otherEnd && !visited[otherEnd]) {

          // Check if not already added to the stack
          copy = s;
          while (!copy.empty()) {
            if (copy.top()->getData() == otherEnd)
              break;
            copy.pop();
          }

          if (copy.empty()) {
            s.push(vertexList->exists(otherEnd));
            depthReached = false;
          }
        }

        currentEdge = currentEdge->getNext();
      }

      if (depthReached) {
        std::cout << currentVertex << std::endl;
        s.pop();
        visited[currentVertex->getData()] = true;
      }
    }
  };

private:
  NodeList<Vertex *> *vertexList;
  NodeList<Edge *> *edgeList;
};

void test(Graph *g);

int main() {
  int n, m, i, u, v;
  Graph g;
  // test(&g);

  std::cin >> n >> m;

  for (i = 0; i < n; i++) {
    std::cin >> u;
    g.insertVertex(u);
  }

  for (i = 0; i < m; i++) {
    std::cin >> u >> v;
    g.insertEdge(u, v);
  }

  g.BFS();
  std::cout << std::endl;
  g.DFS();
  // std::cout << std::endl;
  // g.printAdjacentList();

  return 0;
};

void test(Graph *g) {
  std::string filename = "test_sample.txt";
  std::cout << "Filename: ";
  std::getline(std::cin, filename);
  std::ifstream inFile(filename);

  if (!inFile) {
    std::cerr << "Input file could not be opened" << std::endl;
    exit(-1);
  }

  int n, m, i, u, v;
  inFile >> n >> m;

  for (i = 0; i < n; i++) {
    inFile >> u;
    g->insertVertex(u);
  }

  for (i = 0; i < m; i++) {
    inFile >> u >> v;
    g->insertEdge(u, v);
  }

  g->printAdjacentList();
  std::cout << "Start vertex for BFS and DFS: ";
  std::cin >> u;
  g->BFS(u);
  std::cout << std::endl;
  g->DFS(u);
};
