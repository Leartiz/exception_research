#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <chrono>
#include <thread>

std::string rand_str(const size_t n = 10)
{
    static const char avb[] =
        "abcdefghijklmnopqrstuvwxyz";

    std::string result;
    result.reserve(n);

    for (size_t i = 0; i < n; ++i)
        result.push_back(
            avb[std::rand() % sizeof(avb)]);

    return result;
}

void useful_action()
{
    std::this_thread::sleep_for(
        std::chrono::microseconds(1));
}

void vector_as_exception()
{
    useful_action();

    throw std::vector<std::string>{
        rand_str()
    };
}

void exception_as_exception()
{
    useful_action();

    throw std::runtime_error{
        rand_str()
    };
}

// -----------------------------------------------------------------------

int main()
{
    const int exp_count = 10'000'000;

    // ***

    using namespace std::chrono;
    using time_unit = microseconds;
    {
        const auto beg = system_clock::now();
        for (int i = 0; i < exp_count; ++i)
            try {
                vector_as_exception();
            }
            catch (const std::vector<std::string>& err) {
                static_cast<void>(err.front());
            }
        const auto end = system_clock::now();

        const auto dur = duration_cast<time_unit>(end - beg).count();
        std::cout << "vec as err dur: " << dur << std::endl;
    }

    // ***

    {
        const auto beg = system_clock::now();
        for (int i = 0; i < exp_count; ++i)
            try {
                exception_as_exception();
            }
            catch (const std::runtime_error& err) {
                static_cast<void>(err.what());
            }
        const auto end = system_clock::now();

        const auto dur = duration_cast<time_unit>(end - beg).count();
        std::cout << "err as err dur: " << dur << std::endl;
    }
}
