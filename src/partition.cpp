#include "../include/partition.hpp"

#include <ctime>
#include <string.h>

Partition::Partition(bool timing, bool dump, bool log, char* benchmark, int mode) :
                    timing(timing), dump(dump), log(log), mode(mode) {
    
    this->benchmark = (char*)malloc(sizeof(char) * (strlen(benchmark) + 1));
    strcpy(this->benchmark, benchmark);

    if (this->timing) {
        this->start_time = std::clock();
        this->prev_time = this->start_time;
    }

    if (this->log)
        this->log_file.open("./partition.log", std::fstream::out | std::fstream::trunc);

    this->init_nodes();
    this->init_nets();
    this->init_partitions();
    this->init_bucket();
    this->init_move_vector();
}

Partition::~Partition() {
    free(this->benchmark);
    
    if (this->log)
        this->log_file.close();

    delete this->nodes;
    delete this->nets;
    delete this->bucket;
    delete this->move_vector;
}



void Partition::print_time() {
    if (this->timing) {
        std::clock_t current_time = std::clock();
        double elapsed_time = static_cast<double>(current_time - this->prev_time) / CLOCKS_PER_SEC;
        printf("Took %lf seconds\n", elapsed_time);
        if (this->log)
            this->log_file << "Took " << elapsed_time << " seconds" << std::endl << std::endl;
        this->prev_time = current_time;
    }
}

void Partition::print_total() {
    if (this->timing) {
        std::clock_t current_time = std::clock();
        double elapsed_time = static_cast<double>(current_time - this->start_time) / CLOCKS_PER_SEC;
        printf("\nTotal time: %lf seconds\n", elapsed_time);
        if (this->log)
            this->log_file << "Total time: " << elapsed_time << " seconds" << std::endl;
    }
}



void Partition::dump_partitions() {
    std::fstream dump_file;
    dump_file.open("./partition.dmp", std::fstream::out | std::fstream::trunc);
    dump_file << "Partition information for " << this->benchmark << std::endl;
    dump_file << std::endl << "P1:" << std::endl;
    for (int i = 0; i < this->nodes->get_size(); i++) {
        Node* node = this->nodes->get_node(i);
        if (node->get_partition() == P1)
            node->print(dump_file, false, false, false, false, false);
    }
    dump_file << std::endl << "P2:" << std::endl;
    for (int i = 0; i < this->nodes->get_size(); i++) {
        Node* node = this->nodes->get_node(i);
        if (node->get_partition() == P2)
            node->print(dump_file, false, false, false, false, false);
    }
    dump_file.close();
}



void Partition::calculate_cut() {
    int cut_size = 0;
    for (int i = 0; i < this->nets->get_size(); i++) {
        if (this->nets->get_net(i)->get_criticality())
            cut_size++;
    }
    this->cut_size = cut_size;
}

void Partition::modify_cut(int count) {
    this->cut_size -= count;
}



void Partition::calculate_sizes() {
    this->p1_size = this->p2_size = 0;
    for (int i = 0; i < this->nodes->get_size(); i++) {
        Node* node = this->nodes->get_node(i);
        if (node->get_partition() == P1)
            this->p1_size++;
        else
            this->p2_size++;
    }
}

void Partition::modify_sizes(int count, int p) {
    this->p1_size += (p == P1) ? count : -count;
    this->p2_size += (p == P1) ? -count : count;
}



void Partition::calculate_areas() {
    this->p1_area = this->p2_area = 0;
    for (int i = 0; i < this->nodes->get_size(); i++) {
        Node* node = this->nodes->get_node(i);
        if (node->get_partition() == P1)
            this->p1_area += node->get_area();
        else
            this->p2_area += node->get_area();
    }
}

void Partition::modify_areas(int area, int p) {
    this->p1_area += (p == P1) ? area : -area;
    this->p2_area += (p == P1) ? -area : area;
}



void Partition::init_nodes() {
    printf("\nInitializing nodes...\n");
    if (this->log)
        this->log_file << "Initializing nodes..." << std::endl;
    this->nodes = new NodeTable();
    this->nodes->load_table(this->benchmark);
    printf("Node initialization complete\n");
    if (this->log) {
        this->log_file << "Node initialization complete" << std::endl;
        this->nodes->print(this->log_file, true, false, false, false, false);
    }
    this->print_time();
}

void Partition::init_nets() {
    printf("\nInitializing nets...\n");
    if (this->log)
        this->log_file << "Initializing nets..." << std::endl;
    this->nets = new NetTable();
    nets->load_table(this->benchmark, *this->nodes);
    printf("Net initialization complete\n");
    if (this->log) {
        this->log_file << "Net initialization complete" << std::endl;
        this->nets->print(this->log_file, true, false, false);
    }
    this->print_time();
}

void Partition::init_partitions() {
    printf("\nInitializing partitions...\n");
    if (this->log)
        this->log_file << "Initializing partitions..." << std::endl;
    this->nodes->init_partitions();
    this->nets->init_partitions();
    this->calculate_cut();
    if (this->mode == MODE_NODE) {
        this->calculate_sizes();
        this->max_size = (int)((this->p1_size + this->p2_size) * TOLERANCE_SIZE);
    } else if (this->mode == MODE_AREA) {
        this->calculate_areas();
        this->max_area = (int)((this->p1_area + this->p2_area) * TOLERANCE_AREA);
    }
    printf("Partition initialization complete\n");
    if (this->log) {
        this->log_file << "Partition initialization complete" << std::endl;
        this->nodes->print(this->log_file, false, false, true, false, false);
        this->nets->print(this->log_file, false, true, true);
    }
    this->print_time();
}

void Partition::init_bucket() {
    printf("\nInitializing bucket structure...\n");
    if (this->log)
        this->log_file << "Initializing bucket structure..." << std::endl;
    this->bucket = new BucketStruct(this->nodes->max_degree());
    this->nodes->init_gains(this->bucket);
    printf("Bucket structure initialization complete\n");
    if (this->log) {
        this->log_file << "Bucket structure initialization complete" << std::endl;
        this->bucket->print(this->log_file);
    }
    this->print_time();
}

void Partition::init_move_vector() {
    printf("\nInitialiing move vector...\n");
    if (this->log)
        this->log_file << "Intitializing move vector...\n" << std::endl;
    this->move_vector = new MoveVector(this->nodes->get_size());
    printf("Move vector initialization complete\n");
    if (this->log)
        this->log_file << "Move vector initialization complete" << std::endl;
    this->print_time();
}



void Partition::move(Node* node) {
    int T = node->get_other_part();
    node->set_lock();
    this->nodes->inc_lock();
    node->set_partition(T);
    this->modify_cut(node->get_gain());
    node->set_gain(-node->get_gain());
    this->bucket->remove(node);
    if (this->mode == MODE_NODE)
        this->modify_sizes(1, T);
    if (this->mode == MODE_AREA)
        this->modify_areas(node->get_area(), T);
}



void Partition::restore_moves(int count) {
    for (int i = this->move_vector->get_min_pos() + 1; i < count; i++) {
        Node* node = this->move_vector->get_node(i);
        node->set_partition(node->get_other_part());
    }
    this->cut_size = this->move_vector->get_min_cut();
}



void Partition::run() {
    printf("\nRunning partitioning algorithm...\n");
    if (this->log)
        this->log_file << "Running partitioning algorithm..." << std::endl;

    int pass_count = 0;
    int prev_cut = -1;
    while ((prev_cut == -1 || prev_cut > this->cut_size) && pass_count < MAX_PASSES) {
        int node_count = 0;
        prev_cut = this->cut_size;
        printf("FM pass %d...\n", pass_count);
        while (this->nodes->get_lock() != this->nodes->get_size()) {
            if (this->log) {
                this->log_file << "FM Pass " << pass_count <<  " - " << node_count << std::endl;
                this->nodes->print(this->log_file, false, false, false, false, true); // print lock
                this->bucket->print(this->log_file);
                this->log_file << "Cut Size: " << this->cut_size << std::endl << std::endl;
            }

            Node* base_cell;
            if (this->mode == MODE_NONE) // Get node with max gain that meets constraints
                base_cell = this->bucket->get_max();
            else if (this->mode == MODE_NODE)
                base_cell = this->bucket->get_max_size(this->p1_size, this->p2_size, this->max_size);
            else if (this->mode == MODE_AREA)
                base_cell = this->bucket->get_max_area(this->p1_area, this->p2_area, this->max_area);
            if (!base_cell) { // If a cell is repeated, then break (cant meet constraint)
                if (this->log)
                    this->log_file << "Cell " << base_cell->get_name() << " repeated, ending pass" << std::endl;
                break;
            }
            if (this->log) {
                this->log_file << "Max gain found..." << std::endl;
                base_cell->print(this->log_file, false, false, true, true, false);
            }


            int F = base_cell->get_partition();
            int T = base_cell->get_other_part();
            this->move(base_cell);
            if (this->log)
                this->log_file << "Moved cell " << base_cell->get_name() << " to partition " << base_cell->get_partition() << std::endl << std::endl;

            if (this->log)
                this->log_file << "Analyzing nets connected to " << base_cell->get_name() << "..." << std::endl;
            base_cell->init_iter();
            for (int i = 0; i < base_cell->get_net_count(); i++) {
                Net* net = base_cell->get_next_net();
                if (this->log)
                    net->print(this->log_file, false, true, true);
                
                if (net->T(F) == 0)
                    net->inc_gains(this->log_file, this->bucket, base_cell, F);
                else if (net->T(F) == 1)
                    net->dec_gains(this->log_file, this->bucket, base_cell, T);

                net->dec_F(F);
                net->inc_T(F);
                net->update_criticality();
                if (this->log) {
                    this->log_file << "Changed partition counts on net " << net->get_name() << std::endl;
                    net->print(this->log_file, false, true, true);
                }

                if (net->F(F) == 0)
                    net->dec_gains(this->log_file, this->bucket, base_cell, T);
                else if (net->F(F) == 1)
                    net->inc_gains(this->log_file, this->bucket, base_cell, F);

                if (this->log) this->log_file << std::endl;
                base_cell->iter_next();
            }
            this->move_vector->set_move(node_count++, base_cell, this->cut_size);
            if (this->log) {
                this->log_file << "Node move " << node_count << " finished" << std::endl
                            << "Cut size: " << this->cut_size << std::endl;
                this->print_time();
            }
        }
        if (this->log) {
            this->log_file << "Pass " << pass_count << " finished" << std::endl;
            this->move_vector->print(this->log_file);
            this->log_file << "Last cut: " << prev_cut << std::endl
                        << "Best cut: " << this->move_vector->get_min_cut() << std::endl
                        << "Move position: " << this->move_vector->get_min_pos() << std::endl
                        << "Resetting to min cut..." << std::endl;
        }


        printf("Pass %d finished\n", pass_count);
        //this->print_time();
        printf("Last cut: %d\n", prev_cut);
        printf("Best cut: %d\n", this->move_vector->get_min_cut());
        printf("Resetting to min cut...\n");
        this->restore_moves(node_count);
        this->move_vector->reset();
        this->nodes->unlock();
        this->nets->init_partitions();
        this->nodes->init_gains(this->bucket);
        if (this->mode == MODE_NODE)
            this->calculate_sizes();
        if (this->mode == MODE_AREA)
            this->calculate_areas();
        printf("Reset complete\n\n");
        pass_count++;
    }

    printf("Partitioning complete\n");
    printf("Took %d passes\n", pass_count);
    printf("Cut Size: %d\n", this->cut_size);
    switch(this->mode) {
        case MODE_NONE:
            this->calculate_sizes();
        case MODE_NODE:
            printf("P1: %d\n", this->p1_size);
            printf("P2: %d\n", this->p2_size);
            break;
        case MODE_AREA:
            printf("P1: %d\n", this->p1_area);
            printf("P2: %d\n", this->p2_area);
            break;
        default:
            break;
    }
    if (this->log) {
        this->log_file << "Partitioning complete" << std::endl;
        nodes->print(this->log_file, false, false, true, false, false);
        bucket->print(this->log_file);
        this->log_file << "Cut Size: " << this->cut_size << std::endl;
    }
    this->print_time();
    this->print_total();
    if (this->dump)
        this->dump_partitions();
}