#include <iostream>
#include <vector>
#include <queue>
#include <stack>
using namespace std;

const int SIZE = 9;

struct Edge {
	int src, dest, weight;
};

typedef pair<int, int> Pair; // Creates alias 'Pair' for the pair<int,int> data type

class Graph {
public:
	// a vector of vectors of Pairs to represent an adjacency list
	vector<vector<Pair>> adjList;
	// Graph Constructor
	Graph(vector<Edge> const& edges) {
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

		cout << endl << "BFS starting from vertex " << start << ":" << endl;

		// Continue until there are no more nodes to visit
		while (!q.empty()) {
			// Get the next node from the front of the queue
			int current = q.front();
			q.pop();

			cout << current << " ";

			// Visit each neighbor of the current node
			for (Pair neighbor : adjList[current]) {
				int next = neighbor.first;

				// If the neighbor has not been visited, mark it and enqueue it
				if (!visited[next]) {
					visited[next] = true;
					q.push(next);
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

		cout << "\nDFS starting from vertex " << start << ":\n";

		// Continue until there are no more nodes in the stack
		while (!s.empty()) {
			// Get the node from the top of the stack
			int current = s.top();
			s.pop();

			cout << current << " ";

			// Visit each neighbor of the current node
			for (Pair neighbor : adjList[current]) {
				int next = neighbor.first;

				// If the neighbor has not been visited, mark it and push it
				if (!visited[next]) {
					visited[next] = true;
					s.push(next);
				}
			}
		}

		cout << endl;
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
	// Creates graph
	Graph graph(edges);
	// Prints adjacency list representation of graph
	graph.printGraph();

	graph.DFS(0);
	graph.BFS(0);

	return 0;
}