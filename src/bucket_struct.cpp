#include "../include/bucket_struct.hpp"

#include <fstream>

BucketStruct::BucketStruct(int pmax) : pmax(pmax) {
    this->table = (Link**)malloc(sizeof(Link*) * (2*pmax+1));
    for (int i = 0; i < 2*pmax+1; i++)
        this->table[i] = NULL;
    this->max = -pmax;
}

BucketStruct::~BucketStruct() {
    for (int i = -this->pmax; i <= this->pmax; i++) {
        if (this->table[this->gain_to_idx(i)])
            delete this->table[this->gain_to_idx(i)];
    }
    free(this->table);
}

void BucketStruct::print(std::fstream& file) {
    file << "Bucket Structure:" << std::endl;
    for (int i = this->pmax; i >= -this->pmax; i--)
        this->print_bucket(file, i);
    file << std::endl;
}

void BucketStruct::print_bucket(std::fstream& file, int bucket) {
    Link* current = this->table[this->gain_to_idx(bucket)];
    file << "\t- Bucket " << bucket << std::endl;
    if (current == NULL)
        file << "\t\t- Empty" << std::endl;
    while (current) {
        current->print(file);
        current = current->get_next();
    }   
}

int BucketStruct::get_pmax() {
    return this->pmax;
}

void BucketStruct::set_pmax(int pmax) {
    this->pmax = pmax;
}



Link** BucketStruct::get_table() {
    return this->table;
}

void BucketStruct::set_table(Link** table) {
    this->table = table;
}



Link* BucketStruct::get_bucket(int idx) {
    return this->table[idx];
}

Link* BucketStruct::get_bucket(Node* node) {
    int gain = node->get_gain(); // Get gain of the node
    int idx = this->gain_to_idx(gain); // Convert gain to table index
    return this->get_bucket(idx); // Use index to get Link from table
}

Link* BucketStruct::get_last(Link* bucket) {
    Link* next = NULL;
    Link* current = bucket;
    Link* prev = NULL;
    while (current != NULL) {
        next = current->get_next();
        if (next == NULL) {
            break;
        }
        prev = current;
        current = next;
    }
    return current;
}

void BucketStruct::empty_bucket(int idx) {
    this->table[idx] = NULL;
}



void  BucketStruct::insert(Node* node) { // Inserts node at beginning of bucket
    int gain = node->get_gain(); // Get gain of the node
    int idx = this->gain_to_idx(gain); // Convert gain to table index
    Link* bucket = this->get_bucket(idx); // Use index to get Link from table
    
    Link* new_link = node->get_gain_ptr();
    if (!new_link) {
        new_link = new Link(node, idx, bucket, NULL); // Create new link for the node
        node->set_gain_prt(new_link);
    } else {
        new_link->set_bucket(idx);
        new_link->set_next(bucket);
        new_link->set_prev(NULL);
    }

    if (bucket) bucket->set_prev(new_link); // Set ends next to the new link
    this->table[idx] = new_link;

    if (gain > this->max)
        this->max = gain;
}

void BucketStruct::move(Node* node) {
    Link* gain_link = node->get_gain_ptr(); // Get link for this node
    int gain = node->get_gain();

    Link* prev = gain_link->get_prev(); // Remove link from its bucket
    Link* next = gain_link->get_next();
    if (prev) prev->set_next(next);
    else this->table[gain_link->get_bucket()] = next;// if prev == NULL then this was first node in bucket, must empty the bucket
    if (next) next->set_prev(prev);

    int idx = this->gain_to_idx(gain); // Convert gain to table index
    Link* bucket = this->get_bucket(idx); // Use index to get Link from table

    if (bucket) bucket->set_prev(gain_link);
    gain_link->set_next(bucket);
    gain_link->set_prev(NULL);
    gain_link->set_bucket(idx);
    this->table[idx] = gain_link;

    if (gain > this->max) this->max = gain;
}

void BucketStruct::remove(Node* node) {
    Link* gain_link = node->get_gain_ptr(); // Get link for this node
    int gain = node->get_gain();

    Link* prev = gain_link->get_prev(); // Remove link from its bucket
    Link* next = gain_link->get_next();
    if (prev) prev->set_next(next);
    else this->table[gain_link->get_bucket()] = next;// if prev == NULL then this was first node in bucket, must empty the bucket
    if (next) next->set_prev(prev);

    gain_link->set_next(NULL);
    gain_link->set_prev(NULL);
}



Node* BucketStruct::get_max() {
    Link* bucket;
    for (int i = this->max; i >= -this->pmax; i--) {
        bucket = this->get_bucket(this->gain_to_idx(i));
        if (bucket) return bucket->get_node();
    }
    return bucket->get_node();
}

Node* BucketStruct::get_max_size(int p1_size, int p2_size, int max_size) {
    Link* bucket;
    for (int i = this->max; i >= -this->pmax; i--) {
        bucket = this->get_bucket(this->gain_to_idx(i));

        Node* node;
        while (bucket) {
            node = bucket->get_node();
            int p = node->get_partition();
            if ((p == P1 && p2_size + 1 < max_size) || (p == P2 && p1_size + 1 < max_size))
                return node;
            bucket = bucket->get_next();
        }
    }
    return NULL;
}

Node* BucketStruct::get_max_area(int p1_area, int p2_area, int max_area) {
    Link* bucket;
    for (int i = this->max; i >= -this->pmax; i--) {
        bucket = this->get_bucket(this->gain_to_idx(i));

        Node* node;
        while (bucket) {
            node = bucket->get_node();
            int area = node->get_area();
            int p = node->get_partition();
            if ((p == P1 && p2_area + area < max_area) || (p == P2 && p1_area + area < max_area))
                return node;
            bucket = bucket->get_next();
        }
    }
    return NULL;
}



int BucketStruct::gain_to_idx(int gain) {
    return gain + this->pmax;
}