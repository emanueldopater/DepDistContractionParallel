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
	Network * karate_network = new Network();
	Network * lesmis_network = new Network();
	Network * football_network = new Network();
	Network * netscience_network = new Network();
	Network * condmat_network = new Network();
	Network * citeseer_network = new Network();

	// first parameter is path, second is if the edges has weights
	karate_network->load_edge_list("datasets/karateclub_edges.csv", false);
	lesmis_network->load_edge_list("datasets/lesmis_edges.csv", true);
	football_network->load_edge_list("datasets/football_edges.csv", false);
	netscience_network->load_edge_list("datasets/ca-netscience_edges.csv", false);
	condmat_network->load_edge_list("datasets/condmat_edges.csv", true);
	citeseer_network->load_edge_list("datasets/citeseer.edges", true);
	//citeseer_network->load_node_labels("datasets/citeseer_node_labels.csv");

	Dependency * karate_dependency = new Dependency(karate_network);
	Dependency * lesmis_dependency = new Dependency(lesmis_network);
	Dependency * football_dependency = new Dependency(football_network);
	Dependency * netscience_dependency = new Dependency(netscience_network);
	Dependency * condmat_dependency = new Dependency(condmat_network);
	Dependency * citeseer_dependency = new Dependency(citeseer_network);


	DepDistContraction* karate_ddc = new DepDistContraction(
		karate_network,
		karate_dependency,
		2, 
		0.002, 
		0.01
	);

	DepDistContraction* lesmis_ddc = new DepDistContraction(
		lesmis_network,
		lesmis_dependency,
		128,
		0.002,
		0.01
	);

	DepDistContraction* football_ddc = new DepDistContraction(
		football_network,
		football_dependency,
		2,
		0.002,
		0.01
	);

	DepDistContraction* netscience_ddc = new DepDistContraction(
		netscience_network,
		netscience_dependency,
		2, 
		0.002, 
		0.01
	);
		
	DepDistContraction * condmat_ddc = new DepDistContraction(
		condmat_network,
		condmat_dependency,
		2, 
		0.002, 
		0.01
	);

	DepDistContraction* citeseer_ddc = new DepDistContraction(
		citeseer_network,
		citeseer_dependency,
		2,
		0.002,
		0.01
	);


	//karate_ddc->run(500);
	//football_ddc->run(500);
	//lesmis_ddc->run(500);
	//netscience_ddc->run(500);
	condmat_ddc->run(500);

	//citeseer_ddc->run(500);

	//citeseer_network->print_intern_mapping();

    //karate_ddc->export_gdf("output_embs/karate_embs.gdf");

	
	//lesmis_ddc->export_gdf("output_embs/lesmis_embs.gdf");
	//lesmis_ddc->export_embs_for_CGE("output_embs/lesmis_embs_for_CGE.txt");
	//football_ddc->export_gdf("output_embs/football_embs.gdf");
	//netscience_ddc->export_gdf("output_embs/netscience_embs.gdf");
	
	condmat_ddc->export_gdf("output_embs/condmat_embs.gdf");
	//condmat_ddc->export_embs_for_CGE("output_embs/condmat_embs_for_CGE.txt");
	//citeseer_ddc->export_gdf("output_embs/citeseer_embs.gdf");
	//citeseer_ddc->export_gdf_node_labels("output_embs/citeseer_embs_node_labels.gdf");

	//citeseer_ddc->export_embs_for_CGE("output_embs/citeseer_embs_for_CGE.txt");
	delete karate_network;
	delete lesmis_network;
	delete football_network;
	delete netscience_network;
	delete condmat_network;

	/*delete karate_dependency;
	delete lesmis_dependency;
	delete football_dependency;
	delete netscience_dependency;
	delete condmat_dependency;

	delete karate_ddc;
	delete lesmis_ddc;
	delete football_ddc;
	delete netscience_ddc;
	delete condmat_ddc;*/

	return 0;

}