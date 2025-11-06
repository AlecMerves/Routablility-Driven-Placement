#include "../include/net_table.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

NetTable::NetTable(int size/*=1*/) : size(size) {
    this->table = (Net**)malloc(sizeof(Net*) * this->size);
}

NetTable::~NetTable() {
    for (int i = 0; i < this->size; i++)
        delete this->table[i];
    free(this->table);
}


void NetTable::print(std::fstream& file, bool nodes/*=false*/, bool partition/*=false*/, bool p_nodes/*=false*/) {
    file << "Nets:" << std::endl;
    for (int i = 0; i < this->size; i++) {
        this->table[i]->print(file, nodes, partition, p_nodes);
    }
    file << std::endl;
}



int NetTable::get_size() {
    return this->size;
}

void NetTable::set_size(int size) {
    this->size = size;
    this->table = (Net**)realloc(this->table, sizeof(Net*) * this->size);
}



Net* NetTable::get_net(int index) {
    return this->table[index];
}

void NetTable::set_net(int index, Net* net) {
    this->table[index] = net;
}



Net** NetTable::get_nets() {
    return this->table;
}

void NetTable::set_nets(Net** net_list) {
    this->table = net_list;
}



void NetTable::load_table(char* benchmark, NodeTable& nodes) {
    std::string line, buff, name;
    std::stringstream sstream;
    char file_name[100];
    int num_nets, net_degree, node_pos;
    Net* new_net;
    Node* node;

    sprintf(file_name, "./benchmarks/%s/%s.nets", benchmark, benchmark);
    std::ifstream file(file_name);
    for (int i = 0; i < NET_HEADER_SIZE; i++) {
        std::getline(file, line);
        sstream << line;

        sstream >> buff;
        if (buff.compare("NumNets") == 0) {
            sstream >> buff;
            sstream >> buff;
            num_nets = std::stoi(buff);
            this->set_size(num_nets);
        }

        sstream.str("");
        sstream.clear();
    }

    int count = 0;
    while (std::getline(file, line)) {
        sstream << line;
        if (sstream.str().empty())
            continue;
        
        sstream >> buff;
        if (buff.compare("NetDegree") == 0) {
            sstream >> buff;
            sstream >> buff;
            net_degree = std::stoi(buff);
            sstream >> name;

            new_net = new Net(name, net_degree);
            for (int i = 0; i < net_degree; i++) {
                sstream.str("");
                sstream.clear();
                std::getline(file, line);
                sstream << line;

                sstream >> buff;
                node = nodes.get_node(buff);
                new_net->set_node(i, node);
                node->add_net(new_net);
            }
            this->table[count++] = new_net;
        }

        sstream.str("");
        sstream.clear();
    }
    file.close();
}

void NetTable::init_partitions() {
    for (int i = 0; i < this->size; i++) {
        this->table[i]->init_partitions();
    }
}