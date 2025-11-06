#ifndef MV_VEC
#define MV_VEC

#include "node.hpp"
class Node;

class MoveVector {
    private:
        class move_node { // Stores the node that was moved that iteration
            private:
                Node* node;
                int cut_size;

            public:
                move_node();

                void set_node(Node* node);
                Node* get_node();

                void set_cut(int cut_size);
                int get_cut();

                void reset();
        };

        int size;
        move_node** move_table;

        int min_cut;
        int min_pos;

    public:
        MoveVector(int size);
        ~MoveVector();

        void print(std::fstream& file);

        void set_move(int idx, Node* node, int cut);

        Node* get_node(int idx);

        int get_size();
        int get_min_cut();
        int get_min_pos();

        void reset();
};

#endif