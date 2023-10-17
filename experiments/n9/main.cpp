#include <iostream>
#include <stdexcept>
#include <vector>
#include <chrono>

using namespace std;

int main()
{
    using namespace chrono;

    // -------------------------------------------------------------------

    {
        using time_unit = nanoseconds;
        const int exp_count{ 1'000'000 };
        {
            auto beg = system_clock::now();
            for (int i = 0; i < exp_count; ++i) {
                std::vector<int> any_vector(0);
                static_cast<void>(any_vector);
            }
            auto end = system_clock::now();

            auto dur = duration_cast<time_unit>(end - beg).count();
            std::cout << "create 0000 elements:\t" << dur << std::endl;
        }
        {
            auto beg = system_clock::now();
            for (int i = 0; i < exp_count; ++i) {
                std::vector<int> any_vector(5);
                static_cast<void>(any_vector);
            }
            auto end = system_clock::now();

            auto dur = duration_cast<time_unit>(end - beg).count();
            std::cout << "create 0005 elements:\t" << dur << std::endl;
        }
        {
            auto beg = system_clock::now();
            for (int i = 0; i < exp_count; ++i) {
                std::vector<int> any_vector(1000);
                static_cast<void>(any_vector);
            }
            auto end = system_clock::now();

            auto dur = duration_cast<time_unit>(end - beg).count();
            std::cout << "create 1000 elements:\t" << dur << std::endl;
        }
    }
    std::cout << "***" << std::endl;

    // -------------------------------------------------------------------

    {
        auto beg = system_clock::now();
        std::vector<int> any_vector(0);
        auto end = system_clock::now();

        std::cout << "vec.capacity: " << any_vector.capacity() << std::endl;
        std::cout << "vec.size: " << any_vector.size() << std::endl;
        std::cout << "dur:\t" << duration_cast<nanoseconds>(
                         end - beg).count() << std::endl;
    }
    std::cout << "***" << std::endl;
    {
        auto beg = system_clock::now();
        std::runtime_error any_err{ "" };
        auto end = system_clock::now();

        std::cout << "dur:\t" << duration_cast<nanoseconds>(
                         end - beg).count() << std::endl;
    }
    std::cout << "***" << std::endl;

    // -------------------------------------------------------------------

    {
        using time_unit = nanoseconds;
        const int exp_count{ 10'000'000 };
        long long dur_errs{ 0 };
        long long dur_vecs{ 0 };

        {
            auto beg = system_clock::now();
            for (int i = 0; i < exp_count; ++i) {
                std::runtime_error any_err(""); // <--- !
                static_cast<void>(any_err);
            }
            auto end = system_clock::now();

            dur_errs = duration_cast<time_unit>(end - beg).count();
            std::cout << "dur_errs:\t" << dur_errs << std::endl;
        }
        std::cout << "***" << std::endl;
        {
            auto beg = system_clock::now();
            for (int i = 0; i < exp_count; ++i) {
                std::vector<int> any_vector(0);
                static_cast<void>(any_vector);
            }
            auto end = system_clock::now();

            dur_vecs = duration_cast<time_unit>(end - beg).count();
            std::cout << "dur_vecs:\t" << dur_vecs << std::endl;
        }

        /* time difference depends on the length of the error message. */
        std::cout << "diff: " << std::abs(dur_vecs - dur_errs) << std::endl;
    }
    return 0;
}
