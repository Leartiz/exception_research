#include <iostream>
#include <stdio.h>

#include "omp.h"

int main()
{
    {
        double start_time, end_time, tick;
        start_time = omp_get_wtime();
        end_time = omp_get_wtime();
        tick = omp_get_wtick();

        // ***

        printf(
            "Время на замер времени %lf\n",
            end_time - start_time
            );
        printf(
            "Точность таймера %lf\n",
            tick
            );
    }

    std::cout << "***" << std::endl;

    {
        const auto start_time = omp_get_wtime();
        const auto tick = omp_get_wtick();
        const auto end_time = omp_get_wtime();

        std::cout << "dur: "
                  << end_time - start_time << std::endl;

        std::cout << "tick: " << tick << std::endl;
    }

    return 0;
}
