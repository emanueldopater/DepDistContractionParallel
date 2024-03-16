void iteration(int i)
{
	int super_node_id = this->network->get_super_node_id();

		for (int X = 0; X < NUM_NODES; X++)
		{

			int Y = choose_node(X);


			if (Y == super_node_id)
			{
                M = 0.5 - EMBEDDING_CURRENT_STATE[X]
			}
			else
			{
                M = EMBEDDING_CURRENT_STATE[Y] - EMBEDDING_CURRENT_STATE[X]

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
