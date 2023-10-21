#include "Network.h"

Network::Network()
{
	this->dok = map<int, map<int, double> >();
	this->number_of_nodes = 0;
	this->number_of_edges = 0;
	this->keys = new vector<int>();

}

Network::~Network()
{
	delete this->keys;
}

void Network::load_edge_list(string filename,bool has_weights)
{
	// measure time of execution


	ifstream file(filename);
	string line;
	int src;
	int tar;
	double weight = 1.0;

	while (getline(file, line))
	{
		this->number_of_edges++;

		istringstream iss(line);
		if (has_weights)
		{
			iss >> src >> tar >> weight;
		}
		else
		{
			iss >> src >> tar;
		}
		
		if (this->dok.find(src) == this->dok.end())
		{
			this->dok[src] = map<int, double>();

			this->number_of_nodes++;

		}

		if (this->dok.find(tar) == this->dok.end())
		{
			this->dok[tar] = map<int, double>();
			this->number_of_nodes++;
		}

		this->dok[src][tar] = weight;
		this->dok[tar][src] = weight;
	}


	// generate keys 
	for (auto const& x : this->dok)
	{
		this->keys->push_back(x.first);
	}
}


void Network::print_edge_list()
{
	for (auto const& x : this->dok)
	{
		cout << x.first << ": ";
		for (auto const& y : x.second)
		{
			cout << y.first << " ";
		}
		cout << endl;
	}
}

void Network::print_neighbours(int node)
{
	for (auto const& x : this->dok[node])
	{
		cout << x.first << " ";
	}
	cout << endl;
}

void Network::prin_dok()
{
for (auto const& x : this->dok)
	{
		cout << x.first << ": ";
		for (auto const& y : x.second)
		{
			cout << y.first << " ";
		}
		cout << endl;
	}
}

int Network::common_neighbours_count(int node1, int node2)
{
	int common_neighbours = 0;

	for (auto const& x : this->dok[node1])
	{
		if (this->dok[node2].find(x.first) != this->dok[node2].end())
		{
			common_neighbours++;
		}
	}

	return common_neighbours;
}

vector<int> Network::common_neighbours_list(int node1, int node2)
{
	vector<int> common_neighbours;

	for (auto const& x : this->dok[node1])
	{
		if (this->dok[node2].find(x.first) != this->dok[node2].end())
		{
			common_neighbours.push_back(x.first);
		}
	}

	return common_neighbours;
}

vector<int>* Network::get_keys()
{
	return this->keys;
}

void Network::insert_edge(int src, int tar)
{
	this->number_of_edges++;

	if (this->dok.find(src) == this->dok.end())
	{
		this->dok[src] = map<int, double>();
		this->number_of_nodes++;
	}
	if (this->dok.find(tar) == this->dok.end())
	{
		this->dok[tar] = map<int, double>();
		this->number_of_nodes++;
	}

	if (this->dok[src].find(tar) == this->dok[src].end())
	{
		this->dok[src][tar] = 1.0;
	}
	else
	{
		this->dok[src][tar] += 1.0;

	}

	// undirected network

	if (this->dok[tar].find(src) == this->dok[tar].end())
	{
		this->dok[tar][src] = 1.0;
	}
	else
	{
		this->dok[tar][src] += 1.0;
	}
}

void Network::insert_node(int node)
{
	if (this->dok.find(node) == this->dok.end())
	{
		this->dok[node] = map<int, double>();
		this->number_of_nodes++;
	}
}

void Network::compute_keys()
{
	// generate keys 
	for (auto const& x : this->dok)
	{
		this->keys->push_back(x.first);
	}
}
