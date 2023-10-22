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

	// first parameter is path, second is if the edges has weights
	karate_network->load_edge_list("datasets/karateclub_edges.csv", false);
	lesmis_network->load_edge_list("datasets/lesmis_edges.csv", true);
	football_network->load_edge_list("datasets/football_edges.csv", false);
	netscience_network->load_edge_list("datasets/ca-netscience_edges.csv", false);
	condmat_network->load_edge_list("datasets/condmat_edges.csv", true);


	Dependency * karate_dependency = new Dependency(karate_network);
	Dependency * lesmis_dependency = new Dependency(lesmis_network);
	Dependency * football_dependency = new Dependency(football_network);
	Dependency * netscience_dependency = new Dependency(netscience_network);
	Dependency * condmat_dependency = new Dependency(condmat_network);


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
		2,
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


	karate_ddc->run(500);
	football_ddc->run(500);
	netscience_ddc->run(500);
	netscience_ddc->run(500);
	condmat_ddc->run(500);


	karate_ddc->export_gdf("output_embs/karate_embs.gdf");
	lesmis_ddc->export_gdf("output_embs/lesmis_embs.gdf");
	football_ddc->export_gdf("output_embs/football_embs.gdf");
	netscience_ddc->export_gdf("output_embs/netscience_embs.gdf");
	condmat_ddc->export_gdf("output_embs/condmat_embs.gdf");

}