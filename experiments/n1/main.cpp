#include <thread>
#include <iostream>

#include "omp.h"

int main()
{
#pragma omp parallel
    {
#pragma omp critical (cout)
        {
            std::cout << std::this_thread::get_id() << std::endl;
        }
    }

    std::cout << "***\n";

#pragma omp parallel num_threads(8)
        {
#pragma omp critical (cout)
            {
                std::cout << omp_get_thread_num() << std::endl;
            }
        }

    std::cout << "***\n";

#pragma omp parallel
    {
        std::cout << '.';
    }

    std::cout << std::endl;
    return 0;
}
