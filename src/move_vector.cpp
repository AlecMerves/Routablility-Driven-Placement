#include "../include/move_vector.hpp"

#include <fstream>

MoveVector::move_node::move_node() {
    this->node = NULL;
    this->cut_size = -1;
}



void MoveVector::move_node::set_node(Node* node) {
    this->node = node;
}

Node* MoveVector::move_node::get_node() {
    return this->node;
}



void MoveVector::move_node::set_cut(int cut_size) {
    this->cut_size = cut_size;
}

int MoveVector::move_node::get_cut() {
    return this->cut_size;
}



void MoveVector::move_node::reset() {
    this->node = NULL;
    this->cut_size = -1;
}





MoveVector::MoveVector(int size) : size(size) {
    this->move_table = (move_node**)malloc(sizeof(move_node*) * size);
    for (int i = 0; i < size; i++)
        this->move_table[i] = new move_node();
    this->min_cut = -1;
    this->min_pos = -1;
}

MoveVector::~MoveVector() {
    for (int i = 0; i < this->size; i++)
        delete this->move_table[i];
    free(this->move_table);
}



void MoveVector::print(std::fstream& file) {
    file << "Move table:" << std::endl;
    for (int i = 0; i < this->size; i++)
        file << "\t- Move " << i << " - cut: " << this->move_table[i]->get_cut() << std::endl;
    file << std::endl;
}



void MoveVector::set_move(int idx, Node* node, int cut) {
    MoveVector::move_node* move = this->move_table[idx];
    move->set_node(node);
    move->set_cut(cut);
    if (cut <= this->min_cut || this->min_cut == -1) {
        this->min_cut = cut;
        this->min_pos = idx;
    }
}



Node* MoveVector::get_node(int idx) {
    return this->move_table[idx]->get_node();
}



int MoveVector::get_size() {
    return this->size;
}

int MoveVector::get_min_cut() {
    return this->min_cut;
}

int MoveVector::get_min_pos() {
    return this->min_pos;
}



void MoveVector::reset() {
    for (int i = 0; i < this->size; i++)
        this->move_table[i]->reset();
    this->min_cut = -1;
    this->min_pos = -1;
}