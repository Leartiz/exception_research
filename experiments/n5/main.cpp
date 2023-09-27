#include <iostream>
#include <thread>
#include <chrono>

#include "omp.h"

int main()
{
    srand(
        time(
            nullptr));

    const auto beg = omp_get_wtime();

#pragma omp parallel
    {
        std::this_thread::sleep_for(
            std::chrono::seconds(
                std::rand() % 5));
    }

    const auto end = omp_get_wtime();

    std::cout << "dur: " << end - beg
              << std::endl;

    return 0;
}
