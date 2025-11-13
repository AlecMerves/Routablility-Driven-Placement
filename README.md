# Routability-Driven Placement

This project started as a class project for a automated electronic design course I took in the spring of the senior year at Stony Brook University.
The goal of the project was to implement the Fiduccia-Mattheyses partitioning algorithm in C++ and use it to partition circuits from the ISPD 2011 competition. I now plan to continue this project by implementing placement and routing algorithms in my personal time.

Currently this project only implements the partitioning step of circuit automation.

## COMPILING

This program was written on a Windows 10 machine with the GCC compiler. The makefile uses UNIX commands from Bash.
Maybe at a future date these will be made platform independent.

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

## BENCHMARKS

To run a benchmark, place a folder with the same name as the benchmark into the ./benchmarks folder
To run the superblue benchmarks, place each superblue# folder into the ./benchmarks folder (can be found at the ISPD [website](https://www.ispd.cc/contests/11/ispd2011_contest.html))

## USAGE

To run the program, enter ./main into the command line with the approiate flags

## OPTIONS

--super # &emsp;&emsp;&emsp;&emsp;- Run a superblue benchmark, cannot be used with the --bench option  
--bench name&emsp;&emsp;- Run a custom/other benchmark, cannot be used with the --super option  

--node&emsp;&emsp;&emsp;&emsp;&emsp;&ensp;- Impose a node restriction on the algorithm, cannot be used with the --area option  
--area&emsp;&emsp;&emsp;&emsp;&emsp;&ensp; - Impose an area restriction on the algorithm, cannot be used with the --node option  

 -t&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;- Display timing information, will also log all timing information if the -l flag is supplied  
 -d&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&ensp; - Dump the final parition data into a file. The file will be ./parition.dmp  
 -l&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;- Log paritioning information into a file. The file will be ./parition.log  
 -h&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&ensp; - Show help information.  
