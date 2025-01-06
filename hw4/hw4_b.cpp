#include <algorithm>
#include <array>
#include <climits>
#include <fstream>
#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

inline void print(std::string s) { std::cout << s << std::endl; }

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
  NodeList(const NodeList<T> *other) {
    Node<T> *p = other->getHead();
    while (p) {
      this->insert(p->getData());
      p = p->getNext();
    }
  };
  ~NodeList() { deleteNode(head); };

  Node<T> *operator[](int i) const {
    if (i >= getSize() || i < 0)
      return nullptr;

    Node<T> *p = getHead();
    int j = 0;
    while (p) {
      if (i == j)
        return p;
      j++;
      p = p->getNext();
    }
    return nullptr;
  };

  int getIndex(const T &d) const {
    Node<T> *p = head;

    int i = 0;
    while (p) {
      if constexpr (std::is_pointer<T>::value) {
        if (*(p->getData()) == *d)
          return i;
      } else if (p->getData() == d)
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
      if constexpr (std::is_pointer<T>::value) {
        if (*(p->getData()) == *d)
          return p;
      } else if (p->getData() == d)
        return p;

      p = p->getNext();
    }

    return nullptr;
  }

  void removeFromHead() {
    if (!head)
      return;

    Node<T> *tmp = head;
    tail = (tail == head) ? nullptr : tail;
    head = head->getNext();
    delete tmp;
  };

  void removeFromTail() {
    if (!head)
      return;
    if (head == tail) {
      delete head;
      head = tail = nullptr;
      return;
    };

    Node<T> *tmp = head;

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

  void addFromHead(const T &data) {
    Node<T> *newNode = new Node<T>(data);
    newNode->setNext(head);
    tail = (!tail) ? newNode : tail;
    head = newNode;
  };

  void addFromTail(const T &data) { insert(data); };
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
    return other && data == other->getData();
  };
  bool operator<(const Vertex *other) const {
    return other && data < other->getData();
  };

  bool operator==(const Vertex &other) const { return (data == other.data); };
  bool operator!=(const Vertex &other) const { return !(*this == other); };
  bool operator<(const Vertex &other) const { return data < other.getData(); };

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

  bool operator<(const Edge &other) const {
    return (*(vertices[0]) < *(other.vertices[0]) ||
            (*(vertices[0]) == *(other.vertices[0]) &&
             *(vertices[1]) < *(other.vertices[1])) ||
            (*(vertices[0]) == *(other.vertices[0]) &&
             *(vertices[1]) == *(other.vertices[1]) &&
             weight < other.getWeight()));
  };

  bool operator!=(const Edge &other) const { return !(*this == other); };

  Vertex *operator[](const int &i) const {
    return (i != 0 && i != 1) ? nullptr : vertices[i];
  };

  Vertex *getAnotherEnd(const Vertex *v) const {
    return (v == vertices[0]) ? vertices[1] : vertices[0];
  };

  int getWeight() const { return weight; };

  friend std::ostream &operator<<(std::ostream &out, const Edge *e) {
    if (e)
      out << e->vertices[0] << " " << e->vertices[1];
    return out;
  };

  friend std::ostream &operator<<(std::ostream &out, const Edge &e) {
    out << e.vertices[0] << " " << e.vertices[1];
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
  Graph(const Graph &g)
      : vertexList(new NodeList<Vertex *>()), edgeList(new NodeList<Edge *>()) {
    Node<Vertex *> *currentVertex = g.getVertexList()->getHead();
    while (currentVertex) {
      insertVertex(currentVertex->getData()->getData());
      currentVertex = currentVertex->getNext();
    }

    Node<Edge *> *currentEdge = g.getEdgeList()->getHead();
    while (currentEdge) {
      insertEdge((*currentEdge->getData())[0]->getData(),
                 (*currentEdge->getData())[1]->getData(),
                 currentEdge->getData()->getWeight());
      currentEdge = currentEdge->getNext();
    }
  };

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

  void insertVertex(int d) {
    Vertex *v = new Vertex(d);
    if (!vertexList->exists(v)) {
      vertexList->insert(v);
    } else
      delete v;
  };

  void insertEdge(const int &u, const int &v, const int &w = 1) {
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
    vExists->addConnectedEdge(newEdge);  // Remove for directional adjacency
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

  void printAdjacentMatrix(int ***adjMat = nullptr) const {
    int n = vertexList->getSize(), u, v;
    int **adjMatrix = (adjMat) ? *adjMat : nullptr;
    bool freeMemory = false;
    print("Adjacent matrix: ");

    if (!adjMatrix) {
      getAdjacentMatrix(adjMatrix);
      freeMemory = true;
    }

    std::cout << "\t| ";
    for (u = 0; u < n; u++, std::cout << "\t")
      std::cout << (*vertexList)[u];
    std::cout << std::endl;
    std::cout << "\t|";
    for (u = 0; u < 7 * (n + 1); u++, std::cout << "_")
      ;
    std::cout << std::endl;

    for (u = 0; u < n; u++) {
      std::cout << (*vertexList)[u] << "\t| ";
      for (v = 0; v < n; v++, std::cout << "\t") {
        if (adjMatrix[u][v] != INT_MAX)
          std::cout << adjMatrix[u][v];
        else
          std::cout << "-";
      }
      std::cout << std::endl;
    }

    // Clean up
    if (freeMemory) {
      for (u = 0; u < n; u++)
        delete[] adjMatrix[u];
      delete[] adjMatrix;
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

    // Initialize the matrix elements to highest possible value
    for (int i = 0; i < dim; i++)
      for (int j = 0; j < dim; j++)
        adjMatrix[i][j] = INT_MAX;

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

  Edge *getEdge(const int &u, const int &v, const int &w = INT_MAX) const {
    Node<Edge *> *e = edgeList->getHead();
    while (e) {
      if (((*e->getData())[0]->getData() == u &&
           (*e->getData())[1]->getData() == v) ||
          ((*e->getData())[1]->getData() == u &&
           (*e->getData())[0]->getData() == v)) {
        if (w == INT_MAX || e->getData()->getWeight() == w)
          return e->getData();
      }
      e = e->getNext();
    }
    return nullptr;
  }

  NodeList<Vertex *> *getVertexList() const { return vertexList; };
  NodeList<Edge *> *getEdgeList() const { return edgeList; };

  void minimumCostSpanningTree() const;

private:
  NodeList<Vertex *> *vertexList;
  NodeList<Edge *> *edgeList;

private:
  bool hasCycle() const {
    std::unordered_map<Vertex *, bool> visited;

    // Initialize visited map for all vertices
    Node<Vertex *> *curVertex = vertexList->getHead();
    while (curVertex) {
      visited[curVertex->getData()] = false;
      curVertex = curVertex->getNext();
    }

    // Perform DFS to detect cycles
    curVertex = vertexList->getHead();
    while (curVertex) {
      if (!visited[curVertex->getData()])
        if (dfsCheckCycle(curVertex->getData(), nullptr, visited))
          return true;
      curVertex = curVertex->getNext();
    }

    return false;
  }

  bool dfsCheckCycle(Vertex *vertex, Vertex *parent,
                   std::unordered_map<Vertex *, bool> &visited) const {
    visited[vertex] = true;

    Node<Edge *> *curEdge = vertex->getConnectedEdges()->getHead();
    while (curEdge) {
        Vertex *neighbor = curEdge->getData()->getAnotherEnd(vertex);

        // Don't count the parent node as part of the cycle
        if (!visited[neighbor]) {
            if (dfsCheckCycle(neighbor, vertex, visited))
                return true;
        } else if (parent && neighbor != parent) {
            return true;
        }

        curEdge = curEdge->getNext();
    }

    return false;
}


};

void Graph::minimumCostSpanningTree() const {
  // Using Kruskal's algorithm and adjacent matrix
  Graph minSpanTree;
  Graph *tmpSpanTree;
  Node<Edge *> *currentEdge;

  int **adjMatrix = nullptr;
  getAdjacentMatrix(adjMatrix);

  Edge *minEdge = nullptr;
  int uIndex, vIndex, i, j, minWeight, nVertices = vertexList->getSize();
  while (true) {
    // Find the min weight in the adjacent matrix and the corresponding edge
    minWeight = INT_MAX;
    uIndex = vIndex = -1;
    for (i = 0; i < nVertices; i++) {
      for (j = 0; j < nVertices; j++) {
        if (adjMatrix[i][j] < minWeight) {
          minWeight = adjMatrix[i][j];
          uIndex = i;
          vIndex = j;
        }
      }
    }

    if (minWeight == INT_MAX)
      break;

    minEdge = getEdge((*vertexList)[uIndex]->getData()->getData(),
                      (*vertexList)[vIndex]->getData()->getData(), minWeight);

    if (!minEdge)
      break;

    // Save the current spanning tree, and check if a cycle is generated when
    // adding the new edge
    tmpSpanTree = new Graph(minSpanTree);

    tmpSpanTree->insertVertex((*minEdge)[0]->getData());
    tmpSpanTree->insertVertex((*minEdge)[1]->getData());
    tmpSpanTree->insertEdge((*minEdge)[0]->getData(), (*minEdge)[1]->getData(),
                            minEdge->getWeight());

    if (!tmpSpanTree->hasCycle()) {
      minSpanTree.insertVertex((*minEdge)[0]->getData());
      minSpanTree.insertVertex((*minEdge)[1]->getData());
      minSpanTree.insertEdge((*minEdge)[0]->getData(), (*minEdge)[1]->getData(),
                             minEdge->getWeight());
    }
    delete tmpSpanTree;

    adjMatrix[uIndex][vIndex] = INT_MAX;
  }

  // Display the result spanning tree in sorted order according to the first and
  // second vertex
  std::vector<Edge *> edges;
  currentEdge = minSpanTree.getEdgeList()->getHead();
  while (currentEdge) {
    edges.push_back(currentEdge->getData());
    currentEdge = currentEdge->getNext();
  }

  std::sort(edges.begin(), edges.end(),
            [](Edge *a, Edge *b) { return *a < *b; });
  for (auto &e : edges)
    std::cout << e << std::endl;

}

void test(Graph *g);

int main() {
  Graph g;

  test(&g);

  // int n, m, i, u, v, w;
  // std::cin >> n >> m;

  // for (i = 0; i < n; i++) {
  //   std::cin >> u;
  //   g.insertVertex(u);
  // }

  // for (i = 0; i < m; i++) {
  //   std::cin >> u >> v >> w;
  //   g.insertEdge(u, v, w);
  // }

  // // g.printAdjacentMatrix();
  // g.minimumCostSpanningTree();

  // return 0;
}

void test(Graph *g) {
  int n, m, i, u, v, w;
  std::string filename = "test_b.txt";
  std::cout << "Test filename: ";
  std::getline(std::cin, filename);
  std::ifstream inFile(filename);
  if (!inFile) {
    std::cout << "File could not be opened" << std::endl;
    exit(-1);
  }

  inFile >> n >> m;

  for (i = 0; i < n; i++) {
    inFile >> u;
    g->insertVertex(u);
  }

  for (i = 0; i < m; i++) {
    inFile >> u >> v >> w;
    g->insertEdge(u, v, w);
  }
  std::cout << std::endl;

  g->minimumCostSpanningTree();
};
