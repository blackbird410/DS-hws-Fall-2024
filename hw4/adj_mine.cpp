#include <iostream>
#include <array>
#include <fstream>
#include <string>
#include <queue>
#include <unordered_map>
#include <vector>
#include <memory>

template <class T> class NodeList;
template <class T> class Node {
public:
  Node() : next(nullptr) {};
  Node(const T &d, Node<T>* n = nullptr) : data(d), next(n) {};
  ~Node() = default;

  T getData() const { return data; };
  void setData(const T &d) { data = d; }
  Node* getNext() const { return next; };
  void setNext(Node<T>* n) { next = n; };

  friend class NodeList<T>;
  friend std::ostream &operator<<(std::ostream &out, const Node<T>* n) {
    if (n)
      out << n->data;
    return out;
  };

private:
  T data;
  Node<T> *next;
};

template <class T> class NodeList {
public:
  NodeList() : head(nullptr), tail(nullptr) {};
  ~NodeList() { deleteNode(head); };

  Node<T> *getHead() const { return head; };

  Node<T>* getNode(T* data) const {
    Node<T>* p = head;
    while (p) {
      if (*(p->getData()) == *(*data)) break;
      p = p->getNext();
    }
    return p;
  };

  bool exists(const T& data) const {
    Node<T> *p = head;
    while (p) {
      if (p->data == data)
        return true;
      p = p->getNext();
    }
    return false;
  };

  bool exists(const Node<T> *n) const {
    Node<T> *p = head;
    while (p) {
      if (p->getData() == n->getData())
        return true;
      p = p->getNext();;
    }
    return false;
  };

  void insert(const T &data) {
    if (!data) return;

    if (!head) {
      head = tail= new Node<T>(data);
    } else {
      if (exists(data)) return;
      tail->setNext(new Node<T>(data));
      tail = tail->getNext();
    }
  };

  friend std::ostream &operator<<(std::ostream &out, const NodeList<T> *l) {
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
  }

  friend std::ostream &operator<<(std::ostream &out, const std::unique_ptr<NodeList<T>>& l) {
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
  }
private:
  Node<T> *head;
  Node<T> *tail;

  void deleteNode(Node<T> *n) {
    if (!n) return;
    deleteNode(n->getNext());
    std::cout << "Deleting " << n << std::endl;
    delete n;
  }
};

class Edge;
class Graph;
class Vertex {
public:
  Vertex() : data(0) {};
  Vertex(const int& d) : data(d) {};
  ~Vertex() { 
    edgeList.clear();
  };
  bool operator==(const Vertex& other) const { return data == other.data; };
  bool operator==(const Vertex* other) const { return data == other->data; };

  int getData() const { return data; };
  void setData(int v) { data = v; };
  void addEdge(Edge* newEdge) { 
    for (auto& e: edgeList)
      if (e == newEdge) return;
    edgeList.push_back(newEdge);
  };

  friend class Graph;
  friend std::ostream &operator<<(std::ostream &out, const Vertex *v) {
    if (v)
      out << v->getData();
    return out;
  }

private:
  int data;
  std::vector<Edge*> edgeList;
};

class Edge {
public:
  Edge() : vertices({nullptr, nullptr}) {};
  Edge(Vertex* v1, Vertex* v2) : vertices({v1, v2}) {};
  ~Edge() = default;

  bool operator==(const Edge &other) const {
    return ((vertices[0]->getData() == other.vertices[0]->getData() &&
             vertices[1]->getData() == other.vertices[1]->getData()) ||
            (vertices[0]->getData() == other.vertices[1]->getData() &&
             vertices[1]->getData() == other.vertices[0]->getData()));
  };

  Vertex *getAnotherEnd(const Vertex* v) const {
    return (v == vertices[0]) ? vertices[1] : (v == vertices[1]) ? vertices[0] : nullptr;
  };

  friend std::ostream& operator<<(std::ostream& out, const Edge* e) {
    if (e) out << e->vertices[0] << " <-> " << e->vertices[1];
    return out;
  };

private:
  std::array<Vertex*, 2> vertices;
};

class Graph {
public:
  Graph();
  ~Graph();

  void addVertex(const int &d) {
    Vertex* v = new Vertex(d);
    if (vertexList->exists(v)) {
      std::cerr << "Vertex " << d << " already added" << std::endl;
      delete v;
      return;
    }
    vertexList->insert(v);
    nVertices++;
  };

  void addEdge(const int &a, const int &b) {
    Vertex* tmp = new Vertex(a);
    Vertex* v1 = vertexList->getNode(&tmp)->getData();
    tmp->setData(b);
    Vertex* v2 = vertexList->getNode(&tmp)->getData();
    delete tmp;
    if (!v1 || !v2) {
      std::cerr << "Vertex not found" << std::endl;
      return;
    }

    Edge* e = new Edge(v1, v2);
    if (edgeList->exists(e)) {
      delete e;
      return;
    }

    edgeList->insert(e);
    v1->addEdge(e);
    v2->addEdge(e);
    nEdges++;
  };

  std::vector<std::pair<Vertex*, std::unique_ptr<NodeList<Vertex*>>>> adj() const {
    Node<Vertex*>* v = vertexList->getHead();
    Vertex* otherEnd = nullptr;
    Node<Edge*>* e;
    std::vector<std::pair<Vertex*, std::unique_ptr<NodeList<Vertex*>>>> adjMatrix;

    while (v) {
      std::unique_ptr<NodeList<Vertex*>> adjVertices = std::make_unique<NodeList<Vertex *>>();
      e = edgeList->getHead();
      while (e) {
        otherEnd = e->getData()->getAnotherEnd(v->getData());
        adjVertices->insert(otherEnd);
        e = e->getNext();
      }
      adjMatrix.push_back({v->getData(), std::move(adjVertices)});
      v = v->getNext();
    }

    return adjMatrix;
  };

  void BFS() const {
    // Starts the BFS with the first inserted vertex by default
    std::unordered_map<Vertex*, bool> visited;
    std::queue<Vertex*> q, tmpQ;
    Vertex* start =  vertexList->getHead()->getData();
    Vertex* currentVertex = nullptr, *otherEnd = nullptr;
    Node<Edge* >* currentEdge = nullptr;
    Node<Vertex* >* tmp = vertexList->getHead();
    bool isEnqueued;

    // Initialize the visited list
    while (tmp) {
      visited[tmp->getData()] = false;
      tmp = tmp->getNext();
    }

    q.push(start);
    while (!q.empty()) {
      currentVertex = q.front();
      q.pop();
      visited[currentVertex] = true;
      std::cout << currentVertex << std::endl;

      for (auto& currentEdge: currentVertex->edgeList) {
        otherEnd = currentEdge->getAnotherEnd(currentVertex);

        // Check if already added in the queue
        tmpQ = q;
        isEnqueued = false;
        while (!tmpQ.empty()) {
          if (tmpQ.front() == otherEnd) {
            isEnqueued = true;
            break;
          }
          tmpQ.pop();
        }

        if (!visited[otherEnd] && !isEnqueued)
          q.push(otherEnd);
      }
    }
  };

private:
  int nVertices;
  int nEdges;
  NodeList<Vertex *> *vertexList;
  NodeList<Edge *> *edgeList;
};

Graph::Graph()
    : vertexList(new NodeList<Vertex *>()),
      edgeList(new NodeList<Edge *>()), nVertices(0), nEdges(0) {};
Graph::~Graph() {
  delete edgeList;
  delete vertexList;
}

void test(Graph* g) {
  std::string filename;
  std::cout << "Filename: ";
  std::getline(std::cin, filename);
  std::ifstream inFile(filename);
  if (!inFile) {
    std::cerr << "Input file could not be opened" << std::endl;
    return;
  };

  int i, v1, v2, n, m;
  inFile >> n >> m; 
  for (i = 0; i < n; i++) {
    inFile >> v1;
    g->addVertex(v1);
  }

  for (i = 0; i < m; i++) {
    inFile >> v1 >> v2;
    g->addEdge(v1, v2);
  }
}

int main() {
  Graph g;
  test(&g);
  std::vector<std::pair<Vertex*, std::unique_ptr<NodeList<Vertex*>>>> adjMatrix = g.adj();
  for (auto& e: adjMatrix)
    std::cout << e.first << ": " << e.second << std::endl;
  std::cout << std::endl;
  g.BFS();
  return 0;
};
