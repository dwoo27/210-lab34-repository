#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <string>
using namespace std;

const int SIZE = 9;
const int INF = 999999;

struct Edge {
	int src, dest, weight;
};

typedef pair<int, int> Pair; // Creates alias 'Pair' for the pair<int,int> data type

class Graph {
	// Stores the real-world names of each campus shuttle stop
	vector<string> stopNames; 

public:
	// a vector of vectors of Pairs to represent an adjacency list
	vector<vector<Pair>> adjList;
	// Graph Constructor
	Graph(vector<Edge> const& edges, vector<string> const& names) {
		// Copy the stop names into the class member variable
		stopNames = names;
		// resize the vector to hold SIZE elements of type vector<Edge>
		adjList.resize(SIZE);
		// add edges to the directed graph
		for (auto& edge : edges) {
			int src = edge.src;
			int dest = edge.dest;
			int weight = edge.weight;
			// insert at the end
			adjList[src].push_back(make_pair(dest, weight));
			// for an undirected graph, add an edge from dest to src also
			adjList[dest].push_back(make_pair(src, weight));
		}
	}
	// Breadth-First Search traversal
    // BFS explores the graph level by level from the starting node
	void BFS(int start) {
		// Keeps track of whether each node has already been visited
		vector<bool> visited(SIZE, false);

		// Queue is used for BFS because it follows First In, First Out order
		queue<int> q;

		// Mark the starting node as visited and add it to the queue
		visited[start] = true;
		q.push(start);

		cout << "\nShuttle Service Area Check (BFS) from Stop "
			<< start << " (" << stopNames[start] << "):\n";
		cout << "Purpose: Checking stops by distance from the starting shuttle stop\n";
		cout << "=================================================================\n";

		// Continue until there are no more nodes to visit
		while (!q.empty()) {
			// Get the next node from the front of the queue
			int current = q.front();
			q.pop();

			cout << "Checking Stop " << current
				<< " (" << stopNames[current] << ")\n";

			// Visit each neighbor of the current node
			for (Pair neighbor : adjList[current]) {
				int next = neighbor.first;
				int weight = neighbor.second;

				// If the neighbor has not been visited, mark it and enqueue it
				if (!visited[next]) {
					visited[next] = true;
					q.push(next);

					cout << "  -> Next reachable stop: Stop " << next
						<< " (" << stopNames[next] << ")"
						<< " - Travel time: " << weight << " minutes\n";
				}
			}
		}

		cout << endl;
	}

	// Depth-First Search traversal
    // DFS explores as far as possible down one path before backtracking
	void DFS(int start) {
		// Keeps track of whether each node has already been visited
		vector<bool> visited(SIZE, false);

		// Stack is used for DFS because it follows Last In, First Out order
		stack<int> s;

		// Mark the starting node as visited and push it onto the stack
		visited[start] = true;
		s.push(start);

		cout << "\nEmergency Shuttle Route Trace (DFS) from Stop "
			<< start << " (" << stopNames[start] << "):\n";
		cout << "Purpose: Planning a deep route through connected campus stops\n";
		cout << "=============================================================\n";

		// Continue until there are no more nodes in the stack
		while (!s.empty()) {
			// Get the node from the top of the stack
			int current = s.top();
			s.pop();

			cout << "Inspecting Stop " << current
				<< " (" << stopNames[current] << ")\n";

			// Visit each neighbor of the current node
			for (Pair neighbor : adjList[current]) {
				int next = neighbor.first;
				int weight = neighbor.second;


				// If the neighbor has not been visited, mark it and push it
				if (!visited[next]) {
					visited[next] = true;
					s.push(next);

					cout << "  -> Possible route to Stop " << next
						<< " (" << stopNames[next] << ")"
						<< " - Travel time: " << weight << " minutes\n";
				}
			}
		}

		cout << endl;
	}

	// Calculates shortest paths from a starting node using a simple Dijkstra method
	void shortestPath(int start) {
		vector<int> distance(SIZE, INF);
		vector<bool> visited(SIZE, false);

		distance[start] = 0;

		// Repeat once for each node in the graph
		for (int count = 0; count < SIZE - 1; count++) {
			int currentNode = -1;
			int smallestDistance = INF;

			// Find the unvisited node with the smallest distance
			for (int i = 0; i < SIZE; i++) {
				if (!visited[i] && distance[i] < smallestDistance) {
					smallestDistance = distance[i];
					currentNode = i;
				}
			}

			// If no reachable unvisited node was found, stop
			if (currentNode == -1) {
				break;
			}

			// Mark this node as visited
			visited[currentNode] = true;

			// Update distances for all neighbors of the current node
			for (Pair neighbor : adjList[currentNode]) {
				int nextNode = neighbor.first;
				int weight = neighbor.second;

				if (!visited[nextNode] &&
					distance[currentNode] + weight < distance[nextNode]) {
					distance[nextNode] = distance[currentNode] + weight;
				}
			}
		}

		cout << "\nShortest travel times from Stop "
			<< start << " (" << stopNames[start] << "):\n";
		cout << "================================================\n";

		for (int i = 0; i < SIZE; i++) {
			cout << start << " -> " << i
				<< " (" << stopNames[i] << ") : ";

			if (distance[i] == INF) {
				cout << "No route";
			}
			else {
				cout << distance[i] << " minutes";
			}

			cout << endl;
		}
	}

	// Finds the main parent of a node
// This helps determine which group a node belongs to
	int findParent(vector<int>& parent, int node) {
		if (parent[node] == node) {
			return node;
		}

		return findParent(parent, parent[node]);
	}

	// Combines two groups into one group
	void unionSets(vector<int>& parent, int firstNode, int secondNode) {
		int firstParent = findParent(parent, firstNode);
		int secondParent = findParent(parent, secondNode);

		parent[secondParent] = firstParent;
	}

	// Uses Kruskal's Algorithm to find the Minimum Spanning Tree
	void minimumSpanningTree(vector<Edge> edges) {
		// Sort all edges from smallest weight to largest weight
		sort(edges.begin(), edges.end(), [](Edge a, Edge b) {
			return a.weight < b.weight;
			});

		// parent[i] starts as itself, meaning each node begins in its own group
		vector<int> parent(SIZE);

		for (int i = 0; i < SIZE; i++) {
			parent[i] = i;
		}

		cout << "\nMinimum Spanning Tree edges using Kruskal's Algorithm:\n";
		cout << "=====================================================\n";

		int edgeCount = 0;
		int totalWeight = 0;

		// Go through each edge from cheapest to most expensive
		for (Edge edge : edges) {
			int srcParent = findParent(parent, edge.src);
			int destParent = findParent(parent, edge.dest);

			// If the source and destination are in different groups,
			// adding this edge will not create a cycle
			if (srcParent != destParent) {
				cout << "Edge from Stop " << edge.src
					<< " (" << stopNames[edge.src] << ")"
					<< " to Stop " << edge.dest
					<< " (" << stopNames[edge.dest] << ")"
					<< " with travel time: " << edge.weight << " minutes\n";

				totalWeight += edge.weight;
				edgeCount++;

				// Combine the two groups
				unionSets(parent, srcParent, destParent);
			}

			// A spanning tree for SIZE nodes only needs SIZE - 1 edges
			if (edgeCount == SIZE - 1) {
				break;
			}
		}

		cout << "\nTotal minimum travel time to connect all stops: "
			<< totalWeight << " minutes\n";
	}

	// Print the graph's adjacency list
	void printGraph() {
		cout << "Graph's adjacency list:" << endl;
		for (int i = 0; i < adjList.size(); i++) {
			cout << i << " --> ";
			for (Pair v : adjList[i])
				cout << "(" << v.first << ", " << v.second << ") ";
			cout << endl;
		}
	}

	// Displays the full shuttle network with each stop and its connections
	void displayNetwork() {
		cout << "\nSmart Campus Shuttle Network:\n";
		cout << "=============================\n";

		// Loop through every stop in the graph
		for (int i = 0; i < adjList.size(); i++) {
			cout << "Stop " << i << " (" << stopNames[i] << ") connects to:\n";

			// Print all neighboring stops connected to this stop
			for (Pair neighbor : adjList[i]) {
				cout << "  -> Stop " << neighbor.first
					<< " (" << stopNames[neighbor.first] << ")"
					<< " - Travel time: " << neighbor.second << " minutes\n";
			}

			cout << endl;
		}
	}
};
int main() {
	// Creates a vector of graph edges/weights
	vector<Edge> edges = {
		// (x, y, w) —> edge from x to y having weight w
		{0, 1, 8},
		{0, 2, 21},
		{1, 2, 6},
		{1, 3, 5},
		{1, 4, 4},
		{2, 7, 11},
		{2, 8, 8},
		{3, 4, 9},
		{5, 6, 10},
		{5, 7, 15},
		{5, 8, 5},
		{6, 7, 3},
		{6, 8, 7},
	};

	// Names for each campus shuttle stop
	vector<string> stopNames = {
		"Main Gate",
		"Library",
		"Science Hall",
		"Student Center",
		"Gym",
		"Dorm A",
		"Dorm B",
		"Parking Garage",
		"Cafeteria"


	};

	// Create the campus shuttle network graph
	Graph campusNetwork(edges, stopNames);
	campusNetwork.displayNetwork();
	campusNetwork.DFS(0);
	campusNetwork.BFS(0);
	campusNetwork.shortestPath(0);
	campusNetwork.minimumSpanningTree(edges);
	return 0;
}

