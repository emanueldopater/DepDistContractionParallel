#pragma once
#include "Network.h"
#include "Dependency.h"
#include <random>
#include <omp.h>
using namespace std;

class DepDistContraction
{
public:
	DepDistContraction(
		Network * net,
		Dependency * dep, 
		int embedding_dim, 
		double maxDepDist,
		double maxAccDist);

	void run(int iterations);
	void run_automatic_stop();
	void export_gdf(string filename);
	void export_gdf_node_labels(string filename);
	void export_embs_for_CGE(string filename);


	~DepDistContraction();
private:
	void iteration(int i);
	int choose_node(int X);
	bool check_whether_to_stop();
	bool check_whether_to_stop2();
	void random_embedding_initialization();
	void big_nodes_in_center_embedding_initialization();
	void big_nodes_on_edges_embedding_initialization();
	Network * network;
	Dependency * dependency;
	int embedding_dim;
	double* embedding_current_state;
	double* embedding_next_state;
	double maxDepDist;
	double maxAccDist;

	int embedding_array_size;
	vector<double> prev_diffs;

};

