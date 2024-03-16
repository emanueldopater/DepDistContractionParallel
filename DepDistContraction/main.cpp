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
	omp_set_num_threads(11);

	Network * mouse_network = new Network("mouse_brain");
	Network * airports_network = new Network("airports");
	//Network * github_network = new Network();
	Network * emails_network = new Network("emails");
	Network * ca_grqc_network = new Network("ca_grqc");
	Network * ca_hepth_network = new Network("ca_hepth");
	Network * facebook_combined_network = new Network("facebook_combined");
	Network * p2p_gnutella08_network = new Network("p2p_gnutella08");
	Network * lastfm_asia_network = new Network("lastfm_asia");


	// ddc networks


	// mouse_network->load_edge_list("nets_from_article/mouse_brain.csv", false);
	// airports_network->load_edge_list("nets_from_article/airports.csv", false);
	// //github_network->load_edge_list("nets_from_article/github.csv", false);
	// emails_network->load_edge_list("nets_from_article/emails.csv", false);

	ca_grqc_network->load_edge_list("DDC_Networks/ca_grqc.csv", false);
	ca_hepth_network->load_edge_list("DDC_Networks/ca_hepth.csv", false);
	// facebook_combined_network->load_edge_list("DDC_Networks/facebook_combined.csv", false);
	// p2p_gnutella08_network->load_edge_list("DDC_Networks/p2p_gnutella08.csv", false);
	// lastfm_asia_network->load_edge_list("DDC_Networks/lastfm_asia.csv", false);



	// export mapping 
	// mouse_network->export_mapping("results/nodes_mapping/mouse_brain_mapping.csv");
	// airports_network->export_mapping("results/nodes_mapping/airports_mapping.csv");
	// //github_network->export_mapping("results/nodes_mapping/github_mapping.csv");
	// emails_network->export_mapping("results/nodes_mapping/emails_mapping.csv");

	ca_grqc_network->export_mapping("results/nodes_mapping/ca_grqc_mapping.csv");
	ca_hepth_network->export_mapping("results/nodes_mapping/ca_hepth_mapping.csv");
	// facebook_combined_network->export_mapping("results/nodes_mapping/facebook_combined_mapping.csv");
	// p2p_gnutella08_network->export_mapping("results/nodes_mapping/p2p_gnutella08_mapping.csv");
	// lastfm_asia_network->export_mapping("results/nodes_mapping/lastfm_asia_mapping.csv");

	Dependency* mouse_dependency = new Dependency(mouse_network);
	Dependency* airports_dependency = new Dependency(airports_network);
	//Dependency* github_dependency = new Dependency(github_network);
	Dependency* emails_dependency = new Dependency(emails_network);

	Dependency* ca_grqc_dependency = new Dependency(ca_grqc_network);
	Dependency* ca_hepth_dependency = new Dependency(ca_hepth_network);
	// Dependency* facebook_combined_dependency = new Dependency(facebook_combined_network);
	// Dependency* p2p_gnutella08_dependency = new Dependency(p2p_gnutella08_network);
	// Dependency* lastfm_asia_dependency = new Dependency(lastfm_asia_network);


	vector<int> emb_dims = { 4, 8, 16, 32, 64, 128 };

	string embs_path = "/embs_BE_N_q_swap";
	string result_path = "/result_BE_N_q_swap";

	for (int i = 0; i < emb_dims.size(); i++)
	{
		cout << "dim: " << emb_dims[i] << endl;

		// std::__fs::filesystem::create_directory("results/airports/dim_" + to_string(emb_dims[i]));
		// std::__fs::filesystem::create_directory("results/airports/dim_" + to_string(emb_dims[i]) + embs_path);
		// std::__fs::filesystem::create_directory("results/airports/dim_" + to_string(emb_dims[i]) + result_path);

		// std::__fs::filesystem::create_directory("results/github/dim_" + to_string(emb_dims[i]));
		// std::__fs::filesystem::create_directory("results/github/dim_" + to_string(emb_dims[i]) + embs_path);
		// std::__fs::filesystem::create_directory("results/github/dim_" + to_string(emb_dims[i]) + result_path);

		// std::__fs::filesystem::create_directory("results/emails/dim_" + to_string(emb_dims[i]));
		// std::__fs::filesystem::create_directory("results/emails/dim_" + to_string(emb_dims[i]) + embs_path);
		// std::__fs::filesystem::create_directory("results/emails/dim_" + to_string(emb_dims[i]) + result_path);

		// std::__fs::filesystem::create_directory("results/mouse_brain/dim_" + to_string(emb_dims[i]));
		// std::__fs::filesystem::create_directory("results/mouse_brain/dim_" + to_string(emb_dims[i]) + embs_path);
		// std::__fs::filesystem::create_directory("results/mouse_brain/dim_" + to_string(emb_dims[i]) + result_path);
		std::__fs::filesystem::create_directory("results/ca_grqc/");
		std::__fs::filesystem::create_directory("results/ca_grqc/dim_" + to_string(emb_dims[i]));
		std::__fs::filesystem::create_directory("results/ca_grqc/dim_" + to_string(emb_dims[i]) + embs_path);
		std::__fs::filesystem::create_directory("results/ca_grqc/dim_" + to_string(emb_dims[i]) + result_path);

		std::__fs::filesystem::create_directory("results/ca_hepth/");
		std::__fs::filesystem::create_directory("results/ca_hepth/dim_" + to_string(emb_dims[i]));
		std::__fs::filesystem::create_directory("results/ca_hepth/dim_" + to_string(emb_dims[i]) + embs_path);
		std::__fs::filesystem::create_directory("results/ca_hepth/dim_" + to_string(emb_dims[i]) + result_path);

		// std::__fs::filesystem::create_directory("results/facebook_combined/");
		// std::__fs::filesystem::create_directory("results/facebook_combined/dim_" + to_string(emb_dims[i]));
		// std::__fs::filesystem::create_directory("results/facebook_combined/dim_" + to_string(emb_dims[i]) + embs_path);
		// std::__fs::filesystem::create_directory("results/facebook_combined/dim_" + to_string(emb_dims[i]) + result_path);

		// std::__fs::filesystem::create_directory("results/p2p_gnutella08/");
		// std::__fs::filesystem::create_directory("results/p2p_gnutella08/dim_" + to_string(emb_dims[i]));
		// std::__fs::filesystem::create_directory("results/p2p_gnutella08/dim_" + to_string(emb_dims[i]) + embs_path);
		// std::__fs::filesystem::create_directory("results/p2p_gnutella08/dim_" + to_string(emb_dims[i]) + result_path);

		// std::__fs::filesystem::create_directory("results/lastfm_asia/");
		// std::__fs::filesystem::create_directory("results/lastfm_asia/dim_" + to_string(emb_dims[i]));
		// std::__fs::filesystem::create_directory("results/lastfm_asia/dim_" + to_string(emb_dims[i]) + embs_path);
		// std::__fs::filesystem::create_directory("results/lastfm_asia/dim_" + to_string(emb_dims[i]) + result_path);

		for (int y = 0; y < 10; y++)
		{
			cout << "iter: " << y << endl;

			// DepDistContraction* mouse_ddc = new DepDistContraction(
			// 	mouse_network,
			// 	mouse_dependency,
			// 	emb_dims[i],
			// 	0.002,
			// 	0.01
			// );

			// DepDistContraction* airport_ddc = new DepDistContraction(
			// 	airports_network,
			// 	airports_dependency,
			// 	emb_dims[i],
			// 	0.002,
			// 	0.01
			// );

			// // DepDistContraction* github_ddc = new DepDistContraction(
			// // 	github_network,
			// // 	github_dependency,
			// // 	emb_dims[i],
			// // 	0.002,
			// // 	0.01
			// // );

			// DepDistContraction* emails_ddc = new DepDistContraction(
			// 	emails_network,
			// 	emails_dependency,
			// 	emb_dims[i],
			// 	0.002,
			// 	0.01
			// );

			DepDistContraction* ca_grqc_ddc = new DepDistContraction(
				ca_grqc_network,
				ca_grqc_dependency,
				emb_dims[i],
				0.002,
				0.01
			);

			DepDistContraction* ca_hepth_ddc = new DepDistContraction(
				ca_hepth_network,
				ca_hepth_dependency,
				emb_dims[i],
				0.002,
				0.01
			);

			// DepDistContraction* facebook_combined_ddc = new DepDistContraction(
			// 	facebook_combined_network,
			// 	facebook_combined_dependency,
			// 	emb_dims[i],
			// 	0.002,
			// 	0.01
			// );

			// DepDistContraction* p2p_gnutella08_ddc = new DepDistContraction(
			// 	p2p_gnutella08_network,
			// 	p2p_gnutella08_dependency,
			// 	emb_dims[i],
			// 	0.002,
			// 	0.01
			// );

			// DepDistContraction* lastfm_asia_ddc = new DepDistContraction(
			// 	lastfm_asia_network,
			// 	lastfm_asia_dependency,
			// 	emb_dims[i],
			// 	0.002,
			// 	0.01
			// );

			// mouse_ddc->run(500);
			// airport_ddc->run(500);
			// // github_ddc->run(500);
			// emails_ddc->run(500);

			ca_grqc_ddc->run(500);
			ca_hepth_ddc->run(500);
			// facebook_combined_ddc->run(500);
			// p2p_gnutella08_ddc->run(500);
			// lastfm_asia_ddc->run(500);

			// mouse_ddc->run_automatic_stop();
			// airport_ddc->run_automatic_stop();
			// //github_ddc->run(500);
			// emails_ddc->run_automatic_stop();

			// mouse_ddc->export_embs_for_CGE("results/mouse_brain/dim_" + to_string(emb_dims[i]) + embs_path +"/emb_" + to_string(y) +".embs");
			// //github_ddc->export_embs_for_CGE("results/github/dim_" + to_string(emb_dims[i]) + "/embs/emb_" + to_string(y) + ".embs");
			// airport_ddc->export_embs_for_CGE("results/airports/dim_" + to_string(emb_dims[i]) + embs_path +"/emb_" + to_string(y) + ".embs");
			// emails_ddc->export_embs_for_CGE("results/emails/dim_" + to_string(emb_dims[i]) + embs_path+"/emb_" + to_string(y) + ".embs");

			ca_grqc_ddc->export_embs_for_CGE("results/ca_grqc/dim_" + to_string(emb_dims[i]) + embs_path + "/emb_" + to_string(y) + ".embs");
			ca_hepth_ddc->export_embs_for_CGE("results/ca_hepth/dim_" + to_string(emb_dims[i]) + embs_path + "/emb_" + to_string(y) + ".embs");
			// facebook_combined_ddc->export_embs_for_CGE("results/facebook_combined/dim_" + to_string(emb_dims[i]) + embs_path + "/emb_" + to_string(y) + ".embs");
			// p2p_gnutella08_ddc->export_embs_for_CGE("results/p2p_gnutella08/dim_" + to_string(emb_dims[i]) + embs_path + "/emb_" + to_string(y) + ".embs");
			// lastfm_asia_ddc->export_embs_for_CGE("results/lastfm_asia/dim_" + to_string(emb_dims[i]) + embs_path + "/emb_" + to_string(y) + ".embs");

			// delete mouse_ddc;
			// delete airport_ddc;
			// //delete github_ddc;
			// delete emails_ddc;

			delete ca_grqc_ddc;
			delete ca_hepth_ddc;
			// delete facebook_combined_ddc;
			// delete p2p_gnutella08_ddc;
			// delete lastfm_asia_ddc;
		}
	}
}