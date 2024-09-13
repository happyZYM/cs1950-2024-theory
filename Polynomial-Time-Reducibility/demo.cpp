#include <algorithm>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// 表示一个文字（变量或其否定）
struct Literal {
  int variable;
  bool is_negated;
};

// 表示一个子句
using Clause = vector<Literal>;

// 3-SAT 公式
using Formula = vector<Clause>;

// 图的邻接矩阵表示
using Graph = vector<vector<bool>>;

// 检查两个文字是否相容（不矛盾）
bool AreCompatible(const Literal &l1, const Literal &l2) {
  return l1.variable != l2.variable || l1.is_negated == l2.is_negated;
}

// 将3-SAT公式归约到图
Graph ReduceToGraph(const Formula &formula) {
  int n = formula.size() * 3; // 顶点数量
  Graph graph(n, vector<bool>(n, false));

  for (int i = 0; i < formula.size(); i++) {
    if (formula[i].size() != 3)
      throw std::invalid_argument(
          "Each clause must contain exactly 3 literals.");
    for (int j = i + 1; j < formula.size(); j++) {
      for (int k = 0; k < 3; k++) {
        for (int l = 0; l < 3; l++) {
          if (AreCompatible(formula[i][k], formula[j][l])) {
            graph[i * 3 + k][j * 3 + l] = true;
            graph[j * 3 + l][i * 3 + k] = true;
          }
        }
      }
    }
  }

  return graph;
}

// 检查图中是否存在大小为k的团
bool HasClique(const Graph &graph, int k) {
  int n = graph.size();
  vector<int> vertices(n);
  for (int i = 0; i < n; i++)
    vertices[i] = i;

  vector<int> combination(k);

  // 生成所有k个顶点的组合并检查是否形成团
  function<bool(int, int)> findClique = [&](int start, int depth) {
    if (depth == k) {
      for (int i = 0; i < k; i++) {
        for (int j = i + 1; j < k; j++) {
          if (!graph[combination[i]][combination[j]]) {
            return false;
          }
        }
      }
      return true;
    }

    for (int i = start; i <= n - k + depth; i++) {
      combination[depth] = vertices[i];
      if (findClique(i + 1, depth + 1)) {
        return true;
      }
    }
    return false;
  };

  return findClique(0, 0);
}

int main() {
  // 示例: (x1 OR !x2 OR x3) AND (x2 OR x3 OR !x4) AND (!x1 OR !x3 OR x4)
  Formula formula = {{{1, false}, {2, true}, {3, false}},
                     {{2, false}, {3, false}, {4, true}},
                     {{1, true}, {3, true}, {4, false}}};

  Graph graph = ReduceToGraph(formula);
  int k = formula.size(); // 团的大小等于子句数

  cout << "Graph adjacency matrix:" << endl;
  for (const auto &row : graph) {
    for (bool cell : row) {
      cout << (cell ? "1 " : "0 ");
    }
    cout << endl;
  }

  bool hasCliqueOfSizeK = HasClique(graph, k);

  cout << "Does the graph have a clique of size " << k << "? "
       << (hasCliqueOfSizeK ? "Yes" : "No") << endl;

  cout << "The original 3-SAT formula is "
       << (hasCliqueOfSizeK ? "satisfiable" : "unsatisfiable") << endl;

  return 0;
}