#ifndef NETTABLE
#define NETTABLE

#include <fstream>

#include "node_table.hpp"
#include "net.hpp"
class NodeTable;
class Net;

#define NET_HEADER_SIZE 5 // Size of header for a nets file

class NetTable {
    private:
        int size;
        Net** table;

    public:
        NetTable(int size=1);
        ~NetTable();

        void print(std::fstream& file, bool nodes=false, bool partition=false, bool p_nodes=false);

        int get_size();
        void set_size(int size);

        Net* get_net(int index);
        void set_net(int index, Net* net);

        Net** get_nets();
        void set_nets(Net** net_list);

        void load_table(char* benchmark, NodeTable& nodes);
        void init_partitions();
};

#endif