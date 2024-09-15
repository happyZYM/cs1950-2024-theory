#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>

using namespace std;

struct City {
  int id;
  double x, y;
};

double Distance(const City& a, const City& b) {
  return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

pair<vector<int>, double> ExhaustiveTSP(const vector<City>& cities) {
  int n = cities.size();
  vector<int> best_tour;
  double shortest_distance = numeric_limits<double>::max();

  vector<int> tour(n);
  for (int i = 0; i < n; i++) {
    tour[i] = i;
  }

  do {
    double current_distance = 0;
    for (int i = 0; i < n - 1; i++) {
      current_distance += Distance(cities[tour[i]], cities[tour[i+1]]);
    }
    current_distance += Distance(cities[tour[n-1]], cities[tour[0]]); // Return to start

    if (current_distance < shortest_distance) {
      shortest_distance = current_distance;
      best_tour = tour;
    }
  } while (next_permutation(tour.begin() + 1, tour.end()));

  best_tour.push_back(best_tour[0]); // Add starting city at the end
  return {best_tour, shortest_distance};
}

int main() {
  vector<City> cities = {
    {0, 0, 0}, {1, 2, 4}, {2, 3, 1}, {3, 5, 3}, {4, 1, 5}
  };

  auto [tour, total_distance] = ExhaustiveTSP(cities);

  cout << "Optimal Tour: ";
  for (int city : tour) {
    cout << city << " ";
  }
  cout << endl;
  cout << "Total distance: " << total_distance << endl;

  return 0;
}