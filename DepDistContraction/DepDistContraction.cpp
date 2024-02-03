#include "DepDistContraction.h"

DepDistContraction::DepDistContraction(
	Network* net, 
	Dependency* dep, 
	int embedding_dim,
	double maxDepDist,
	double maxAccDist)
{
	this->network = net;
	this->dependency = dep;
	this->embedding_dim = embedding_dim;
	this->embedding_array_size = embedding_dim * this->network->get_number_of_nodes();

	this->embedding_current_state = new double[embedding_array_size];
	this->embedding_next_state = new double[embedding_array_size];

	this->maxAccDist = maxAccDist;
	this->maxDepDist = maxDepDist;

	// fill embedding_current_state with random values
	mt19937 rng(random_device{}());
	// Create a uniform real distribution to generate numbers between 0 and 1

	//double base_std = 1.0 / this->network->get_number_of_nodes();

	auto net_dok = this->network->get_dok();

	int current_node_index = 0;
	int emb_dim_counter = 0;
	for (int i = 0; i < embedding_array_size; i++)
	{
		// The mean is in 0.5, and standard deviation is base_std * number of neighbours.
		// nodes with higher degree have higher chance of not being in center. 

		//normal_distribution<double> dist_normal(0.5, base_std * net_dok->at(current_node_index).size());
		uniform_real_distribution<double> dist(0.0, 1.0);

		double rand_num = dist(rng);
		int node_degree = net_dok->at(current_node_index).size();
		// random_emb = std::log(1 + random_emb) / std::log(2 + base);
		// // check if random_emb is nan or inf
		// if (std::isnan(random_emb) || std::isinf(random_emb))
		// {
		// 	cout << "random_emb is nan or inf" << endl;
		// }

		// if (rand_num < 0.5)
		// {
		// 	random_emb = 1 - random_emb;
		// }
		//cout << "random_emb: " << random_emb << endl;

		double random_emb = std::pow(rand_num, (1.0/20.0)) * std::pow(0.95, node_degree);
		// in 1/2 chance it will change sign 
		if (rand_num < 0.5)
			random_emb *= -1;

		this->embedding_current_state[i] = random_emb;
		emb_dim_counter++;

		if (emb_dim_counter == embedding_dim)
		{
			emb_dim_counter = 0;
			current_node_index++;
		}
	}

	// uniform_real_distribution<double> dist(0.0, 1.0);

	// for (int i = 0; i < embedding_array_size; i++)
	// {
	// 	this->embedding_current_state[i] = dist(rng);
	// }

}

void DepDistContraction::run(int iterations)
{
	for (int i = 0; i < iterations; i++)
	{
		this->iteration();

		// copy embedding current state to embedding next state with mempcy
		memcpy(this->embedding_current_state, this->embedding_next_state, this->embedding_array_size * sizeof(double));

	}
}

void DepDistContraction::export_gdf(string filename)
{
	ofstream gdf_gile;
	gdf_gile.open(filename);

	auto reverse_mapping = this->network->get_reverse_mapping();

	gdf_gile << "nodedef>name VARCHAR,x DOUBLE,y DOUBLE" << endl;

	// write nodes
	for (int i = 0; i < this->network->get_number_of_nodes(); i++)
	{
		//get reverse mapping of node 
		gdf_gile << reverse_mapping->at(i);
		for (int j = 0; j < this->embedding_dim; j++)
		{
			gdf_gile << "," << this->embedding_current_state[i * this->embedding_dim + j];
		}
		gdf_gile << endl;
	}
	// write edges
	gdf_gile << "edgedef> node1,node2,weight DOUBLE, directed BOOLEAN" << endl;

	for (auto const& x : *(this->network->get_dok()))
	{
		for (auto const& y : x.second)
		{
			gdf_gile << reverse_mapping->at(x.first) << "," << reverse_mapping->at(y.first) << "," << y.second << ",false" << endl;
		}
	}
	gdf_gile.close();
}

void DepDistContraction::export_gdf_node_labels(string filename)
{
	//ofstream gdf_gile;
	//gdf_gile.open(filename);

	//auto reverse_mapping = this->network->get_reverse_mapping();
	//auto node_labels = this->network->get_node_labels();

	//gdf_gile << "nodedef>name VARCHAR, class INTEGER,x DOUBLE,y DOUBLE" << endl;

	//// write nodes
	//for (int i = 0; i < this->network->get_number_of_nodes(); i++)
	//{
	//	//get reverse mapping of node 
	//	gdf_gile << reverse_mapping->at(i);
	//	gdf_gile << "," << node_labels->at(reverse_mapping->at(i));
	//	for (int j = 0; j < this->embedding_dim; j++)
	//	{
	//		gdf_gile << "," << this->embedding_current_state[i * this->embedding_dim + j];
	//	}
	//	gdf_gile << endl;
	//}
	//// write edges
	//gdf_gile << "edgedef> node1,node2,weight DOUBLE, directed BOOLEAN" << endl;

	//for (auto const& x : *(this->network->get_dok()))
	//{
	//	for (auto const& y : x.second)
	//	{
	//		gdf_gile << reverse_mapping->at(x.first) << "," << reverse_mapping->at(y.first) << "," << y.second << ",false" << endl;
	//	}
	//}
	//gdf_gile.close();
}

void DepDistContraction::export_embs_for_CGE(string filename)
{
	ofstream embs_file;
	embs_file.open(filename);

	//auto reverse_mapping = this->network->get_reverse_mapping();

	for (int i = 0; i < this->network->get_number_of_nodes(); i++)
	{
		embs_file << i + 1;
		for (int j = 0; j < this->embedding_dim; j++)
		{
			embs_file << " " << this->embedding_current_state[i * this->embedding_dim + j];
		}
		embs_file << endl;
	}

	embs_file.close();
}

DepDistContraction::~DepDistContraction()
{
	delete [] this->embedding_current_state;
	delete [] this->embedding_next_state;
}

void DepDistContraction::iteration()
{
	#pragma omp parallel
	{

		#pragma omp for
		for (int X = 0; X < this->network->get_number_of_nodes(); X++)
		{
			double* M = new double[this->embedding_dim];
			double* M1 = new double[this->embedding_dim];

			int Y = this->choose_node(X);

			if (Y == -1)
			{
				cout << "Y is -1. " << endl;
                delete[] M;
                delete[] M1;
                continue;
			}

			for (int m = 0; m < this->embedding_dim; m++)
			{

				M[m] = this->embedding_current_state[(this->embedding_dim * Y) + m] - 
					this->embedding_current_state[(this->embedding_dim * X) + m];
			}

			double norm_M = 0.0;
			for (int m = 0; m < this->embedding_dim; m++)
			{
				norm_M += pow(M[m], 2);
			}
			norm_M = sqrt(norm_M);

			for (int m = 0; m < this->embedding_dim; m++)
			{
				M1[m] = M[m] / norm_M;
			}

			double D_x_y = this->dependency->calculate_dependency_lazy(X, Y);
			double D_y_x = this->dependency->calculate_dependency_lazy(Y, X);
			double q = pow(D_x_y, 2) * D_y_x;

			double depDist = (1.0 - q) * this->maxDepDist;
			double accDist = (1.0 - q) * this->maxAccDist;
			double accCoef = 0.5 + 0.5 * norm_M / accDist;
			mt19937 rng(random_device{}());
			uniform_real_distribution<double> dist(0.0, 1.0);
			for (int m = 0; m < this->embedding_dim; m++)
			{
				int emb_index = (this->embedding_dim * X) + m;
				this->embedding_next_state[emb_index] =
					this->embedding_current_state[emb_index] + pow(q, 1.0 / accCoef) * (norm_M - depDist) * M1[m];
			}

			//cout << "Before deleting M and M1" << endl;
			delete[] M;
			delete[] M1;
			//cout << "After deleting M and M1" << endl;

		}

	}	
}

int DepDistContraction::choose_node(int X)
{
	vector<int> all_neighs = this->network->neighbours_list(X);
	if (all_neighs.size() == 0)
	{
		return -1;
	}
	if (all_neighs.size() == 1)
	{
		return all_neighs[0];
	}

	double neighProp = 1.0 - 1.0 / (1 + all_neighs.size());

	mt19937 rng(random_device{}());
	// Create a uniform real distribution to generate numbers between 0 and 1
	uniform_real_distribution<double> dist(0.0, 1.0);

	double rand = dist(rng);

	// random choice from all neighbours

	uniform_int_distribution<int> dist_neighbor(0, all_neighs.size() - 1);
	int random_neig_index = dist_neighbor(rng);
	int selected_neigh = all_neighs[random_neig_index];

	if (rand < neighProp)
	{
		return selected_neigh;
	}

	vector<int> neighs_of_neigh = this->network->neighbours_list(selected_neigh);
	//remove from neighs_of_neigh the node X

	neighs_of_neigh.erase(remove(neighs_of_neigh.begin(), neighs_of_neigh.end(), X), neighs_of_neigh.end());
	if (neighs_of_neigh.size() == 0)
	{
		return selected_neigh;
	}

	// loop through all_neighs
	for (auto neigh : all_neighs)
	{
		// if neigh is in neighs_of_neigh
		if (find(neighs_of_neigh.begin(), neighs_of_neigh.end(), neigh) != neighs_of_neigh.end())
		{
			// remove neigh from neighs_of_neigh
			neighs_of_neigh.erase(remove(neighs_of_neigh.begin(), neighs_of_neigh.end(), neigh), neighs_of_neigh.end());
		}
	}

	if (neighs_of_neigh.size() > 0)
	{
		uniform_int_distribution<int> dist_neigh_neigh(0, neighs_of_neigh.size() - 1);
		int random_neig_index = dist_neigh_neigh(rng);
		int selected_neigh_of_neigh = neighs_of_neigh[random_neig_index];
		return selected_neigh_of_neigh;
	}
	else
	{
		return selected_neigh;
	}
}

bool DepDistContraction::check_whether_to_stop()
{
	for (int current_dim = 0; current_dim < this->embedding_dim; current_dim++)
	{
		double min_value = 999.0;
		double max_value = -1.0;

		for (int emb_idx = 0; emb_idx < this->network->get_number_of_nodes(); emb_idx++)
		{
			double current_emb = this->embedding_current_state[emb_idx * this->embedding_dim + current_dim];

			if (current_emb > max_value)
				max_value = current_emb;
			
			if (current_emb < min_value)
				min_value = current_emb;
		}

		// find the difference
		if ( std::abs(max_value - min_value) > this->maxDepDist)
			return false;
	}

	return true;

}
