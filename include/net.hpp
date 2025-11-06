#ifndef NETS
#define NETS

#include "node.hpp"
#include "bucket_struct.hpp"
class Node; // Resolves circular dependency
class BucketStruct;

#include <string>

/*
 * Class for holding the net data. Each net is an object
 * with information about that net, including:
 * - name
 * - partition data (T and F)
 * - criticality
 * - node list 
 */
class Net {
    private:
        char* name;

        int p1; // partition counts
        int p2;

        bool critical; // criticality

        int node_count;
        Node** node_list; // array of nodes connected to this net
    
    public:
        // Constructors
        Net(const char* name, int count);
        Net(std::string name, int count);
        ~Net();

        // Debugging
        void print(std::fstream& file, bool nodes, bool partition, bool p_nodes);

        // Member methods
        const char* get_name();

        int get_p1();
        void set_p1(int count);

        int get_p2();
        void set_p2(int count);

        bool get_criticality();
        void update_criticality();

        int get_node_count();
        void set_node_count(int count);

        Node* get_node(int index);
        void set_node(int index, Node* node);

        Node** get_nodes();
        void set_nodes(Node** node_list);

        int T(int F); // Functions to compute T(n) and F(n) given a partition
        int F(int F);

        void dec_F(int F);
        void inc_T(int F);

        void inc_gains(std::fstream& file, BucketStruct* bucket, Node* base_node, int p);
        void dec_gains(std::fstream& file, BucketStruct* bucket, Node* base_node, int p);

        void init_partitions();
};

#endif