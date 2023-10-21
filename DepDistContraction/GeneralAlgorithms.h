#pragma once
#include "Network.h"
#include <omp.h>
#include <stdio.h>
class GeneralAlgorithms
{
private:
	Network * network;

	public:
		GeneralAlgorithms(Network * network);
		vector<double> average_maximum_degree();
		void degree_distribution(string file_name);
		void average_maximum_common_neighbours();
		void clustering_coefficient_and_distribution(string fileName);
		void calculate_dependency();

		double average_clustering_coefficient();
		double average_weighted_degree();



};

