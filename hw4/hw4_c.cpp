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

  Vertex *getAnotherEnd(const Vertex *v = nullptr) const {
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
    Node<Vertex *> *curVertex = g.getVertexList()->getHead();
    while (curVertex) {
      insertVertex(curVertex->getData()->getData());
      curVertex = curVertex->getNext();
    }

    Node<Edge *> *curEdge = g.getEdgeList()->getHead();
    while (curEdge) {
      insertEdge((*curEdge->getData())[0]->getData(),
                 (*curEdge->getData())[1]->getData(),
                 curEdge->getData()->getWeight());
      curEdge = curEdge->getNext();
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
    vExists->addConnectedEdge(newEdge); // Adjacency is directional
  };

  void printAdjacentList(std::unordered_map<Vertex *, std::vector<Vertex *>>
                             *adj = nullptr) const {
    std::unordered_map<Vertex *, std::vector<Vertex *>> adjList;
    if (!adj) {
      getAdjacencyList(adjList);
      for (const auto &pair : adjList) {
        std::cout << pair.first << ": ";
        for (const auto &adjVertex : pair.second)
          std::cout << adjVertex << " ";
        std::cout << std::endl;
      }
    } else {
      for (const auto &pair : *adj) {
        std::cout << pair.first << ": ";
        for (const auto &adjVertex : pair.second)
          std::cout << adjVertex << " ";
        std::cout << std::endl;
      }
    }
  };

  void getAdjacencyList(
      std::unordered_map<Vertex *, std::vector<Vertex *>> &adjList) const {
    Node<Vertex *> *v = vertexList->getHead();
    Node<Edge *> *e = nullptr;
    Vertex *otherEnd = nullptr;

    while (v) {
      std::vector<Vertex *> curVertexAdjList;
      e = v->getData()->getConnectedEdges()->getHead();
      while (e) {
        otherEnd = e->getData()->getAnotherEnd(v->getData());
        curVertexAdjList.push_back(otherEnd);
        e = e->getNext();
      }

      adjList[v->getData()] = curVertexAdjList;
      v = v->getNext();
    }
  };

  bool isLinked(const Edge *otherEdge) const {
    if (otherEdge) {
      Node<Edge *> *e = getEdgeList()->getHead();
      while (e) {
        Vertex *first = e->getData()->getAnotherEnd();
        Vertex *second = e->getData()->getAnotherEnd(first);
        if ((*otherEdge)[0]->getData() == first->getData() ||
            (*otherEdge)[0]->getData() == second->getData() ||
            (*otherEdge)[1]->getData() == first->getData() ||
            (*otherEdge)[1]->getData() == second->getData())
          return true;
        e = e->getNext();
      }
    }

    return false;
  };

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
      if (!visited[curVertex->getData()]) {
        if (dfsCheckCycle(curVertex->getData(), nullptr, visited)) {
          return true;
        }
      }
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

      // If the neighbor is not visited, recurse on it
      if (!visited[neighbor]) {
        if (dfsCheckCycle(neighbor, vertex, visited))
          return true;
      }
      // If the neighbor is visited and not the parent, we found a cycle
      else if (neighbor != parent)
        return true;

      curEdge = curEdge->getNext();
    }

    return false;
  }
};

void Graph::minimumCostSpanningTree() const {
  // Using Prim's algorithm and adjacent list
  Graph minSpanTree;
  Graph *tmpSpanTree;
  Node<Vertex *> *curVertex;
  Node<Edge *> *curEdge;
  Edge *minEdge = nullptr, *tmpEdge = nullptr;
  std::unordered_map<Edge *, bool> visitedEdges;
  std::unordered_map<Vertex *, bool> visitedVertices;
  std::unordered_map<Vertex *, std::vector<Vertex *>> adjLists;
  std::vector<Vertex *> curAdjList;
  bool vertexFullyVisited = false;
  int minWeight;

  getAdjacencyList(adjLists);
  // printAdjacentList(&adjLists);

  do {
    // Parse all the existing vertices in the graph that are not visited and try
    // to find one that is connected to the spanning tree with a minEdge Use the
    // adjacent list to get the connected edges that are not visited Once you
    // are done visiting all the edges of a vertex, mark it as visited Select
    // the min cost available edge among those connected edges
    //    If none is found break the main loop
    //    Otherwise:
    //      - If it does not cause a cycle, add it to the minSpanTree
    //      - Mark the minEdge as visited whether added or not to the tree

    curVertex = getVertexList()->getHead();
    minWeight = INT_MAX;
    while (curVertex) {
      vertexFullyVisited = true;
      if (!visitedVertices[curVertex->getData()]) {

        // Find the corresponding adjacent list for the current vertex
        for (const auto &list : adjLists) {
          if (list.first->getData() == curVertex->getData()->getData()) {
            curAdjList = list.second;
            break;
          }
        }

        // Find the minEdge that is connected to the minSpanTree with the
        // adjacency list
        for (const auto &adjVertex : curAdjList) {
          tmpEdge =
              getEdge(curVertex->getData()->getData(), adjVertex->getData());
          if (!visitedEdges[tmpEdge] && minSpanTree.isLinked(tmpEdge)) {
            if (tmpEdge->getWeight() < minWeight) {
              minEdge = tmpEdge;
              minWeight = minEdge->getWeight();
            }
          }

          if (!visitedEdges[tmpEdge])
            vertexFullyVisited = false;
        }
      }

      if (vertexFullyVisited)
        visitedVertices[curVertex->getData()] = true;

      curVertex = curVertex->getNext();
    }

    // Check the min edge found
    if (minEdge && !visitedEdges[minEdge]) {
      tmpSpanTree = new Graph(minSpanTree);

      tmpSpanTree->insertVertex((*minEdge)[0]->getData());
      tmpSpanTree->insertVertex((*minEdge)[1]->getData());
      tmpSpanTree->insertEdge((*minEdge)[0]->getData(),
                              (*minEdge)[1]->getData(), minEdge->getWeight());

      if (!tmpSpanTree->hasCycle()) {
        minSpanTree.insertVertex((*minEdge)[0]->getData());
        minSpanTree.insertVertex((*minEdge)[1]->getData());
        minSpanTree.insertEdge((*minEdge)[0]->getData(),
                               (*minEdge)[1]->getData(), minEdge->getWeight());
      }

      delete tmpSpanTree;

      visitedEdges[minEdge] = true;
    } else {
      // Check a forest in the original graph to find a minEdge

      // Prioritize finding one that is connected to the minSpanTree
      curEdge = edgeList->getHead();
      minWeight = INT_MAX;
      minEdge = nullptr;
      while (curEdge) {
        if (!visitedEdges[curEdge->getData()]) {
          if ((!minSpanTree.isLinked(minEdge) &&
               (minSpanTree.isLinked(curEdge->getData()) ||
                (curEdge->getData()->getWeight() < minWeight))) ||
              (minSpanTree.isLinked(minEdge) &&
               minSpanTree.isLinked(curEdge->getData()) &&
               (curEdge->getData()->getWeight() < minWeight))) {
            minEdge = curEdge->getData();
            minWeight = minEdge->getWeight();
          }
        }

        curEdge = curEdge->getNext();
      }
    }
  } while (minEdge);

  // Display the result spanning tree in sorted order according to the first and
  // second vertex
  std::vector<Edge *> edges;
  curEdge = minSpanTree.getEdgeList()->getHead();
  while (curEdge) {
    edges.push_back(curEdge->getData());
    curEdge = curEdge->getNext();
  }

  std::sort(edges.begin(), edges.end(),
            [](Edge *a, Edge *b) { return *a < *b; });
  for (auto &e : edges)
    std::cout << e << std::endl;
}

void test(Graph *g);

int main() {
  Graph g;

  // test(&g);

  int n, m, i, u, v, w;
  std::cin >> n >> m;

  for (i = 0; i < n; i++) {
    std::cin >> u;
    g.insertVertex(u);
  }

  for (i = 0; i < m; i++) {
    std::cin >> u >> v >> w;
    g.insertEdge(u, v, w);
  }

  g.minimumCostSpanningTree();

  return 0;
}

// void test(Graph *g) {
//   int n, m, i, u, v, w;
//   std::string filename = "test_b2.txt";
//   // std::cout << "Test filename: ";
//   // std::getline(std::cin, filename);
//   std::ifstream inFile(filename);
//   if (!inFile) {
//     std::cout << "File could not be opened" << std::endl;
//     exit(-1);
//   }
//
//   inFile >> n >> m;
//
//   for (i = 0; i < n; i++) {
//     inFile >> u;
//     g->insertVertex(u);
//   }
//
//   for (i = 0; i < m; i++) {
//     inFile >> u >> v >> w;
//     g->insertEdge(u, v, w);
//   }
//   std::cout << std::endl;
//   g->minimumCostSpanningTree();
// };
