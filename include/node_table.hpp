#ifndef NODETABLE
#define NODETABLE

#include "node.hpp"
#include "bucket_struct.hpp"
class Node;
class BucketStruct;

#define NODE_HEADER_SIZE 5 // Size of the header for a nodes file

/*
 * Node table class for storing all nodes in an array (maybe tree)
 */
class NodeTable {
    private:
        int size;  // Number of nodes in table
        Node** table; // Table of nodes

        int o_pos; // Positions of o-nodes
        int p_pos; // Positions of p-nodes

        int locked; // number of locked nodes 

    public:

        NodeTable(int size=1);
        ~NodeTable();

        void print(std::fstream& file, bool dim=false, bool net=false, bool partition=false, bool gain=false, bool lock=false);

        int get_size();
        void set_size(int size);

        Node* get_node(int index);
        Node* get_node(std::string name);
        void set_node(int index, Node* node);

        Node** get_nodes();
        void set_nodes(Node** node_list);

        void load_table(char* benchmark);
        void init_partitions();
        void init_gains(BucketStruct* bucket); // Return max gain
        int max_degree();

        void unlock();
        void inc_lock();
        int get_lock();
};


#endif