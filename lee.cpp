#include <iostream>
#include <random>
#include <string>
#include <queue>
#include <algorithm>
#include <limits>

using namespace std;

// struct for node
struct node {
	int row;
	int col;
	int dist;
	int type;
};

// random number generator
mt19937 rng;

// size of graph (SIZE x SIZE matrix)
const int SIZE = 40;
// uniform random distribution for generating start and finish coords
uniform_int_distribution<mt19937::result_type> runif_SIZE(0, SIZE - 1);
// graph
node M[SIZE][SIZE];
// pointer to start and finish nodes
node *start;
node *finish;
// queue of nodes to expand
queue<node*> q;

// randomly populate the graph with start, finish and obstacles
// (may or may not be solvable)
void gen_matrix() {
	
	// arrays for start and finish coordinates
	int start_coords[2];
	int finish_coords[2];
	
	// randomly generate start coordinates
	for (int i = 0; i < 2; i++) {
		start_coords[i] = runif_SIZE(rng);
	}
	// generate finish coordinates and ensure they are not the same
	do {
		for (int i = 0; i < 2; i++) {
			finish_coords[i] = runif_SIZE(rng);
		}
	} while (start_coords[0] == finish_coords[0] && start_coords[1] == finish_coords[1]);
	
	// populate graph
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			M[i][j] = { i, j, 0, 0 };
			// radomly make node an obstacle
			if (runif_SIZE(rng) < 4) {
				M[i][j].type = 3; // obstacle
			}
			// set node as start if coordinates match
			if (i == start_coords[0] && j == start_coords[1]) {
				M[i][j].type = 2; // start
				start = &M[i][j];
			}
			// set node as finish if coordinates match
			if (i == finish_coords[0] && j == finish_coords[1]) {
				M[i][j].type = 1; // finish
				finish = &M[i][j];
			}
		}
	}
}

// print graph to console
void print_matrix() {
	
	// loop through row indices
	for (int i = 0; i < SIZE; i++) {
		// loop through column indices
		for (int j = 0; j < SIZE; j++) {
			// element to print
			string element = "";
			// type of node
			int type = M[i][j].type;
			// if normal node
			if (type == 0) {
				int dist = M[i][j].dist;
				if (dist >= 0 && dist <= 9) {
					element += " ";
				}
				// element to print is dist (distance from start)
				element += to_string(dist);
			}
			// print F for finish node
			else if (type == 1) {
				element += " F";
			}
			// print S for start node
			else if (type == 2) {
				element += " S";
			}
			// print # for obstacles
			else if (type == 3) {
				element += " #";
			}
			// print . for nodes on path
			else if (type == 4) {
				element += " .";
			}
			cout << element << "  ";
		}
		cout << "\n" << endl;
	}
	cout << endl;
}

// returns vector of (pointers to) all non-obstacle nodes adjacent to given
vector<node*> get_adj_nodes(const node * const n) {

	vector<node*> adj_nodes = {};
	
	// row and column indices of current node
	int row = n->row;
	int col = n->col;
	// row index - 1 (one row north)
	int north = n->row - 1;
	// row index + 1 (one row south)
	int south = n->row + 1;
	// column index - 1 (one column west)
	int west = n->col - 1;
	// column index + 1 (one column east)
	int east = n->col + 1;
	
	// for NSWE, add (pointer to) adjacent node to vector if
	// it is valid (within bounds of graph) and is either 
	// a standard node or the finish node
	if (north >= 0 && (M[north][col].type <=2)) {
		adj_nodes.push_back(&M[north][col]);
	}
	if (south <= SIZE - 1 && (M[south][col].type <= 2)) {
		adj_nodes.push_back(&M[south][col]);
	}
	if (west >= 0 && (M[row][west].type <= 2)) {
		adj_nodes.push_back(&M[row][west]);
	}
	if (east <= SIZE - 1 && (M[row][east].type <= 2)) {
		adj_nodes.push_back(&M[row][east]);
	}
	
	// return vector of (pointers to) adjacent nodes
	return adj_nodes;
}

int iterations = 0;

void find_path() {
	
	q.push(start);
	bool done = false;
	while (!done) {
		iterations++;
		node *next_node = q.front();
		q.pop();
		for (node *n : get_adj_nodes(next_node)) {
			if (n->type == 1) {
				done = true;
			}
			else if (n->type <= 1 && n->dist == 0) {
				n->dist = max(next_node->dist + 1, 1);
				q.push(n);
			}
		}
	}
}

void backtrace() {
	
	int min_dist = numeric_limits<int>::max();
	node *pos = finish;
	bool done = false;
	while(!done) {
		for (node *n : get_adj_nodes(pos)) {
			if(n->type == 2) {
				done = true;
			}
			else if (n->dist < min_dist && n->dist > 0) {
				min_dist = n->dist;
				pos = n;
			}
		}
		if(!done) {
			pos->type = 4;
		}
	}	
}

int main() {
	
	rng.seed(random_device()());
	
	gen_matrix();
	print_matrix();
	find_path();
	print_matrix();
	backtrace();
	print_matrix();
	cout << "Iterations: " << iterations << endl;
	
	return 0;
}
