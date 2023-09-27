#include <thread>
#include <iostream>

int main()
{

#pragma omp parallel
    {
#pragma omp critical (cout)
        {
            std::cout << std::this_thread::get_id() << std::endl;
        }
    }

#pragma omp parallel
    {
        std::cout << '.';
    }

    std::cout << std::endl;
    return 0;
}
