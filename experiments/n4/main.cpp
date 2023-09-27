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

#pragma omp critical (write_some)
    {
        std::cout << '.' << std::endl;
    }

    const int id =
        omp_get_thread_num();

    // correct to use std::exception_ptr...
    if (id == 3) {
        throw std::runtime_error{
            "something wrong"
        };
    }
}

int main()
{
    try {
#pragma omp parallel num_threads(6)
        {
            routine();
        }
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << "\n";
    }
    return 0;
}
