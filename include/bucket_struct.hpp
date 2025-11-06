#ifndef BUCKET
#define BUCKET

#include "bucket_link.hpp"
#include "node.hpp"
class Link;
class Node;

#include <fstream>

class BucketStruct {
    private:
        int pmax;
        int max;
        Link** table;

    public:
        BucketStruct(int pmax);
        ~BucketStruct();

        void print(std::fstream& file);
        void print_bucket(std::fstream& file, int bucket);

        int get_pmax();
        void set_pmax(int pmax);

        Link** get_table();
        void set_table(Link** table);

        Link* get_bucket(int idx);
        Link* get_bucket(Node* node);
        Link* get_last(Link* bucket);
        void empty_bucket(int idx);

        void insert(Node* node);
        void move(Node* node); // Move node's link
        void remove(Node* node); // Remove node's link

        Node* get_max();      // gets max gain node
        Node* get_max_size(int p1_size, int p2_size, int max_size); // gets max gain node that meets size constraints
        Node* get_max_area(int p1_area, int p2_area, int max_area); // gets max gain node that meets area constraints
        void find_max();

        int gain_to_idx(int gain); // Converts the gain into the correct idx for the table

        

};

#endif