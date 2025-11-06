Fidducia-Mattheyses Partitioning Algorithm with node and area constriants.
Authors: Alec Merves & Brian Park

INSTALLATION
If Make is available on your device:
Just enter "make" or "make all" into the command prompt in this directory.

If Make is not available on your device:
Compile each file in the ./src folder into object (.o) files and then compile the main.cpp file, linking each object
    
If GCC is installed on your device, the following commands can be used:    
g++ -c src/bucket_link.cpp -o obj/bucket_link.o
g++ -c src/bucket_struct.cpp -o obj/bucket_struct.o
g++ -c src/move_vector.cpp -o obj/move_vector.o
g++ -c src/net.cpp -o obj/net.o
g++ -c src/net_table.cpp -o obj/net_table.o
g++ -c src/node.cpp -o obj/node.o
g++ -c src/node_table.cpp -o obj/node_table.o
g++ -c src/partition.cpp -o obj/partition.o
g++ -O3 main.cpp -o main  obj/bucket_link.o  obj/bucket_struct.o  obj/move_vector.o  obj/net.o  obj/net_table.o  obj/node.o  obj/node_table.o  obj/partition.o

BENCHMARKS
To run a benchmark, place a folder with the same name as the benchmark into the ./benchmarks folder
To run the superblue benchmarks, place each superblue# folder into the ./benchmarks folder

USAGE
To run the program, enter ./main into the command line with the approiate flags

OPTIONS
--super #       - Run a superblue benchmark, cannot be used with the --bench option
--bench name    - Run a custom/other benchmark, cannot be used with the --super option

--node          - Impose a node restriction on the algorithm, cannot be used with the --area option
--area          - Impose an area restriction on the algorithm, cannot be used with the --node option

-t              - Display timing information, will also log all timing information if the -l flag is supplied
-d              - Dump the final parition data into a file. The file will be ./parition.dmp
-l              - Log paritioning information into a file. The file will be ./parition.log
-h              - Show help information.
