#include "Dependency.h"

Dependency::Dependency(Network* net)
{
	this->network = net;
	this->dependency_dok = map<int, map<int, double>>();
	this->network_dok = net->get_dok();
	this->sum_N_dok = map<int, double>();
}

double Dependency::calculate_dependency_lazy(int x, int y)
{
	// check if the dependency is already calculated
	if (this->dependency_dok.find(x) != this->dependency_dok.end())
	{
		if (this->dependency_dok[x].find(y) != this->dependency_dok[x].end())
		{
			return this->dependency_dok[x][y];
		}

		// create the dependency for node1
	}
	if (this->dependency_dok.find(x) == this->dependency_dok.end())
	{
		this->dependency_dok[x] = map<int, double>();
	}

	double weight_xy = 0.0;
	if (this->network_dok->at(x).find(y) != this->network_dok->at(x).end())
	{
		weight_xy = this->network_dok->at(x).at(y);
	}

	vector<int> common_neighbours = this->network->common_neighbours_list(x, y);
	double sum_CN = 0.0;

	sum_CN += weight_xy;

	for (auto v_i : common_neighbours)
	{
		double weight_xvi = this->network_dok->at(x).at(v_i);
		double weight_viy = this->network_dok->at(v_i).at(y);

		double r = weight_viy / (weight_xvi + weight_viy);

		sum_CN += (r * weight_xvi);
	}

	auto N_x = this->network_dok->at(x);
	auto N_y = this->network_dok->at(y);

	double sum_Nx = 0.0;
	double sum_Ny = 0.0;

	// neigbours of x
	if (this->sum_N_dok.find(x) != this->sum_N_dok.end())
	{
		sum_Nx = this->sum_N_dok[x];
	}
	else
	{
		for (auto v_j = N_x.begin(); v_j != N_x.end(); ++v_j)
		{
			sum_Nx += v_j->second;
		}
		this->sum_N_dok[x] = sum_Nx;
	}

	// neigbours of y
	if (this->sum_N_dok.find(y) != this->sum_N_dok.end())
	{
		sum_Ny = this->sum_N_dok[y];
	}
	else
	{
		for (auto v_j = N_y.begin(); v_j != N_y.end(); ++v_j)
		{
			sum_Ny += v_j->second;
		}
		this->sum_N_dok[y] = sum_Ny;
	}

	double D_xy = (sum_CN / sum_Nx);
	double D_yx = (sum_CN / sum_Ny);

	this->dependency_dok[x][y] = D_xy;

	// compute also the dependency for y to x
	this->dependency_dok[y][x] = D_yx;

	return D_xy;

}

map<int, map<int, double>> Dependency::calculate_dependy_whole_net()
{
	//auto keys = network->get_keys();

	//auto dok = this->network->get_dok();
	//auto local_dependency_dok = this->network->get_dok();

	//#pragma omp parallel num_threads(8)
	//{
	//#pragma omp for
	//	for (int i = 0; i < keys->size(); ++i)
	//	{
	//		int node_x_key = keys->at(i);
	//		auto x = dok.find(node_x_key);

	//		for (auto y = x->second.begin(); y != x->second.end(); ++y)
	//		{
	//			double weight_xy = y->second;

	//			vector<int> common_neighbours = this->network->common_neighbours_list(x->first, y->first);
	//			double sum_CN = 0.0;

	//			sum_CN += weight_xy;
	//			for (auto v_i : common_neighbours)
	//			{
	//				double weight_xvi = dok[x->first][v_i];

	//				// compute r
	//				double weight_viy = dok[v_i][y->first];

	//				double r = weight_viy / (weight_xvi + weight_viy);

	//				sum_CN += (r * weight_xvi);
	//			}

	//			auto N_x = x->second;

	//			double sum_N = 0.0;

	//			for (auto v_j = N_x.begin(); v_j != N_x.end(); ++v_j)
	//			{
	//				sum_N += v_j->second;
	//			}

	//			double D_xy = (sum_CN / sum_N);

	//			local_dependency_dok[x->first][y->first] = D_xy;

	//		}
	//	}
	//}

	//return local_dependency_dok;

	return map<int, map<int, double>>();
}
