
#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <chrono>
#include <vector>
#include <set>

using namespace std;
class Network
{
public:
	Network();
	~Network();
	void load_edge_list(string filename, bool has_weights);

	void print_edge_list();
	void print_neighbours(int node);
	void prin_dok();

	int common_neighbours_count(int node1, int node2);
	vector<int> common_neighbours_list(int node1, int node2);


	//get dok
	map<int, map<int, double> > get_dok() { return dok; }
	int get_number_of_nodes() { return number_of_nodes; }
	int get_number_of_edges() { return number_of_edges; }
	vector<int> * get_keys();

	void insert_edge(int src, int tar);
	void insert_node(int node);
	void compute_keys();



protected:
	map<int, map<int,double> > dok;
	int number_of_nodes;
	int number_of_edges;
	vector<int>* keys;


};

