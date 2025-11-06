#include "include/partition.hpp"

#include <string.h>
#include <iostream>

#define MAX_FILE_PATH 100

/*  FLAGS
 * -t : show timing information
 * -d : dump partition data
 * -l : log processing data to file
 * -h : show help information
 * 
 * --super (num) : run superblue benchmark
 * --bench (benchmark_name) : run custom benchmark (not a superblue)
 * 
 * --node : impose partition size restrictions
 * --area : impose area restrictions
 */

int main(int argc, char* argv[]) {
    char benchmark[MAX_FILE_PATH] = "", user_confirm;
    int mode = MODE_NONE;
    bool timing, dump, log, help, super_op;
    timing = dump = log = help = super_op = false;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            printf("[ERROR] Improper use of flags: %s\n", argv[i]);
            exit(1);
        }

        if (argv[i][1] == '-') {
            if (strcmp(argv[i], "--super") == 0) {
                if (strcmp(benchmark, "") != 0) {
                    printf("[ERROR] Invalid use of options, only supply one occrance of --super or --bench\n");
                    exit(1);
                }
                int super = atoi(argv[++i]);
                switch (super) {
                    case 1: case 2: case 4:
                    case 5: case 10: case 12:
                    case 15: case 18: break;
                    default:
                        printf("[ERROR] Invalid superblue benchmark number (1, 2, 4, 5, 10, 12, 15, 18): %s %s\n", argv[i-1], argv[i]);
                        exit(1);
                }
                super_op = true;
                sprintf(benchmark, "superblue%d", super);
            } else if (strcmp(argv[i], "--bench") == 0) {
                if (strcmp(benchmark, "") != 0) {
                    printf("[ERROR] Invalid use of options, only supply one occrance of --super or --bench\n");
                    exit(1);
                }
                if (i == argc - 1 || strcmp(argv[++i], "") == 0) {
                    printf("[ERROR] No supplied benchmark with --bench option\n");
                    exit(1);
                }
                sprintf(benchmark, "%s", argv[i]);
            } else if (strcmp(argv[i], "--node") == 0) {
                if (mode != MODE_NONE) {
                    printf("[ERROR] Invalid use of options, only supply one occurance of --node or --area\n");
                    exit(1);
                }
                mode = MODE_NODE;
            } else if (strcmp(argv[i], "--area") == 0) {
                if (mode != MODE_NONE) {
                    printf("[ERROR] Invalid use of options, only supply one occurance of --node or --area\n");
                    exit(1);
                }
                mode = MODE_AREA;
            } else {
                printf("[ERROR] Invalid flag option: %s\n", argv[i]);
                exit(1);
            }
        } else {
            for (int j = 1; j < strlen(argv[i]); j++) {
                switch (argv[i][j]) {
                    case 't': timing = true;     break;
                    case 'd': dump = true;  break;
                    case 'l': log = true; break;
                    case 'h': help = true; break;
                    default:
                        printf("[ERROR] Invalid flag option: %c\n", argv[i][j]);
                        exit(1);
                }
            }
        }
    }

    if (help)  {
        printf("\n"
                "ESE 326 FM Partitioning Project Application\n"
                "Authors: Alec Merves & Brian Park\n"
                "Usage: ./main.exe [OPTIONS] [--super OR --bench] BENCHMARK [MODE]\n"
                "Performs the Fiduccia-Mattheyses partitioning algorithm on the supplied benchmark\n\n"

                "OPTIONS:\n"
                "-t : include timing information\n"
                "-d : dump partition data to file [WARNING - USE WITH SUPERBLUE BENCHMARKS WILL RESULT IN LARGE FILE GENERATION]\n"
                "-l : log processing data to file [WARNING - USE WITH SUPERBLUE BENCHMARKS WILL RESULT IN SLOW RUN TIME]\n"
                "-h : display this information\n\n"

                "BENCHMARK OPTIONS:\n"
                "--super (num) : run superblue benchmark\n"
                "--bench (benchmark_name) : run custom benchmark (not a superblue)\n\n"

                "MODE OPTIONS:\n"
                "--node : impose partition size restrictions\n"
                "--area : impose area restrictions\n"

                "\n"
                );
        exit(0);
    }

    if (strcmp(benchmark, "") == 0) {
        printf("[ERROR] No supplied benchmark, use either the --super or --bench options, or use -h for help\n");
        exit(1);
    }

    if (super_op && dump) {
        printf("[WARNING] Use of dump feature with superblue benchmarks should only be used if viewing final partitioning is critical\n"
                "do you wish to proceed? [y/n] ");
        std::cin >> user_confirm;
        if (user_confirm != 'y') exit(0);
    }

    if (super_op && log) {
        printf("[CRITICAL WARNING] Use of log feature with superblue benchmarks is strongly advised against\n"
                "do you with to proceed? [y/n] ");
        std::cin >> user_confirm;
        if (user_confirm != 'y') exit(0);
    }

    Partition partition(timing, dump, log, benchmark, mode);
    partition.run();
}