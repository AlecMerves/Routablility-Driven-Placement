#ifndef NODE
#define NODE

#include "net.hpp"
class Net;
class Link;

#include <string>

#define P1 1
#define P2 2

/*
 * Class for holding the node data. Each node is an object 
 * with information about that node, including:
 * - name
 * - area
 * - delay (?)
 * - partition
 * - gain
 * - lock status
 * - net list
 */

class Node {
    private:
        class net_link {
            private:
                Net* net;
                net_link* next;
            
            public:
                net_link(Net* net);
                ~net_link();

                Net* get_net();
                void set_net(Net* net);

                net_link* get_next();
                void set_next(net_link* next);
        };

        char* name;
        int id;

        int height;
        int width;
        int area;

        int partition;
        int lock_status;

        int gain;
        Link* gain_ptr; // pointer to the link for this node within the bucket structure

        int net_count;
        net_link* net_list; 

        net_link* iter;

    public:
        // Constructors
        Node(const char* name, int id, int width=0, int height=0);
        Node(std::string name, int id, int width=0, int height=0);
        ~Node();

        // Debugging
        void print(std::fstream& file, bool dim, bool net, bool partition, bool gain, bool lock);

        // Member methods

        // Name getter
        const char* get_name();

        // ID getter
        const int get_id();

        // Width getter/setter
        int get_width();
        void set_width(int width);

        // Height getter/setter
        int get_height();
        void set_height(int height);

        // Area getter
        int get_area();

        // Partition getter/setter
        int get_partition();
        void set_partition(int partition);
        int get_other_part();

        // Lock getter and set/reset
        int get_lock(); 
        void set_lock();
        void reset_lock();

        // Gain getters/setters
        int get_gain();
        void set_gain(int gain);
        Link* get_gain_ptr();
        void set_gain_prt(Link* gain_ptr);
        void update_gain();
        void modify_gain(int change);
        
        // Net getter/setters
        int get_net_count();
        void set_net_count(int count);

        net_link* get_net();
        void set_net(net_link* net_list); // TODO: methods to add nets to the nets list
        void add_net(Net* net);

        Net* get_next_net();
        void init_iter();
        void iter_next();
};

#endif