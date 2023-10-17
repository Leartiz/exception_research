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

        std::cout << "\t<create some different length vectors>" << std::endl;

        {
            auto beg = system_clock::now();
            for (int i = 0; i < exp_count; ++i) {
                std::vector<int> any_vector(0);
                static_cast<void>(any_vector.size());
            }
            auto end = system_clock::now();

            auto dur = duration_cast<time_unit>(end - beg).count();
            std::cout << "create 0000 elements:\t" << dur << std::endl;
        }
        {
            const int cur_len = 5;
            auto beg = system_clock::now();
            for (int i = 0; i < exp_count; ++i) {
                std::vector<int> any_vector(cur_len);
                static_cast<void>(any_vector.size());
            }
            auto end = system_clock::now();

            auto dur = duration_cast<time_unit>(end - beg).count();
            std::cout << "create 0005 elements:\t" << dur << std::endl;
        }
        {
            const int cur_len = 1'000;
            auto beg = system_clock::now();
            for (int i = 0; i < exp_count; ++i) {
                std::vector<int> any_vector(cur_len);
                static_cast<void>(any_vector.size());
            }
            auto end = system_clock::now();

            auto dur = duration_cast<time_unit>(end - beg).count();
            std::cout << "create 1000 elements:\t" << dur << std::endl;
        }
    }
    std::cout << "\n***\n\n";

    // -------------------------------------------------------------------

    {
        using time_unit = nanoseconds;

        {
            std::cout << "\t<create almost empty vector>" << std::endl;

            auto beg = system_clock::now();
            std::vector<int> any_vector{ 0 };
            static_cast<void>(any_vector.front());
            auto end = system_clock::now();

            std::cout << "vec.capacity: " << any_vector.capacity() << std::endl;
            std::cout << "vec.size: " << any_vector.size() << std::endl;
            std::cout << "dur vec:\t" << duration_cast<time_unit>(
                                         end - beg).count() << std::endl;
        }
        std::cout << "***" << std::endl;
        {
            std::cout << "\t<create runtime exception>" << std::endl;

            auto beg = system_clock::now();
            std::runtime_error any_err{ "" };
            static_cast<void>(any_err.what());
            auto end = system_clock::now();

            std::cout << "err.what: " << any_err.what() << std::endl;
            std::cout << "dur err:\t" << duration_cast<time_unit>(
                                             end - beg).count() << std::endl;
        }
    }
    std::cout << "\n***\n\n";

    // -------------------------------------------------------------------

    {
        using time_unit = microseconds;
        const int exp_count{ 1'000'000 };
        long long dur_errs{ 0 };
        long long dur_vecs{ 0 };

        {
            std::cout << "\t<create some almost empty vectors>" << std::endl;

            auto beg = system_clock::now();
            for (int i = 0; i < exp_count; ++i) {
                {
                    std::vector<int> any_vector{ 0 };
                    static_cast<void>(any_vector.front());
                }
            }
            auto end = system_clock::now();

            dur_vecs = duration_cast<time_unit>(end - beg).count();
            std::cout << "dur vecs:\t" << dur_vecs << std::endl;
        }
        std::cout << "***" << std::endl;
        {
            std::cout << "\t<create some runtime exceptions>" << std::endl;

            auto beg = system_clock::now();
            for (int i = 0; i < exp_count; ++i) {
                {
                    std::runtime_error any_err{ "" }; // <--- !
                    static_cast<void>(any_err.what());
                }
            }
            auto end = system_clock::now();

            dur_errs = duration_cast<time_unit>(end - beg).count();
            std::cout << "dur errs:\t" << dur_errs << std::endl;
        }

        /* time difference depends on the length of the error message. */
        std::cout << "common difference dur: "
                  << std::abs(dur_vecs - dur_errs)
                  << std::endl;
    }
    return 0;
}
