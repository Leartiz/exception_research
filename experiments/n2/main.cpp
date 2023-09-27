#include <thread>
#include <iostream>

#include "omp.h"

int main()
{
    omp_set_nested(1);
    {
#pragma omp parallel num_threads(4)
        {
#pragma omp parallel num_threads(4)
            {
#pragma omp critical (cout)
                {
                    std::cout << std::this_thread::get_id() << std::endl;
                }
            }
        }
    }
    omp_set_nested(0);

    std::cout << std::endl;
    return 0;
}
