#include <iostream>
#include <vector>
#include <random>
#include <limits>
#include <string>

using namespace std;


// struct for nodes in graph (unweighted)
struct node {
	int row;
	int col;
	int type;
	int g;
	int h;
	int f;
	node *parent;
};

// size of graph (SIZE X SIZE matrix)
const int SIZE = 40;

// random number generator
mt19937 rng;
// uniform random distibution for choosing start and finish coordinates
uniform_int_distribution<mt19937::result_type> runif_SIZE(0, SIZE - 1);

// SIZE x SIZE matrix of nodes to represent graph
node M[SIZE][SIZE];

// pointers to start and end nodes
node *start;
node *finish;

// open list: (pointers to) nodes to be expanded
vector<node*> open_list;
// closed list: (pointers to) nodes which have been expanded
vector<node*> closed_list;


// returns the index of the (pointer to) node in the open list
// with the lowest f value
int get_from_open_list() {
	// minimum f value so far
	int min_f = numeric_limits<int>::max();
	// index at which this occurs
	int min_f_i = 0;
	// traverse open list
	for (int i = 0; i < (int)open_list.size(); i++) {
		// update min_f and min_f_i if current element has
		// lower f value than min so far
		int current_f = open_list[i]->f;
		if (current_f <= min_f) {
			min_f = current_f;
			min_f_i = i;
		}
	}
	// return index
	return min_f_i;
}

// heuristic function for estimating cost of travelling to finish
// using manhattan distance
int heuristic(const node * const n) {
	// return manhattan distance from n to finish
	return abs(n->row - finish->row) + abs(n->col - finish->col);
}

// adds a (pointer to) node to the open list after
// setting its h and f values
void add_to_open_list (node * const n) {
	// set h based on heuristic
	n->h = heuristic(n);
	// set f as g + h
	n->f = n->g + n->h;
	// add n to open list
	open_list.push_back(n);
}

// check if (pointer to) node appears in given vector
bool is_in_list(const vector<node*> * const list, const node * const n_to_find) {
	// traverse given vector
	for (node *n : *list) {
		// return true if node is found
		if(n == n_to_find) {
			return true;
		}
	}
	// return false if (pointer to) node not found 
	return false;
}

// checks if (pointer to) node appears in open or closed list
bool in_open_or_closed(const node * const n_to_find) {
	// check if node is in closed list
	if (is_in_list(&closed_list, n_to_find)) {
		return true;
	}
	// if not, check if node is in open list
	else {
		return is_in_list(&open_list, n_to_find);
	}
}

// randomly populate the graph with start, finish and obstacles
// (may or may not be solvable)
void gen_matrix(int obs_odds) {
	
	uniform_int_distribution<mt19937::result_type> runif_obstacle(0, obs_odds);
	
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
			M[i][j] = { i, j, 0, 0, 0, 0, nullptr };
			// radomly make node an obstacle
			if (runif_obstacle(rng) == 0) {
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
				int g = M[i][j].g;
				if (g >= 0 && g <= 9) {
					element += " ";
				}
				// element to print is g (distance from start)
				if (g == 0) {
					element += "-";
				}
				else {
					element += to_string(g);
				}
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
	if (north >= 0) {
		node *north_node = &M[north][col];
		if (north_node->type < 2) {
			adj_nodes.push_back(north_node);
		}
	}
	if (south <= SIZE - 1) {
		node *south_node = &M[south][col];
		if (south_node->type < 2) {
			adj_nodes.push_back(south_node);
		}
	}
	if (west >= 0) {
		node *west_node = &M[row][west];
		if (west_node->type < 2) {
			adj_nodes.push_back(west_node);
		}
	}
	if (east <= SIZE - 1) {
		node *east_node = &M[row][east];
		if (east_node->type < 2) {
			adj_nodes.push_back(east_node);
		}
	}
	
	// return vector of (pointers to) adjacent nodes
	return adj_nodes;
}

int iterations = 0;

bool astar() {
	bool solved = false;
	// add (pointer to) starting node to open list
	open_list.push_back(start);
	// loop while open list is not empty and maze is not solved
	while (open_list.size() > 0 && !solved) {
		// increment iterations counter
		iterations++;
		// get index of (pointer to) node with lowest f value
		int lowest_f_i = get_from_open_list();
		node *current_node = open_list[lowest_f_i];
		// remove (pointer to) this node from open list
		open_list.erase(open_list.begin() + lowest_f_i);
		// if this node is finish, maze is solved
		if (current_node->type == 1) {
			solved = true;
		}
		// otherwise
		else {
			// next g (distance from start) is current g +1
			int next_g = current_node->g + 1;
			// for (pointer to) each adjacent node which is not an obstacle or the start
			for (node *n : get_adj_nodes(current_node)) {
				// check whether (pointer to) this node occurs in open or closed list
				bool in_either_list = in_open_or_closed(n);
				// if (pointer to) node is present in open or closed lists and current g is lower
				// i.e. current path to this node is shorter than one previously found
				// update g of node and change its parent to current node
				if (next_g < n->g && in_either_list) {
					n->g = next_g;
					n->parent = current_node;
				}
				// if node is not in either list, set its g to current g
				// and its parent to (pointer to) the current node, and
				// add it to the open list
				else if (!in_either_list) {
					n->parent = current_node;
					n->g = next_g;
					add_to_open_list(n);
				}
			}
			// after processing child nodes, add (pointer to) current node to closed list
			closed_list.push_back(current_node);
		}
	}
	// print message and return false if maze is unsolved after open list
	// is depleted
	if (!solved) {
		cout << "\nMaze not solvable!" << endl;
		return false;
	}
	// return true if maze is solved
	return true;
}

// trace back from finish and set type of nodes in path
void backtrace() {
	// whether backtrace is finished
	bool done = false;
	// (pointer to) current node in path, initialized to 
	// (pointer to) parent of finish node
	node *pos = finish->parent;
	// loop until done (i.e. parent node is start)
	while (!done) {	
		// set type to 4 / path
		pos->type = 4;
		// done if parent type is 2 / start
		if (pos->parent->type == 2) {
			done = true;
		}
		// otherwise move to parent node
		else {
			pos = pos->parent;
		}
	}
}

int main() {
	// seed rng
	rng.seed(random_device()());
	// generate matrix
	gen_matrix(5);
	// attempt to solve
	bool solved = astar();
	// backtrace and print details if solved
	if (solved) {
		backtrace();
		print_matrix();
		cout << "Iterations: " << iterations << endl;
		cout << "Path length: " << finish->g << endl;
	}
	return 0;
}
