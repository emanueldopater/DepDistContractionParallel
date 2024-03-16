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
	Network(string name);
	~Network();
	void load_edge_list(string filename, bool has_weights);
	void load_node_labels(string filename);

	void print_edge_list();
	void print_neighbours(int node);
	vector<int> neighbours_list(int node);
	void prin_dok();

	int common_neighbours_count(int node1, int node2);
	vector<int> common_neighbours_list(int node1, int node2);

	//get dok
	const map<int, map<int, double> > *  get_dok() { return &dok; }
	int get_number_of_nodes() { return number_of_nodes; }
	int get_number_of_edges() { return number_of_edges; }
	vector<int> * get_keys();

	void insert_edge(int src, int tar);
	void insert_node(int node);
	void compute_keys();

	void print_intern_mapping();

	map<int, string>* get_reverse_mapping() { return &this->reverse_intern_node_id_mapping;  }
	map<int, int> * get_node_labels() { return &this->node_labels; }

	void export_mapping(string filename);
	int get_super_node_id() { return super_node_id; }
	string get_netwotk_name() { return this->name; }

protected:
	map<int, map<int,double> > dok;
	map<string, int> intern_node_id_mapping;
	map<int, string> reverse_intern_node_id_mapping;
	map<int, int> node_labels;
	int number_of_nodes;
	int number_of_edges;
	vector<int>* keys;
	int super_node_id;
	string name;
};

