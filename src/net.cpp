#include "../include/net.hpp"

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

Net::Net(const char* name, int count) {
    this->name = (char*)malloc(sizeof(char) * std::strlen(name));
    std::strcpy(this->name, name);

    this->node_list = NULL;
    this->set_node_count(count);
}

Net::Net(std::string name, int count) {
    this->name = (char*)malloc(sizeof(char) * name.length());
    name.copy(this->name, name.length(), 0);
    this->name[name.length()] = '\0';

    this->node_list = NULL;
    this->set_node_count(count);
}

Net::~Net() {
    free(this->name);
    free(this->node_list);
}



void Net::print(std::fstream& file, bool nodes, bool partition, bool p_nodes) {
    file << "Net " << this->name << std::endl;
    file << "\t- " << this->node_count << " nodes" << std::endl;
    if (partition) {
        file << "\t- P1: " << this->p1 << std::endl;
        if (p_nodes) {
            for (int i = 0; i < this->node_count; i++) {
                Node* node = this->node_list[i];
                if (node->get_partition() == P1) {
                    file << "\t\t- ";
                    node->print(file, false, false, false, false, false);
                }
            } 
        }       
        file << "\t- P2: " << this->p2 << std::endl;
        if (p_nodes) {
            for (int i = 0; i < this->node_count; i++) {
                Node* node = this->node_list[i];
                if (node->get_partition() == P2) {
                    file << "\t\t- ";
                    node->print(file, false, false, false, false, false);
                }
            }   
        }
    }
    if (nodes) {
        file << "\t- Nodes:" << std::endl;
        for (int i = 0; i < this->node_count; i++)
            file << "\t\t- " << this->get_node(i)->get_name() << std::endl;
    }
}



const char* Net::get_name() {
    return this->name;
}



int Net::get_p1() {
    return this->p1;
}

void Net::set_p1(int count) {
    this->p1 = count;
}



int Net::get_p2() {
    return this->p2;
}

void Net::set_p2(int count) {
    this->p2 = count;
}



bool Net::get_criticality() {
    return this->critical;
}

void Net::update_criticality() {
    this->critical = this->p1 > 0 && this->p2 > 0;
}



int Net::get_node_count() {
    return this->node_count;
}

void Net::set_node_count(int count) {
    this->node_count = count;
    if (this->node_list != NULL)
        this->node_list = (Node**)realloc(this->node_list, sizeof(Node*) * this->node_count);
    else
        this->node_list = (Node**)malloc(sizeof(Node*) * this->node_count);
}



Node* Net::get_node(int index) {
    return this->node_list[index];
}

void Net::set_node(int index, Node* node) {
    this->node_list[index] = node;
}



Node** Net::get_nodes() {
    return this->node_list;
}

void Net::set_nodes(Node** node_list) {
    this->node_list = node_list;
}



int Net::T(int F) { // To side
    return (F == P1) ? this->p2 : this->p1;
}

int Net::F(int F) { // From side
    return (F == P1) ? this->p1 : this->p2;
}



void Net::dec_F(int F) {
    this->p1 -= (F == P1) ? 1 : 0;
    this->p2 -= (F == P2) ? 1 : 0;
}

void Net::inc_T(int F) {
    this->p1 += (F == P2) ? 1 : 0;
    this->p2 += (F == P1) ? 1 : 0;
}



void Net::inc_gains(std::fstream& file, BucketStruct* bucket, Node* base_node, int p) {
    for (int i = 0; i < this->node_count; i++) {
        Node* node = this->node_list[i];
        if (node == base_node || node->get_partition() != p || node->get_lock()) continue;
        node->modify_gain(1);
        bucket->move(node);
        if (file)
            file << "\t- Incremented gain of " << node->get_name() << std::endl;
    }
}

void Net::dec_gains(std::fstream& file, BucketStruct* bucket, Node* base_node, int p) {
    for (int i = 0; i < this->node_count; i++) {
        Node* node = this->node_list[i];
        if (node == base_node || node->get_partition() != p || node->get_lock()) continue;
        node->modify_gain(-1);
        bucket->move(node);
        if (file)
            file << "\t- Decremented gain of " << node->get_name() << std::endl;
    }
}



void Net::init_partitions() {
    this->p1 = 0;
    this->p2 = 0;
    for (int i = 0; i < this->node_count; i++) {
        if (this->node_list[i]->get_partition() == P1)
            this->p1++;
        else    
            this->p2++;
    }
    this->update_criticality();
}

