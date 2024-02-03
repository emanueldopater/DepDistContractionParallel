#include <iostream>
#include "Network.h"
#include "Dependency.h"
#include "DepDistContraction.h"
#include <random>
#include <omp.h>
#include <filesystem>


using namespace std;

int main()
{
	
	//omp_set_num_threads(11);
	o

	/*Network * karate_network = new Network();
	Network * lesmis_network = new Network();
	Network * football_network = new Network();
	Network * netscience_network = new Network();
	Network * condmat_network = new Network();
	Network * citeseer_network = new Network();*/
	Network * mouse_network = new Network();
	Network * airports_network = new Network();
	//Network * github_network = new Network();
	Network * emails_network = new Network();
	
	// first parameter is path, second is if the edges has weights
	/*karate_network->load_edge_list("datasets/karateclub_edges.csv", false);
	lesmis_network->load_edge_list("datasets/lesmis_edges.csv", true);
	football_network->load_edge_list("datasets/football_edges.csv", false);
	netscience_network->load_edge_list("datasets/ca-netscience_edges.csv", false);*/

	mouse_network->load_edge_list("nets_from_article/mouse_brain.csv", false);
	airports_network->load_edge_list("nets_from_article/airports.csv", false);
	//github_network->load_edge_list("nets_from_article/github.csv", false);
	emails_network->load_edge_list("nets_from_article/emails.csv", false);

	// export mapping 
	mouse_network->export_mapping("results/nodes_mapping/mouse_brain_mapping.csv");
	airports_network->export_mapping("results/nodes_mapping/airports_mapping.csv");
	//github_network->export_mapping("results/nodes_mapping/github_mapping.csv");
	emails_network->export_mapping("results/nodes_mapping/emails_mapping.csv");

	//condmat_network->load_edge_list("datasets/condmat_edges.csv", true);
	//citeseer_network->load_edge_list("datasets/citeseer.edges", true);
	//citeseer_network->load_node_labels("datasets/citeseer_node_labels.csv");

	Dependency* mouse_dependency = new Dependency(mouse_network);
	Dependency* airports_dependency = new Dependency(airports_network);
	//Dependency* github_dependency = new Dependency(github_network);
	Dependency* emails_dependency = new Dependency(emails_network);

	vector<int> emb_dims = { 4, 8, 16, 32, 64, 128 };

	for (int i = 0; i < emb_dims.size(); i++)
	{
		cout << "dim: " << emb_dims[i] << endl;

		std::__fs::filesystem::create_directory("results/airports/dim_" + to_string(emb_dims[i]));
		std::__fs::filesystem::create_directory("results/airports/dim_" + to_string(emb_dims[i]) + "/embs_high_in_middle");
		std::__fs::filesystem::create_directory("results/airports/dim_" + to_string(emb_dims[i]) + "/results_high_in_middle");

		std::__fs::filesystem::create_directory("results/github/dim_" + to_string(emb_dims[i]));
		std::__fs::filesystem::create_directory("results/github/dim_" + to_string(emb_dims[i]) + "/embs_high_in_middle");
		std::__fs::filesystem::create_directory("results/github/dim_" + to_string(emb_dims[i]) + "/results_high_in_middle");

		std::__fs::filesystem::create_directory("results/emails/dim_" + to_string(emb_dims[i]));
		std::__fs::filesystem::create_directory("results/emails/dim_" + to_string(emb_dims[i]) + "/embs_high_in_middle");
		std::__fs::filesystem::create_directory("results/emails/dim_" + to_string(emb_dims[i]) + "/results_high_in_middle");

		std::__fs::filesystem::create_directory("results/mouse_brain/dim_" + to_string(emb_dims[i]));
		std::__fs::filesystem::create_directory("results/mouse_brain/dim_" + to_string(emb_dims[i]) + "/embs_high_in_middle");
		std::__fs::filesystem::create_directory("results/mouse_brain/dim_" + to_string(emb_dims[i]) + "/results_high_in_middle");

		for (int y = 0; y < 30; y++)
		{
			cout << "iter: " << y << endl;

			DepDistContraction* mouse_ddc = new DepDistContraction(
				mouse_network,
				mouse_dependency,
				emb_dims[i],
				0.002,
				0.01
			);

			DepDistContraction* airport_ddc = new DepDistContraction(
				airports_network,
				airports_dependency,
				emb_dims[i],
				0.002,
				0.01
			);

			// DepDistContraction* github_ddc = new DepDistContraction(
			// 	github_network,
			// 	github_dependency,
			// 	emb_dims[i],
			// 	0.002,
			// 	0.01
			// );

			DepDistContraction* emails_ddc = new DepDistContraction(
				emails_network,
				emails_dependency,
				emb_dims[i],
				0.002,
				0.01
			);


			mouse_ddc->run(500);
			airport_ddc->run(500);
			//github_ddc->run(500);
			emails_ddc->run(500);

			mouse_ddc->export_embs_for_CGE("results/mouse_brain/dim_" + to_string(emb_dims[i]) + "/embs_high_in_middle/emb_" + to_string(y) +".embs");
			//github_ddc->export_embs_for_CGE("results/github/dim_" + to_string(emb_dims[i]) + "/embs/emb_" + to_string(y) + ".embs");
			airport_ddc->export_embs_for_CGE("results/airports/dim_" + to_string(emb_dims[i]) + "/embs_high_in_middle/emb_" + to_string(y) + ".embs");
			emails_ddc->export_embs_for_CGE("results/emails/dim_" + to_string(emb_dims[i]) + "/embs_high_in_middle/emb_" + to_string(y) + ".embs");

			delete mouse_ddc;
			delete airport_ddc;
			//delete github_ddc;
			delete emails_ddc;
		}
	}
}