#include "../include/node_table.hpp"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

NodeTable::NodeTable(int size/*=1*/) : size(size) {
    this->table = (Node**)malloc(sizeof(Node*) * this->size);
    this->o_pos = -1;
    this->p_pos = -1;
    this->locked = 0;
}

NodeTable::~NodeTable() {
    for (int i = 0 ; i < this->size; i++)
        delete this->table[i];
    free(this->table);
}



void NodeTable::print(std::fstream& file, bool dim/*=false*/, bool net/*=false*/, bool partition/*=false*/, bool gain/*=false*/, bool lock/*=false*/) {
    file << "Nodes: " << std::endl;
    for (int i = 0; i < this->size; i++) {
        this->table[i]->print(file, dim, net, partition, gain, lock);
    }
    file << std::endl;
}



int NodeTable::get_size() {
    return this->size;
}

void NodeTable::set_size(int size) {
    this->size = size;
    this->table = (Node**)realloc(this->table, sizeof(Node*) * this->size);
}



Node* NodeTable::get_node(int index) {
    return this->table[index];
}

Node* NodeTable::get_node(std::string name) {
    int pos = (name[0] == 'o') ? this->o_pos : this->p_pos;
    pos += std::stoi(name.substr(1));
    return this->table[pos];
}

void NodeTable::set_node(int index, Node* node) {
    this->table[index] = node;
}



Node** NodeTable::get_nodes() {
    return this->table;
}

void NodeTable::set_nodes(Node** node_list) {
    this->table = node_list;
}



void NodeTable::load_table(char* benchmark) {
    std::string line, buff, name;
    std::stringstream sstream;
    char file_name[100];
    int num_nodes, width, height;
    Node* new_node;

    sprintf(file_name, "./benchmarks/%s/%s.nodes", benchmark, benchmark);
    std::ifstream file(file_name);
    for (int i = 0; i <= NODE_HEADER_SIZE; i++) {
        std::getline(file, line);
        sstream << line;

        sstream >> buff;
        if (buff.compare("NumNodes") == 0) {
            sstream >> buff;
            sstream >> buff;
            num_nodes = std::stoi(buff);
            this->set_size(num_nodes);
        }

        sstream.str("");
        sstream.clear();
    }

    int count = 0;
    while (std::getline(file, line)) {
        sstream << line;
        if (sstream.str().empty())
            continue;

        sstream >> name;
        if (name[0] == 'o' && this->o_pos < 0)
            this->o_pos = count;
        if (name[0] == 'p' && this->p_pos < 0)
            this->p_pos = count;

        sstream >> buff;
        width = std::stoi(buff);
        sstream >> buff;
        height = std::stoi(buff);

        new_node = new Node(name, count, width, height);
        this->table[count++] = new_node;


        sstream.str("");
        sstream.clear();
    }
    file.close();
}

void NodeTable::init_partitions() {
    std::srand(std::time({}));
    for (int i = 0; i < this->size; i++) {
        Node* current = this->table[i];
        //int p = (i % 2 == 0) ? P1 : P2;                       // Initial partitions alternate with nodes
        //int p = (current->get_name()[0] == 'o') ? P1 : P2;    // Initial partitions follow node name
        int p = (std::rand() % 2 == 0) ? P1 : P2;               // Initial partitions random
        current->set_partition(p);
    }
}

void NodeTable::init_gains(BucketStruct* bucket) {
    for (int i = 0; i < this->size; i++) {
        Node* current = this->table[i];
        current->update_gain();
        bucket->insert(current);
    }
}


int NodeTable::max_degree() {
    int max = this->table[0]->get_net_count();
    for (int i = 1; i < this->size; i++) {
        int current = this->table[i]->get_net_count();
        if (current > max)
            max = current;
    }
    return max;
}



void NodeTable::unlock() {
    this->locked = 0;
    for (int i = 0; i < this->size; i++) {
        this->table[i]->reset_lock();
    }
}

void NodeTable::inc_lock() {
    this->locked++;
}

int NodeTable::get_lock() {
    return this->locked;
}

