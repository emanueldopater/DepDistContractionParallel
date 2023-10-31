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
	void export_gdf(string filename);
	void export_gdf_node_labels(string filename);
	void export_embs_for_CGE(string filename);


	~DepDistContraction();
private:
	void iteration();
	int choose_node(int X);
	Network * network;
	Dependency * dependency;
	int embedding_dim;
	double* embedding_current_state;
	double* embedding_next_state;
	double maxDepDist;
	double maxAccDist;

	int embedding_array_size;

};

