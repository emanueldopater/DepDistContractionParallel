#include <iostream>
#include "Network.h"
#include "GeneralAlgorithms.h"
#include "Dependency.h"
#include "DepDistContraction.h"
#include <random>
#include <omp.h>


using namespace std;

int main()
{
	/*Network * karate_network = new Network();
	Network * lesmis_network = new Network();
	Network * football_network = new Network();
	Network * netscience_network = new Network();
	Network * condmat_network = new Network();
	Network * citeseer_network = new Network();*/
	Network * mouse_network = new Network();
	Network * airport_network = new Network();
	Network * github_network = new Network();
	Network * emails_network = new Network();


	// first parameter is path, second is if the edges has weights
	//karate_network->load_edge_list("datasets/karateclub_edges.csv", false);
	//lesmis_network->load_edge_list("datasets/lesmis_edges.csv", true);
	//football_network->load_edge_list("datasets/football_edges.csv", false);
	//netscience_network->load_edge_list("datasets/ca-netscience_edges.csv", false);
	//mouse_network->load_edge_list("nets_from_article/mouse_brain.edges", false);
	airport_network->load_edge_list("nets_from_article/airport.csv", true);

	//github_network->load_edge_list("nets_from_article/git_edges.csv", false);
	//emails_network->load_edge_list("nets_from_article/edges_emails.csv", false);

	// export mapping 
	//mouse_network->export_mapping("nets_from_article/mouse_brain_mapping.csv");
	airport_network->export_mapping("nets_from_article/airport_mapping.csv");
	//github_network->export_mapping("nets_from_article/git_mapping.csv");
	//emails_network->export_mapping("nets_from_article/emails_mapping.csv");


	//condmat_network->load_edge_list("datasets/condmat_edges.csv", true);
	//citeseer_network->load_edge_list("datasets/citeseer.edges", true);
	//citeseer_network->load_node_labels("datasets/citeseer_node_labels.csv");

	Dependency* mouse_dependency = new Dependency(mouse_network);
	Dependency* airport_dependency = new Dependency(airport_network);
	Dependency* github_dependency = new Dependency(github_network);
	Dependency * emails_dependency = new Dependency(emails_network);

	vector<int> emb_dims = { 4, 8, 16, 32, 64, 128 };


	// print dok
	//mouse_network->prin_dok();
	//airport_network->prin_dok();

	for (int i = 0; i < emb_dims.size(); i++)
	{
		cout << "dim: " << emb_dims[i] << endl;
		for (int y = 0; y < 50; y++)
		{
			cout << "iter: " << y << endl;

			/*DepDistContraction* mouse_ddc = new DepDistContraction(
				mouse_network,
				mouse_dependency,
				emb_dims[i],
				0.002,
				0.01
			);*/

			DepDistContraction* airport_ddc = new DepDistContraction(
				airport_network,
				airport_dependency,
				emb_dims[i],
				0.002,
				0.01
			);

			//DepDistContraction* github_ddc = new DepDistContraction(
			//	github_network,
			//	github_dependency,
			//	emb_dims[i],
			//	0.002,
			//	0.01
			//);

			/*DepDistContraction* emails_ddc = new DepDistContraction(
				emails_network,
				emails_dependency,
				emb_dims[i],
				0.002,
				0.01
			);*/



			//mouse_ddc->run(500);
			airport_ddc->run(500);
			//github_ddc->run(500);
			//emails_ddc->run(500);

			//mouse_ddc->export_embs_for_CGE("D:\\Ing\\3. semester\\MAS2\\source_code\\DepDistContraction\\DepDistContraction\\results\\mouse_brain\\dim_" + to_string(emb_dims[i]) + "\\embs\\emb_" + to_string(y) +".embs");
			//github_ddc->export_embs_for_CGE("D:\\Ing\\3. semester\\MAS2\\source_code\\DepDistContraction\\DepDistContraction\\results\\github\\dim_" + to_string(emb_dims[i]) + "\\embs\\emb_" + to_string(y) + ".embs");
			airport_ddc->export_embs_for_CGE("D:\\Ing\\3. semester\\MAS2\\source_code\\DepDistContraction\\DepDistContraction\\results\\airports\\dim_" + to_string(emb_dims[i]) + "\\embs\\emb_" + to_string(y) + ".embs");
			//emails_ddc->export_embs_for_CGE("D:\\Ing\\3. semester\\MAS2\\source_code\\DepDistContraction\\DepDistContraction\\results\\emails\\dim_" + to_string(emb_dims[i]) + "\\embs\\emb_" + to_string(y) + ".embs");


			//airport_ddc->export_embs_for_CGE("results\\airport\\");
			//github_ddc->export_embs_for_CGE("results\\github\\");
			//airport_ddc->export_embs_for_CGE("D:\\Ing\\diplomka\\comparing_embs\\src\\airport_100_2_500_iter\\airport_" + to_string(i) + ".embs");

			//delete mouse_ddc;
			delete airport_ddc;
			//delete github_ddc;
			//delete emails_ddc;
		}
	}

	//for (int i = 0; i < 100; i++)
	//{
	//	cout << i << endl;

		//DepDistContraction* mouse_ddc = new DepDistContraction(
	//		mouse_network,
	//		mouse_dependency,
	//		2,
	//		0.002,
	//		0.01
	//	);


	//	mouse_ddc->run(500);
	//	//mouse_ddc->export_gdf("mouse_brain.gdf");
	//	mouse_ddc->export_embs_for_CGE("D:\\Ing\\diplomka\\comparing_embs\\src\\mouse_brain_100_2_500_iter\\mouse_brain_" + to_string(i) + ".embs");
	//
	//	//delete mouse_dependency;
	//	delete mouse_ddc;

	//}


	//Dependency* karate_dependency = new Dependency(karate_network);
	//Dependency* lesmis_dependency = new Dependency(lesmis_network);
	//Dependency* football_dependency = new Dependency(football_network);
	//Dependency* netscience_dependency = new Dependency(netscience_network);
	//int num_of_iter = 20;






	//for (int i = 0; i < num_of_iter; i++)
	//{
	//	cout << "Iteration: " << i << endl;

	//	DepDistContraction* karate_ddc = new DepDistContraction(
	//		karate_network,
	//		karate_dependency,
	//		32,
	//		0.002,
	//		0.01
	//	);

	//	DepDistContraction* lesmis_ddc = new DepDistContraction(
	//		lesmis_network,
	//		lesmis_dependency,
	//		32,
	//		0.002,
	//		0.01
	//	);

	//	DepDistContraction* football_ddc = new DepDistContraction(
	//		football_network,
	//		football_dependency,
	//		32,
	//		0.002,
	//		0.01
	//	);

	//	DepDistContraction* netscience_ddc = new DepDistContraction(
	//		netscience_network,
	//		netscience_dependency,
	//		32,
	//		0.002,
	//		0.01
	//	);



	//	//cout << " karate" << endl;
	//	karate_ddc->run(500);

	//	//karate_network->prin_dok();
	//	//cout << " lesmis" << endl;
	//	football_ddc->run(500);
	//	//cout << " football" << endl;
	//	lesmis_ddc->run(500);
	//	//cout << " netscience" << endl;
	//	netscience_ddc->run(500);


	//	karate_ddc->export_embs_for_CGE("embs_for_cge/karate/karate_" + to_string(i) + ".embs");
	//	lesmis_ddc->export_embs_for_CGE("embs_for_cge/lesmis/lesmis_" + to_string(i) + ".embs");
	//	football_ddc->export_embs_for_CGE("embs_for_cge/football/football_" + to_string(i) + ".embs");
	//	netscience_ddc->export_embs_for_CGE("embs_for_cge/netscience/netscience_" + to_string(i) + ".embs");



	//	//delete karate_dependency;
	//	//delete lesmis_dependency;
	//	//delete football_dependency;
	//	//delete netscience_dependency;

	//	delete karate_ddc;
	//	delete lesmis_ddc;
	//	delete football_ddc;
	//	delete netscience_ddc;
	//}


}