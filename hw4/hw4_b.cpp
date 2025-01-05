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

  Node<T> *operator[](int i) const {
    if (i > getSize() || i < getSize())
      return nullptr;

    Node<T> *p = getHead();
    int j = 0;
    while (p) {
      if (i == j)
        return p;
      j++;
    }
    return nullptr;
  };

  int getIndex(const T &d) const {
    Node<T> *p = head;

    int i = 0;
    while (p) {
      if constexpr (std::is_pointer<T>::value)
        if (*(p->getData()) == *d)
          return i;
        else if (p->getData() == d)
          return i;
      i++;
      p = p->getNext();
    }

    return -1;
  };

  // T could be a Vertex or an Edge
  Node<T> *exists(const T &d) {
    Node<T> *p = head;

    while (p) {
      if constexpr (std::is_pointer<T>::value)
        if (*(p->getData()) == *d)
          return p;
        else if (p->getData() == d)
          return p;

      p = p->getNext();
    }

    return nullptr;
  }

  void removeFromHead() {
    if (!head) return;

    Node<T>* tmp = head;
    tail = (tail == head) ? nullptr : tail;
    head = head->getNext();
    delete tmp;
  };

  void removeFromTail() {
    if (!head) return;
    if (head == tail) {
      delete head;
      head = tail = nullptr;
      return;
    };

    Node<T>* tmp = head;

    while (tmp) {
      if (tmp->getNext() == tail) {
        delete tmp->getNext();
        tmp->setNext(nullptr);
        tail = tmp;
        break;
      }
      tmp = tmp->getNext();
    }
  };

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
  Edge(Vertex *u, Vertex *v, int w = 1) : vertices({u, v}), weight(w) {};
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

  void insertEdge(const int &u, const int &v, const int& w = 1) {
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
    // vExists->addConnectedEdge(newEdge);  // Adjacency is directional
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

  void getAdjacentMatrix(int **&adjMatrix) const {
    // Get the number of vertices
    int dim = vertexList->getSize();
    if (dim == 0)
      return;

    // Allocate memory for the adjacency matrix
    adjMatrix = new int *[dim];
    for (int i = 0; i < dim; i++)
      adjMatrix[i] = new int[dim];

    // Initialize the matrix elements to 0
    for (int i = 0; i < dim; i++)
      for (int j = 0; j < dim; j++)
        adjMatrix[i][j] = 0;

    // Populate the adjacency matrix
    Node<Vertex *> *p = vertexList->getHead();
    while (p) {
      int uIndex = vertexList->getIndex(p->getData());
      if (uIndex != -1) {
        // Parse the list of connected edges for the current vertex
        Node<Edge *> *currentEdge =
            p->getData()->getConnectedEdges()->getHead();
        while (currentEdge) {
          Vertex *otherEnd =
              currentEdge->getData()->getAnotherEnd(p->getData());
          if (otherEnd) {
            int vIndex = vertexList->getIndex(otherEnd);
            if (vIndex != -1)
              adjMatrix[uIndex][vIndex] = currentEdge->getData()->getWeight();
          }
          currentEdge = currentEdge->getNext();
        }
      }
      p = p->getNext();
    }
  }

  void minimumCostSpanningTree() const;
private:
  NodeList<Vertex *> *vertexList;
  NodeList<Edge *> *edgeList;
};

void Graph::minimumCostSpanningTree() const {}

int main() {
  int n, m, i, u, v, w;
  Graph g;

  std::cin >> n >> m;

  for (i = 0; i < n; i++) {
    std::cin >> u;
    g.insertVertex(u);
  }

  for (i = 0; i < m; i++) {
    std::cin >> u >> v >> w;
    g.insertEdge(u, v, w);
  }

  std::cout << std::endl;

  // Generate and print the adjacency matrix
  int **adjMatrix = nullptr;
  g.getAdjacentMatrix(adjMatrix);

  for (u = 0; u < n; u++) {
    for (v = 0; v < n; v++, std::cout << " ")
      std::cout << adjMatrix[u][v];
    std::cout << std::endl;
  }

  // Clean up
  for (u = 0; u < n; u++)
    delete[] adjMatrix[u];
  delete[] adjMatrix;

  g.minimumCostSpanningTree();

  return 0;
}

