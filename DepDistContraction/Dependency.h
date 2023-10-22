#pragma once
#include <map>
#include "Network.h"
using namespace std;
class Dependency
{
public:
	Dependency(Network * net);
	double calculate_dependency_lazy(int node1, int node2);
	map<int, map<int, double>> calculate_dependy_whole_net();
private:
	map<int, map<int, double>> dependency_dok;
	const map<int, map<int, double>> *  network_dok;

	Network * network;

};

