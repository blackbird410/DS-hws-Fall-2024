void Graph::BFS(int v) {
  visited = new bool[NumberVecterxs];
  fill(visited, visited + NumberVecterxs, false);
  visited[v] = true;
  list<int> q;
  q.push_back(v);
  while (!q.empty()) {
    v = q.front();
    cout << v << endl;
    q.pop_front();
    for (auto it : adjacent_list[v]) {
      if (!visited[it[0]])
        q.push_back(it[0]);
      visited[it[0]] = true;
    }
  }
  delete visited;
}
