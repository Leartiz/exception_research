#include <iostream>
#include <stdexcept>
#include <thread>

#include "omp.h"

void routine()
{
    const auto dur = std::rand() % 5;
    std::this_thread::sleep_for(
        std::chrono::seconds(
            dur));

    const int id = omp_get_thread_num();
    if (id == 3) {
        throw std::runtime_error{
            "failed"
        };
    }
}

int main()
{
    try {
#pragma omp parallel num_threads(4)
        {
            routine();
        }
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << "\n";
    }
    return 0;
}
