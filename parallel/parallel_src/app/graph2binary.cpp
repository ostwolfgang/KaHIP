#include <stdio.h>
#include <iostream>
#include "io/parallel_graph_io.h"

using namespace std;

const long fileTypeVersionNumber = 2;
const long header_count    = 3;

int main(int argn, char **argv)
{
        std::cout <<  "program converts a METIS graph file into a binary (distributed graph format) file. "  << std::endl;

        MPI_Init(&argn, &argv);    /* starts MPI */

        int rank, size;
        MPI_Comm_rank( MPI_COMM_WORLD, &rank);
        MPI_Comm_size( MPI_COMM_WORLD, &size);

        if(argn != 3) {
                if( rank == ROOT ) {
                        std::cout <<  "usage: " ;
                        std::cout <<  "graph2binary metisfile outputfilename"  << std::endl;
                }
                MPI_Finalize();
                return 0;
        }

        if( size > 1 ) {
                std::cout <<  "currently only one process supported."  << std::endl;
                MPI_Finalize();
                return 0;
        }

        string graph_filename(argv[1]);
        string filename(argv[2]);

        std::cout <<  "Reading graph " << graph_filename  << std::endl;

        parallel_graph_access G;
        parallel_graph_io::readGraphWeightedMETISFast(G, graph_filename, rank, size);
        parallel_graph_io::writeGraphSequentiallyBinary(G, filename);

        MPI_Finalize();
        return 0;
}
