#include <thread>
#include <iostream>
#include <fstream>

#include "omp.h"

int main()
{
#ifdef _OPENMP
    std::cout << "OpenMP is supported!\n";
#endif

#pragma omp parallel num_threads(4)
    {
        const std::string fname{
            std::to_string(
                omp_get_thread_num()
                ) + ".txt"
        };

        std::ofstream fout{ fname };
        if (!fout.is_open()) {
#pragma omp critical
            std::cout << "file " << fname << " not open";
        }

        fout << ".";
    }

    std::cout << std::endl;
    return 0;
}
