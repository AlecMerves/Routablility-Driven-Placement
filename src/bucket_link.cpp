#include "../include/bucket_link.hpp"

#include <fstream>

Link::Link(Node* node, int idx) : node(node) {
    this->next = NULL;
    this->prev = NULL;
    this->bucket = idx;
}

Link::Link(Node* node, int idx, Link* next, Link* prev) {
    this->node = node;
    this->next = next;
    this->prev = prev;
    this->bucket = idx;
}

Link::~Link() {
    if (this->next)
        delete this->next;
}



void Link::print(std::fstream& file) {
    file << "\t\t- Node: " << this->node->get_name() << std::endl;
}



Node* Link::get_node() {
    return this->node;
}

void Link::set_node(Node* node) {
    this->node = node;
}



Link* Link::get_next() {
    return this->next;
}

void Link::set_next(Link* link) {
    this->next = link;
}



Link* Link::get_prev() {
    return this->prev;
}

void Link::set_prev(Link* link) {
    this->prev = link;
}



int Link::get_bucket() {
    return this->bucket;
}

void Link::set_bucket(int idx) {
    this->bucket = idx;
}