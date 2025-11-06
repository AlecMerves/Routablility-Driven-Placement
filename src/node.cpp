#include "../include/node.hpp"

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

Node::net_link::net_link(Net* net) {
    this->net = net;
    this->next = NULL;
}

Node::net_link::~net_link() {
    if(this->next)
        delete this->next;
}

Net* Node::net_link::get_net() {
    return this->net;
}

void Node::net_link::set_net(Net* net) {
    this->net = net;
}

Node::net_link* Node::net_link::get_next() {
    return this->next;
}

void Node::net_link::set_next(Node::net_link* next) {
    this->next = next;
}

////////////////////////////////////////////////////////////////////////////////

//                            Constructors

Node::Node(const char* name, int id, int width/*=0*/, int height/*=0*/) : id(id) {
    this->name = (char*)malloc(sizeof(char) * std::strlen(name) + 1);
    std::strcpy(this->name, name);

    this->width = width;
    this->height = height;
    this->area = this->width * this->height;
    
    this->net_list = NULL;
    this->net_count = 0; 

    this->gain_ptr = NULL;

    this->lock_status = false;
}

Node::Node(std::string name, int id, int width/*=0*/, int height/*=0*/) : id(id) {
    this->name = (char*)malloc(sizeof(char) * name.length() + 1);
    name.copy(this->name, name.length(), 0);
    this->name[name.length()] = '\0';
    
    this->width = width;
    this->height = height;
    this->area = this->width * this->height;

    this->net_list = NULL;
    this->net_count = 0; 

    this->gain_ptr = NULL;

    this->lock_status = false;
}

Node::~Node() {
    free(this->name);
    if(this->net_list)
        delete this->net_list;
}

////////////////////////////////////////////////////////////////////////////////

//                              Debugging

void Node::print(std::fstream& file, bool dim, bool net, bool partition, bool gain, bool lock) {
    file << "Node " << this->name << std::endl;
    if (dim)
        file << "\t- Dimensions: " << this->width << "x" << this->height << std::endl;
    if (gain)
        file << "\t- Gain: " << this->gain << std::endl;
    if (partition)
        file << "\t- Partition: " << this->partition << std::endl;
    if (net) {
        file << "\t- " << this->net_count << " Nets: ";
        net_link* nets = this->net_list;
        for (int i = 0; i < this->net_count - 1; i++) {
            file << nets->get_net()->get_name();
            nets = nets->get_next();
        }
        file << nets->get_net()->get_name();
    }
    if (lock) {
        file << "\t- ";
        if (this->lock_status)
            file << "Locked";
        else
            file <<"Unlocked";
        file << std::endl;
    }
}

////////////////////////////////////////////////////////////////////////////////

//                           Member Methods

const char* Node::get_name() {
    return this->name;
}



const int Node::get_id() {
    return this->id;
}



int Node::get_width() {
    return this->width;
}

void Node::set_width(int width) {
    this->width = width;
}



int Node::get_height() {
    return this->height;
}

void Node::set_height(int height) {
    this->height = height;
}



int Node::get_area() {
    return this->area;
}



int Node::get_partition() {
    return this->partition;
}

void Node::set_partition(int partition) {
    this->partition = partition;
}

int Node::get_other_part() {
    return (this->partition == P1) ? P2 : P1;
}




int Node::get_lock() {
    return this->lock_status;
}

void Node::set_lock() {
    this->lock_status = true;
}

void Node::reset_lock() {
    this->lock_status = false;
}



int Node::get_gain() {
    return this->gain;
}

void Node::set_gain(int gain) {
    this->gain = gain;
}

Link* Node::get_gain_ptr() {
    return this->gain_ptr;
}

void Node::set_gain_prt(Link* gain_ptr) {
    this->gain_ptr = gain_ptr;
}

void Node::update_gain() {
    this->gain = 0;
    net_link* net_ptr = this->net_list;
    for (int i = 0; i < this->net_count; i++) {
        Net* net = net_ptr->get_net();
        if (net->F(this->partition) == 1)
            this->gain++;
        if (net->T(this->partition) == 0)
            this->gain--;
        net_ptr = net_ptr->get_next();
    }
}

void Node::modify_gain(int change) {
    this->gain = this->gain + change;
}



int Node::get_net_count() {
    return this->net_count;
}

void Node::set_net_count(int count) {
    this->net_count = count;
}



Node::net_link* Node::get_net() {
    return this->net_list;
}

void Node::set_net(Node::net_link* net_list) {
    this->net_list = net_list;
}

void Node::add_net(Net* net) {
    net_link* nets = this->net_list;
    net_link* new_net = new net_link(net);
    if (nets == NULL)
        this->net_list = new_net;
    else {
        for (int i = 0; i < this->net_count - 1; i++) {
            nets = nets->get_next();
        }
        nets->set_next(new_net);
    }
    this->net_count += 1;
}



Net* Node::get_next_net() {
    return this->iter->get_net();
}

void Node::init_iter() {
    this->iter = this->net_list;
}

void Node::iter_next() {
    this->iter = this->iter->get_next();
}