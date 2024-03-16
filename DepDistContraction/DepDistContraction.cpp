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

	// intialize prev_diffs for automatic stopping each dimension to high number
	this->prev_diffs = vector<double>(embedding_dim, 999999.0);

	//this->maxDepDist = maxDepDist;
	//this->maxAccDist = maxAccDist;

	this->maxDepDist = 0.01 / std::pow(this->network->get_number_of_nodes(), 1.0 / this->embedding_dim);
	
	this->maxAccDist = 10.0 * this->maxDepDist;
	//this->random_embedding_initialization();
	//this->big_nodes_in_center_embedding_initialization();
	this->big_nodes_on_edges_embedding_initialization();

}

void DepDistContraction::run(int iterations)
{
	for (int i = 0; i < iterations; i++)
	{
		this->iteration(i);
		//cout << "Iteration: " << i << endl;
		// copy embedding current state to embedding next state with mempcy
		memcpy(this->embedding_current_state, this->embedding_next_state, this->embedding_array_size * sizeof(double));
		// if (this->check_whether_to_stop())
		// 	break;;

	}
}

void DepDistContraction::run_automatic_stop()
{
	while(true)
	{
		this->iteration(0);
		// copy embedding current state to embedding next state with mempcy
		memcpy(this->embedding_current_state, this->embedding_next_state, this->embedding_array_size * sizeof(double));
		if (this->check_whether_to_stop2())
			break;

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
	gdf_gile << "9999, 0.5, 0.5" << endl;

	// write edges
	gdf_gile << "edgedef> node1,node2,weight DOUBLE, directed BOOLEAN" << endl;

	for (auto const& x : *(this->network->get_dok()))
	{
		// if (x.first == this->network->get_super_node_id())
		// 	continue;
		for (auto const& y : x.second)
		{
			// if (y.first == this->network->get_super_node_id())
			// 	continue;

			string x_name;
			string y_name;
			double weight = 1.0;

			if (x.first == this->network->get_super_node_id())
			{
				x_name = "9999";
				weight = 0.1;
			}
			else
				x_name = reverse_mapping->at(x.first);

			if (y.first == this->network->get_super_node_id())
			{
				y_name = "9999";
				weight = 0.1;
			}
			else
				y_name = reverse_mapping->at(y.first);

			gdf_gile << x_name << "," << y_name << "," << weight << ",false" << endl;
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

void DepDistContraction::iteration(int i)
{
	int super_node_id = this->network->get_super_node_id();
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
			if (Y == super_node_id)
			{
				for (int m = 0; m < this->embedding_dim; m++)
				{
					M[m] = 0.5 - this->embedding_current_state[(this->embedding_dim * X) + m];
				}
			}
			else
			{
				for (int m = 0; m < this->embedding_dim; m++)
				{
					M[m] = this->embedding_current_state[(this->embedding_dim * Y) + m] - 
						this->embedding_current_state[(this->embedding_dim * X) + m];
				}
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
			double q;
			if (i < 100)
			{
				q = D_x_y * D_y_x * ((D_x_y + D_y_x) / 2.0);
			}
			else
			{
				q = pow(D_x_y, 2) * D_y_x;
			}

            //q = min(.99, max(D_x_y, D_y_x) ** 3)
			//q = std::min(0.99, std::pow(std::max(D_x_y, D_y_x), 3));

			double depDist = (1.0 - q) * this->maxDepDist;
			double accDist = (1.0 - q) * this->maxAccDist;
			double accCoef = 0.5 + 0.5 * norm_M / accDist;

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

	//double neighProp = 1.0 - 1.0 / (1 + all_neighs.size());
	double neighProp = 0.0;


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
	bool should_end = false;
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

		double current_emb_diff = std::abs(max_value - min_value);
		if (std::abs(current_emb_diff - this->prev_diffs[current_dim]) < this->maxDepDist)
		{
			should_end = true;
		}
		else
		{
			should_end = false;
		}

		this->prev_diffs[current_dim] = current_emb_diff;

	}

	return should_end;

}

bool DepDistContraction::check_whether_to_stop2()
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

		double current_emb_diff = std::abs(max_value - min_value);

		if (current_emb_diff < 0.1)
			return true;
	}

	return false;
}

void DepDistContraction::random_embedding_initialization()
{
	mt19937 rng(random_device{}());
	uniform_real_distribution<double> dist(0.0, 1.0);

	for (int i = 0; i < embedding_array_size; i++)
	{
		this->embedding_current_state[i] = dist(rng);
	}
}

void DepDistContraction::big_nodes_on_edges_embedding_initialization()
{
	mt19937 rng(random_device{}());
	uniform_real_distribution<double> dist(0.0, 1.0);

	auto net_dok = this->network->get_dok();

	int current_node_index = 0;
	int emb_dim_counter = 0;
	for (int i = 0; i < embedding_array_size; i++)
	{
		int node_degree = net_dok->at(current_node_index).size();

		// double random_emb = std::pow(rand_num, (1.0/20.0)) * std::pow(0.95, node_degree);
		// // in 1/2 chance it will change sign 
		// if (rand_num < 0.5)
		// 	random_emb *= -1;

		int DEG = net_dok->at(current_node_index).size();
		double interval = 0.5 * (std::log(1 + std::sqrt(1.0 / (1.0 + DEG))) / std::log(2));
		uniform_real_distribution<double> dist_interval(0.0, interval);

		double number_from_interval = dist_interval(rng);
		double rand_num = dist(rng);
		if (rand_num < 0.5)
			this->embedding_current_state[i] =	number_from_interval;
		else
			this->embedding_current_state[i] = 1.0 - number_from_interval;

		emb_dim_counter++;

		if (emb_dim_counter == embedding_dim)
		{
			emb_dim_counter = 0;
			current_node_index++;
		}
	}
}

void DepDistContraction::big_nodes_in_center_embedding_initialization()
{
	mt19937 rng(random_device{}());
	uniform_real_distribution<double> dist(0.0, 1.0);

	auto net_dok = this->network->get_dok();

	int current_node_index = 0;
	int emb_dim_counter = 0;
	for (int i = 0; i < embedding_array_size; i++)
	{
		int node_degree = net_dok->at(current_node_index).size();

		// double random_emb = std::pow(rand_num, (1.0/20.0)) * std::pow(0.95, node_degree);
		// // in 1/2 chance it will change sign 
		// if (rand_num < 0.5)
		// 	random_emb *= -1;

		int DEG = net_dok->at(current_node_index).size();
		double interval = 0.5 * (std::log(1 + std::sqrt(1.0 / (1.0 + DEG))) / std::log(2));
		uniform_real_distribution<double> dist_interval(0.0, interval);

		double number_from_interval = dist_interval(rng);
		double rand_num = dist(rng);
		if (rand_num < 0.5)
			number_from_interval *= -1;

		this->embedding_current_state[i] = 0.5 + number_from_interval;
		emb_dim_counter++;

		if (emb_dim_counter == embedding_dim)
		{
			emb_dim_counter = 0;
			current_node_index++;
		}
	}

}
