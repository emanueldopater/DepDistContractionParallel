#include "Network.h"

Network::Network()
{
	this->dok = map<int, map<int, double> >();
	this->number_of_nodes = 0;
	this->number_of_edges = 0;
	this->keys = new vector<int>();
	this->intern_node_id_mapping = map<string, int>();
	this->reverse_intern_node_id_mapping = map<int, string>();
	this->node_labels = map<int, int>();
}

Network::~Network()
{
	delete this->keys;
}

void Network::load_edge_list(string filename,bool has_weights)
{
	// measure time of execution
	int intern_node_id = 0;
	ifstream file(filename);
	string line;
	string src;
	string tar;
	double weight = 1.0;

	while (getline(file, line))
	{

		istringstream iss(line);
		if (has_weights)
		{

			iss >> src;
			iss >> tar;
			iss >> weight;

		}
		else
		{
			iss >> src;
			iss >> tar;
		}

		// if src and tar are the same, skip
		if (src == tar)
		{
			continue;
		}

		// node intern id mapping 
		//cout << "Loaded src, tar, weight:" << src << ", " << tar << ", " << weight << endl;

		//src

		int src_int, tar_int;
		if (this->intern_node_id_mapping.find(src) == this->intern_node_id_mapping.end())
		{
			this->intern_node_id_mapping[src] = intern_node_id;
			this->reverse_intern_node_id_mapping[intern_node_id] = src;
			src_int = intern_node_id;
			intern_node_id++;

		}
		else
		{
			src_int = this->intern_node_id_mapping[src];
		}

		////tar
		if (this->intern_node_id_mapping.find(tar) == this->intern_node_id_mapping.end())
		{
			this->intern_node_id_mapping[tar] = intern_node_id;
			this->reverse_intern_node_id_mapping[intern_node_id] = tar;
			tar_int = intern_node_id;
			intern_node_id++;
		}
		else
		{
			tar_int = this->intern_node_id_mapping[tar];
		}

		//cout << "Transformed src, tar, weight:" << src << ", " << tar << ", " << weight << endl;
		//cout << endl;

		/// end of node intern id mapping


		if (this->dok.find(src_int) == this->dok.end())
		{
			this->dok[src_int] = map<int, double>();
			this->number_of_nodes++;
		}

		if (this->dok.find(tar_int) == this->dok.end())
		{
			this->dok[tar_int] = map<int, double>();
			this->number_of_nodes++;
		}

		this->number_of_edges++;


		this->dok[src_int][tar_int] = weight;
		this->dok[tar_int][src_int] = weight;
	}


	// generate keys 
	for (auto const& x : this->dok)
	{
		this->keys->push_back(x.first);
	}
}

void Network::load_node_labels(string filename)
{
	ifstream file(filename);
	string line;
	int node;
	int label;

	while (getline(file, line))
	{
		istringstream iss(line);

		iss >> node;
		iss >> label;

		
		this->node_labels[node] = label;
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

vector<int> Network::neighbours_list(int node)
{
	vector<int> neighbours;

	for (auto const& x : this->dok[node])
	{
		neighbours.push_back(x.first);
	}

	return neighbours;
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

void Network::print_intern_mapping()
{

	for (auto const& x : this->intern_node_id_mapping)
	{
		cout << x.first << ": " << x.second << endl;
	}

	// reverse mapping 
	cout << "Reverse mapping:" << endl;
	for (auto const& x : this->reverse_intern_node_id_mapping)
	{
		cout << x.first << ": " << x.second << endl;
	}
}

void Network::export_mapping(string filename)
{
	ofstream mapping_file;
	mapping_file.open(filename);

	for (auto const& x : this->intern_node_id_mapping)
	{
		mapping_file << x.first << " " << x.second << endl;
	}

	mapping_file.close();
}

