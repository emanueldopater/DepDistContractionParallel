#include "GeneralAlgorithms.h"

GeneralAlgorithms::GeneralAlgorithms(Network* network)
{
	this->network = network;
}

vector<double> GeneralAlgorithms::average_maximum_degree()
{
	auto dok = this->network->get_dok();

	int global_max_degree = 0;
		
	auto keys = this->network->get_keys();

	#pragma omp parallel num_threads(8)
	{
		int local_max_degree = 0;

		#pragma omp for
		for (int i = 0; i < keys->size(); ++i)
		{
			int node_it_key = keys->at(i);
			auto node_it = dok.find(node_it_key);

			if (node_it->second.size() > local_max_degree)
			{
				local_max_degree = node_it->second.size();
			}
		}

		#pragma omp critical
		{
			if (local_max_degree > global_max_degree)
			{
				global_max_degree = local_max_degree;
			}
		}
	}


	vector<double> results;
	/*cout << "Max degree: " << global_max_degree << endl;
	cout << "Number of nodes: " << network->get_number_of_nodes() << endl;
	cout << "Number of edges: " << network->get_number_of_edges() << endl;
	cout << "Average degree: " <<  network->get_number_of_edges() / network->get_number_of_nodes()  << endl;*/

	results.push_back(global_max_degree);
	results.push_back(network->get_number_of_nodes());
	results.push_back(network->get_number_of_edges());
	results.push_back(network->get_number_of_edges() / network->get_number_of_nodes());

	return results;
	
}

void GeneralAlgorithms::degree_distribution(string file_name)
{
	std::map<int, int> degree_distribution;

	auto dok = this->network->get_dok();

	std::vector<int> keys;
	for (const auto& pair : dok) {
		keys.push_back(pair.first);
	}


	#pragma omp parallel num_threads(8)
	{

		#pragma omp for
		for (int i = 0; i < keys.size(); ++i)
		{
			int node_it_key = keys[i];
			auto node_it = dok.find(node_it_key);

			#pragma omp critical
			{
				degree_distribution[node_it->second.size()]++;
			}
		}
	}


	// write to file
	ofstream myfile;
	myfile.open(file_name);
	myfile << "degree" << " " << "num_of_nodes" << endl;
	for (auto const& degree : degree_distribution)
	{
		myfile << degree.first << " " << degree.second << endl;
	}
	myfile.close();
	
}

void GeneralAlgorithms::average_maximum_common_neighbours()
{
	auto dok = this->network->get_dok();

	int max_common_neighbours = 0;
	int sum_common_neighbours = 0;
	int count = 0;


	auto keys = network->get_keys();


	// Define private variables for each thread
	int private_max_common_neighbours = 0;
	int private_sum_common_neighbours = 0;
	int private_count = 0;

	#pragma omp parallel num_threads(8)
	{
		// Private variables for each thread
		int local_max_common_neighbours = 0;
		int local_sum_common_neighbours = 0;
		int local_count = 0;

		#pragma omp for
		for (int i = 0; i < keys->size(); ++i)
		{
			int node_it_1_key = keys->at(i);
			auto node_it_1 = dok.find(node_it_1_key);

			for (int j = i+1; j < keys->size(); ++j)
			{
				int node_it_2_key = keys->at(j);
				auto node_it_2 = dok.find(node_it_2_key);

				if (node_it_1->first == node_it_2->first)
				{
					continue;
				}

				int common_neighbours = this->network->common_neighbours_count(node_it_1->first, node_it_2->first);

				// Accumulate local results
				local_max_common_neighbours = std::max(local_max_common_neighbours, common_neighbours);
				local_sum_common_neighbours += common_neighbours;
				local_count++;
			}
		}

		// Merge local results into private variables
		#pragma omp critical
		{
			private_max_common_neighbours = std::max(private_max_common_neighbours, local_max_common_neighbours);
			private_sum_common_neighbours += local_sum_common_neighbours;
			private_count += local_count;
		}
	}

	max_common_neighbours = private_max_common_neighbours;
	sum_common_neighbours = private_sum_common_neighbours;
	count = private_count;


	cout << "Max common neighbours: " << max_common_neighbours << endl;
	cout << "Average common neighbours: " << sum_common_neighbours / count << endl;
}

void GeneralAlgorithms::clustering_coefficient_and_distribution(string fileName)
{
	//compute clustering coefficient
	auto dok = this->network->get_dok();

	auto keys = network->get_keys();
	map<int, int> degree_countin;
	map<int, double> clustering_coefficient_sum;

	// Define private variables for each thread
	#pragma omp parallel num_threads(8)
	{

		#pragma omp for
		for (int i = 0; i < keys->size(); ++i)
		{
			int node_it_key = keys->at(i);
			auto node_it = dok.find(node_it_key);

			int degree = node_it->second.size();

			int number_of_possible_edges = degree * (degree - 1) / 2;

			auto neighbours = node_it->second;

			int number_of_existing_edges = 0;

			for (auto neighbour_it = neighbours.begin(); neighbour_it != neighbours.end(); ++neighbour_it)
			{
				for (auto neighbour_it_2 = neighbours.begin(); neighbour_it_2 != neighbours.end(); ++neighbour_it_2)
				{
					if (neighbour_it->first == neighbour_it_2->first)
					{
						continue;
					}

					if (dok[neighbour_it->first].find(neighbour_it_2->first) != dok[neighbour_it->first].end())
					{
						number_of_existing_edges++;
					}
				}
			}
			double clustering_coefficient = 0.0;

			if (number_of_possible_edges != 0)
			{
				clustering_coefficient = (double)(2 * number_of_existing_edges) / number_of_possible_edges;
			}
			//cout << "Clustering coeff: " << clustering_coefficient << endl;

			#pragma omp critical
			{

				int degree = node_it->second.size();
				if (degree_countin.find(degree) == degree_countin.end())
				{
					degree_countin[degree] = 0;
				}
				degree_countin[degree] += 1;


				if (clustering_coefficient_sum.find(degree) == clustering_coefficient_sum.end())
				{
					clustering_coefficient_sum[degree] = 0.0;
				}
				clustering_coefficient_sum[degree] += clustering_coefficient;

			}

		}

	}

	for (auto const& degree_count : degree_countin)
	{
		int degree = degree_count.second;
		if (degree != 0.0)
		{
			clustering_coefficient_sum[degree_count.first] /= degree_countin[degree_count.first];
		}
		else
		{
			clustering_coefficient_sum[degree_count.first] = 0.0;
		}
	}

	// write to file
	ofstream myfile;
	myfile.open(fileName);
	myfile << "degree," << "avg_coeff" << endl;
	for (auto const& cc : clustering_coefficient_sum)
	{
		myfile << cc.first << " " <<  cc.second << endl;
	}
	myfile.close();

}

void GeneralAlgorithms::calculate_dependency()
{

	auto keys = network->get_keys();


	auto dok = this->network->get_dok();
	auto dependency_dok = this->network->get_dok();


	#pragma omp parallel num_threads(8)
	{

		#pragma omp for
		for (int i = 0; i < keys->size(); ++i)
		{
			int node_x_key = keys->at(i);
			auto x = dok.find(node_x_key);

			for (auto y = x->second.begin(); y != x->second.end(); ++y)
			{
				double weight_xy = y->second;

				vector<int> common_neighbours = this->network->common_neighbours_list(x->first, y->first);
				double sum_CN = 0.0;

				sum_CN += weight_xy;
				for (auto v_i : common_neighbours)
				{
					double weight_xvi = dok[x->first][v_i];

					// compute r
					double weight_viy = dok[v_i][y->first];

					double r = weight_viy / (weight_xvi + weight_viy);

					sum_CN += (r * weight_xvi);
				}

				auto N_x = x->second;

				double sum_N = 0.0;

				for (auto v_j = N_x.begin(); v_j != N_x.end(); ++v_j)
				{
					sum_N += v_j->second;
				}

				double D_xy = (sum_CN / sum_N);

				dependency_dok[x->first][y->first] = D_xy;

			}

		}

	}

	// print dependency matrix

	//for (auto const& x : dependency_dok)
	//{
	//	for (auto const& y : x.second)
	//	{
	//		cout << x.first << "->" << y.first << ":" << y.second << endl;
	//	}
	//	cout << endl;
	//}




}

double GeneralAlgorithms::average_clustering_coefficient()
{
	//compute clustering coefficient
	auto dok = this->network->get_dok();

	auto keys = network->get_keys();

	double global_sum_clustering_coefficient = 0.0;

	// Define private variables for each thread
	#pragma omp parallel num_threads(8)
	{
		double local_sum_clustering_coefficient = 0.0;

		#pragma omp for
		for (int i = 0; i < keys->size(); ++i)
		{
			int node_it_key = keys->at(i);
			auto node_it = dok.find(node_it_key);

			int degree = node_it->second.size();

			int number_of_possible_edges = degree * (degree - 1) / 2;

			auto neighbours = node_it->second;

			int number_of_existing_edges = 0;

			for (auto neighbour_it = neighbours.begin(); neighbour_it != neighbours.end(); ++neighbour_it)
			{
				for (auto neighbour_it_2 = neighbours.begin(); neighbour_it_2 != neighbours.end(); ++neighbour_it_2)
				{
					if (neighbour_it->first == neighbour_it_2->first)
					{
						continue;
					}

					if (dok[neighbour_it->first].find(neighbour_it_2->first) != dok[neighbour_it->first].end())
					{
						number_of_existing_edges++;
					}
				}
			}
			double clustering_coefficient = 0.0;

			if (number_of_possible_edges != 0)
			{
				clustering_coefficient = (double)(number_of_existing_edges) / number_of_possible_edges;
			}

			local_sum_clustering_coefficient += clustering_coefficient;
		}

		#pragma omp critical
		{
			global_sum_clustering_coefficient += local_sum_clustering_coefficient;
		}

	}

	return global_sum_clustering_coefficient / this->network->get_number_of_nodes();
}

double GeneralAlgorithms::average_weighted_degree()
{
	auto dok = this->network->get_dok();

	int global_max_degree = 0;

	auto keys = network->get_keys();
	
	double global_sum_weighted_degree = 0.0;

	#pragma omp parallel num_threads(8)
	{
		int local_sum_weighted_degree = 0;

		#pragma omp for
		for (int i = 0; i < dok.size(); ++i)
		{
			int node_it_key = keys->at(i);
			auto node_it = dok.find(node_it_key);

			// loop through neighbours
			for (auto neighbour_it = node_it->second.begin(); neighbour_it != node_it->second.end(); ++neighbour_it)
			{
				local_sum_weighted_degree += neighbour_it->second;
			}

		}

		#pragma omp critical
		{
			global_max_degree += local_sum_weighted_degree;
		}
	}


	return global_max_degree / this->network->get_number_of_nodes();

}

