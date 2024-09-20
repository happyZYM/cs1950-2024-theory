// Reduce Graph Coloring to SAT
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;

class Graph {
public:
  int V, E, K;
  vector<vector<int>> adj;

  Graph(int v, int k) : V(v), E(0), K(k), adj(v) {}

  void addEdge(int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
    E++;
  }
};

class SATSolver {
private:
  vector<vector<int>> clauses;
  vector<int> assignment;
  int numVars;

  bool DPLL(vector<int>& assignment) {
    if (allClausesSatisfied(assignment)) {
      return true;
    }

    int unitClause = findUnitClause(assignment);
    if (unitClause != 0) {
      assignment[abs(unitClause) - 1] = (unitClause > 0) ? 1 : 0;
      if (DPLL(assignment)) return true;
      assignment[abs(unitClause) - 1] = -1;
      return false;
    }

    int var = chooseBranchingVariable(assignment);
    if (var == -1) return false;  // No unassigned variables left

    vector<int> newAssignment = assignment;
    newAssignment[var] = 1;
    if (DPLL(newAssignment)) {
      assignment = newAssignment;
      return true;
    }
    newAssignment[var] = 0;
    if (DPLL(newAssignment)) {
      assignment = newAssignment;
      return true;
    }

    assignment[var] = -1;  // Backtrack
    return false;
  }

  bool allClausesSatisfied(const vector<int>& assignment) {
    for (const auto& clause : clauses) {
      bool satisfied = false;
      for (int lit : clause) {
        int var = abs(lit) - 1;
        if ((lit > 0 && assignment[var] == 1) || (lit < 0 && assignment[var] == 0)) {
          satisfied = true;
          break;
        }
      }
      if (!satisfied) return false;
    }
    return true;
  }

  int findUnitClause(const vector<int>& assignment) {
    for (const auto& clause : clauses) {
      int unassigned = 0;
      int lastLit = 0;
      bool satisfied = false;
      for (int lit : clause) {
        int var = abs(lit) - 1;
        if (assignment[var] == -1) {
          unassigned++;
          lastLit = lit;
        } else if ((lit > 0 && assignment[var] == 1) || (lit < 0 && assignment[var] == 0)) {
          satisfied = true;
          break;
        }
      }
      if (!satisfied && unassigned == 1) return lastLit;
    }
    return 0;
  }

  int chooseBranchingVariable(const vector<int>& assignment) {
    for (size_t i = 0; i < assignment.size(); i++) {
      if (assignment[i] == -1) return i;
    }
    return -1;
  }

public:
  SATSolver(int vars) : numVars(vars), assignment(vars, -1) {}

  void addClause(const vector<int>& clause) {
    clauses.push_back(clause);
  }

  bool solve() {
    return DPLL(assignment);
  }

  vector<int> getSolution() {
    return assignment;
  }
};

int var(int v, int c, int V) {
  return v * V + c + 1;
}

void addGraphColoringClauses(const Graph& G, SATSolver& solver) {
  // At least one color
  for (int v = 0; v < G.V; v++) {
    vector<int> clause;
    for (int c = 0; c < G.K; c++) {
      clause.push_back(var(v, c, G.V));
    }
    solver.addClause(clause);
  }

  // At most one color
  for (int v = 0; v < G.V; v++) {
    for (int c1 = 0; c1 < G.K; c1++) {
      for (int c2 = c1 + 1; c2 < G.K; c2++) {
        solver.addClause({-var(v, c1, G.V), -var(v, c2, G.V)});
      }
    }
  }

  // Proper coloring
  for (int v = 0; v < G.V; v++) {
    for (int u : G.adj[v]) {
      if (u > v) {
        for (int c = 0; c < G.K; c++) {
          solver.addClause({-var(v, c, G.V), -var(u, c, G.V)});
        }
      }
    }
  }
}

int main() {
  // Create a simple graph: a triangle
  Graph G(3, 3);  // 3 vertices, 3 colors
  G.addEdge(0, 1);
  G.addEdge(1, 2);
  G.addEdge(2, 0);

  SATSolver solver(G.V * G.K);
  addGraphColoringClauses(G, solver);

  cout << "Solving graph coloring problem..." << endl;
  bool isSatisfiable = solver.solve();

  if (isSatisfiable) {
    cout << "Solution found!" << endl;
    vector<int> solution = solver.getSolution();
    for (int v = 0; v < G.V; v++) {
      cout << "Vertex " << v << " color: ";
      for (int c = 0; c < G.K; c++) {
        if (solution[var(v, c, G.V) - 1] == 1) {
          cout << c << endl;
          break;
        }
      }
    }
  } else {
    cout << "No solution exists." << endl;
  }

  return 0;
}