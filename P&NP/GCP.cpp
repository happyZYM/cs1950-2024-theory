#include <iostream>
#include <vector>

class Graph {
private:
  int V; // 顶点数
  std::vector<std::vector<int>> adj; // 邻接矩阵

public:
  Graph(int v) : V(v), adj(v, std::vector<int>(v, 0)) {}

  void AddEdge(int v, int w) {
    adj[v][w] = 1;
    adj[w][v] = 1;
  }

  bool IsSafe(int v, const std::vector<int>& color, int c) {
    for (int i = 0; i < V; i++)
      if (adj[v][i] && c == color[i])
        return false;
    return true;
  }

  bool GraphColoringUtil(std::vector<int>& color, int m, int v) {
    if (v == V)
      return true;

    for (int c = 1; c <= m; c++) {
      if (IsSafe(v, color, c)) {
        color[v] = c;
        if (GraphColoringUtil(color, m, v + 1))
          return true;
        color[v] = 0;
      }
    }

    return false;
  }

  bool GraphColoring(int m) {
    std::vector<int> color(V, 0);
    if (GraphColoringUtil(color, m, 0)) {
      std::cout << "Solution exists. The coloring is as follows:\n";
      for (int i = 0; i < V; i++)
        std::cout << "Vertex " << i << " -> Color " << color[i] << "\n";
      return true;
    }
    std::cout << "Solution does not exist.\n";
    return false;
  }
};

int main() {
  Graph g(4);
  g.AddEdge(0, 1);
  g.AddEdge(0, 2);
  g.AddEdge(1, 2);
  g.AddEdge(1, 3);
  g.AddEdge(2, 3);

  int m = 3; // 颜色数量
  g.GraphColoring(m);

  return 0;
}