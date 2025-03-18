#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>

using namespace std;

class Graph {
public:
    const int INF = 1e9;
    int numLocations;
    vector<vector<pair<int, int>>> Adj_list;

    Graph(int locations) {
        numLocations = locations;
        Adj_list.resize(locations);
    }

    void AddRoad(int from, int to, int distance) {
        Adj_list[from].push_back({ to, distance });
        Adj_list[to].push_back({ from, distance });
    }

    void exploreRoutes(int startNode, vector<bool>& visited) {
        cout << startNode << " ";
        visited[startNode] = true;
        for (auto x : Adj_list[startNode]) {
            if (!visited[x.first]) {
                exploreRoutes(x.first, visited);
            }
        }
    }

    void exploreAllRoutes(int warehouse) {
        vector<bool> visited(numLocations, false);
        cout << "\nExploring routes from location " << warehouse << ":\n";
        exploreRoutes(warehouse, visited);
        cout << endl;
    }

    vector<int> Dijkstra(int start) {
        vector<int> distance(numLocations, INF);
        distance[start] = 0;
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        pq.push({ 0, start });

        while (!pq.empty()) {
            int dist = pq.top().first;
            int node = pq.top().second;
            pq.pop();

            if (dist > distance[node]) continue;

            for (auto neighbor : Adj_list[node]) {
                int newNode = neighbor.first;
                int weight = neighbor.second;
                if (distance[node] + weight < distance[newNode]) {
                    distance[newNode] = distance[node] + weight;
                    pq.push({ distance[newNode], newNode });
                }
            }
        }
        return distance;
    }

    void sortDeliveries(int warehouse, vector<int>& deliveryLocations) {
        vector<int> shortestDistances = Dijkstra(warehouse);
        vector<pair<int, int>> sortedDeliveries;

        for (int location : deliveryLocations) {
            sortedDeliveries.push_back({ shortestDistances[location], location });
        }

        sort(sortedDeliveries.begin(), sortedDeliveries.end());

        cout << "\nDeliveries sorted from nearest to farthest:\n";
        for (auto delivery : sortedDeliveries) {
            cout << "Location " << delivery.second << " (Distance: " << delivery.first << ")\n";
        }
    }

    int knapsackOptimization(int fuelCapacity, vector<pair<int, int>>& deliveries) {
        int n = deliveries.size();
        vector<vector<int>> dp(n + 1, vector<int>(fuelCapacity + 1, 0));

        for (int i = 1; i <= n; i++) {
            int fuelNeeded = deliveries[i - 1].first;
            int priority = deliveries[i - 1].second;

            for (int j = fuelCapacity; j >= fuelNeeded; j--) {
                dp[i][j] = max(dp[i - 1][j], dp[i - 1][j - fuelNeeded] + priority);
            }
        }
        return dp[n][fuelCapacity];
    }
};

int main() {
    int locations, roads;
    cout << "Enter number of locations and roads: ";
    cin >> locations >> roads;

    Graph system(locations);

    cout << "Enter roads (format: from to distance):\n";
    for (int i = 0; i < roads; i++) {
        int from, to, distance;
        cin >> from >> to >> distance;
        system.AddRoad(from, to, distance);
    }

    int warehouse;
    cout << "Enter warehouse location: ";
    cin >> warehouse;

    int numDeliveries;
    cout << "Enter number of deliveries: ";
    cin >> numDeliveries;

    vector<int> deliveryLocations;
    vector<pair<int, int>> deliveryStops;

    cout << "Enter delivery locations (format: location priority):\n";
    for (int i = 0; i < numDeliveries; i++) {
        int location, priority;
        cin >> location >> priority;
        deliveryLocations.push_back(location);
    }

    int fuelCapacity;
    cout << "Enter truck's fuel capacity: ";
    cin >> fuelCapacity;

    cout << "Enter delivery stops (format: fuel_needed priority_value):\n";
    for (int i = 0; i < numDeliveries; i++) {
        int fuelNeeded, priorityValue;
        cin >> fuelNeeded >> priorityValue;
        deliveryStops.push_back({ fuelNeeded, priorityValue });
    }

    system.exploreAllRoutes(warehouse);

    vector<int> distances = system.Dijkstra(warehouse);
    cout << "\nShortest routes from warehouse:\n";
    for (int i = 0; i < locations; i++) {
        if (i != warehouse) {
            cout << "To location " << i << ": " << distances[i] << " units\n";
        }
    }

    system.sortDeliveries(warehouse, deliveryLocations);

    int maxPriority = system.knapsackOptimization(fuelCapacity, deliveryStops);
    cout << "\nMax achievable delivery priority within fuel limit: " << maxPriority << endl;

    return 0;
}
