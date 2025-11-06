#ifndef PARTITION
#define PARTITION

#include "node_table.hpp"
#include "net_table.hpp"
#include "bucket_struct.hpp"
#include "move_vector.hpp"

#include <ctime>
#include <fstream>

#define MODE_NONE 0
#define MODE_NODE 1
#define MODE_AREA 2

#define TOLERANCE_SIZE 0.51 // max nodes in a partition
#define TOLERANCE_AREA 0.55 // max area in a partition

#define MAX_PASSES 100 // max number of FM passes

class Partition {
    private:   
        bool timing, dump, log;
        std::clock_t start_time, prev_time;

        char* benchmark;
        std::fstream log_file;

        int cut_size;
        int p1_size, p2_size, max_size;
        int p1_area, p2_area, max_area;
        int mode;

        NodeTable* nodes;
        NetTable* nets;
        BucketStruct* bucket;
        MoveVector* move_vector;

    public:
        Partition(bool timing, bool dump, bool log, char* benchmark, int mode);
        ~Partition();

        void print_time();
        void print_total();
        void print_lock();

        void dump_partitions();

        void calculate_cut();
        void modify_cut(int count);

        void calculate_sizes();
        void modify_sizes(int count, int p);

        void calculate_areas();
        void modify_areas(int value, int p);

        void init_nodes();
        void init_nets();
        void init_partitions();
        void init_bucket();
        void init_move_vector();

        void move(Node* node);

        void restore_moves(int count);

        void run();
};

#endif