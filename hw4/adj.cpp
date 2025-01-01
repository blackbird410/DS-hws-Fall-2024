#include <iostream>
#include <list>
#include <vector>

using namespace std;

class Graph {
  /* objects: A nonempty set of vertices and a set of
     undirected edges where each edge is a pair of vertices */
public:
  Graph(); // Create an empty graph
  void InsertVertex(int v);
  void InsertEdge(int u, int v, int weight);
  bool IsEmpty(); // if graph has no vertices return TRUE
  list<vector<int>> Adjacent_List(int u);
  void PrintAdjacentMatrix();
  void PrintAdjacentList();

private:
  int NumberVecterxs;
  int NumberEdges;
  list<int> vertexs;
  vector<vector<int>> adjacent_matrix;
  vector<list<vector<int>>> adjacent_list;
  // return a list of all vertices that are adjacent to v
};

Graph::Graph() { NumberVecterxs = NumberEdges = 0; }
bool Graph::IsEmpty() { return (vertexs.size() == 0); }

list<vector<int>> Graph::Adjacent_List(int i) { return adjacent_list[i]; }

void Graph::InsertVertex(int v) {
  vertexs.push_back(v);
  NumberVecterxs++;
}
void Graph::InsertEdge(int u, int v, int weight = 1) {
  if (adjacent_matrix.size() == 0) {
    for (int i = 0; i < vertexs.size(); i++)
      adjacent_matrix.push_back(vector<int>(vertexs.size()));
  }
  if (adjacent_list.size() == 0) {
    for (int i = 0; i < vertexs.size(); i++)
      adjacent_list.push_back(list<vector<int>>());
  }

  adjacent_matrix[u][v] = adjacent_matrix[v][u] = weight;
  adjacent_list[u].push_back({v, weight});
  adjacent_list[v].push_back({u, weight});
  NumberEdges++;
}

void Graph::PrintAdjacentMatrix() {
  for (int i = 0; i < vertexs.size(); i++) {
    for (int j = 0; j < vertexs.size(); j++) {
      if (adjacent_matrix[i][j])
        cout << "(" << i << ", " << j << ", " << adjacent_matrix[i][j] << ")"
             << endl;
    }
  }
}
void Graph::PrintAdjacentList() {
  for (int i = 0; i < vertexs.size(); i++) {
    for (auto it : adjacent_list[i]) {
      cout << "(" << i << ", " << it[0] << ", " << it[1] << ")" << endl;
    }
  }
}

int main() {
  int u, v;
  int weight;
  int N; // number of input vertexs
  int M; // number of input edges
  /* Template for reading graph with edges with weight = 1*/
  Graph g1;
  cin >> N >> M;
  for (int i = 0; i < N; i++) {
    cin >> u;
    g1.InsertVertex(u);
  }
  for (int j = 0; j < M; j++) {
    cin >> u >> v;
    g1.InsertEdge(u, v);
  }
  /* Test for the input graph */
  g1.PrintAdjacentMatrix();
  g1.PrintAdjacentList();
  cout << endl;
  for (int i = 0; i < N; i++) {
    auto edge_list = g1.Adjacent_List(i);
    for (auto it : edge_list) {
      cout << "(" << i << ", " << it[0] << ", " << it[1] << ")" << endl;
    }
  }
  /* Template for reading graph with edges with weight*/
  Graph g2;
  cin >> N >> M;
  for (int i = 0; i < N; i++) {
    cin >> u;
    g2.InsertVertex(u);
  }
  for (int j = 0; j < M; j++) {
    cin >> u >> v >> weight;
    g2.InsertEdge(u, v, weight);
  }
  return 0;
}
