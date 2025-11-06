#ifndef LINK
#define LINK

#include "node.hpp"
class Node;

#include <fstream>

class Link {
    private:
        Node* node;
        Link* next;
        Link* prev;
        int bucket;
    
    public:
        Link(Node* node, int idx);
        Link(Node* node, int idx, Link* next, Link* prev);
        ~Link();

        void print(std::fstream& file);

        Node* get_node();
        void set_node(Node* node);

        Link* get_next();
        void set_next(Link* link);

        Link* get_prev();
        void set_prev(Link* link);

        int get_bucket();
        void set_bucket(int idx);

};

#endif