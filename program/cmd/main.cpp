#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <exception>
#include <functional>
#include <chrono>
#include <numeric>
#include <cassert>
#include <cmath>
#include <tuple>

#include "omp.h"

struct Environment final
{
    static const int run_count = 5;
    static const int equation_count = 1'000'000;
    static const int exception_count = 1;

    static const std::vector<int> exception_case_indexs;
    static const std::vector<int> build_exception_case_indexs() {
        const int cap = exception_count;
        std::vector<int> result; result.reserve(cap);
        for (int i = 1; i <= cap; ++i) {
            result.push_back(
                equation_count / i);
        }
        std::sort(result.begin(), result.end());
        return result;
    }
    static bool is_exception_case_index(const int index) {
        return std::binary_search(
            exception_case_indexs.begin(),
            exception_case_indexs.end(),
            index
            );
    }
};

const std::vector<int> Environment::exception_case_indexs =
    build_exception_case_indexs();

// utils
// -----------------------------------------------------------------------

bool isEqual(double a, double b, double eps = 0.00001) {
    return abs(a-b) < eps;
}

double sum_vec(const std::vector<double>& vec) {
    return std::accumulate(vec.begin(), vec.end(), 0);
}

std::tuple<double, double, double>
    generate_coeffs(const int index) {
    if (!Environment::is_exception_case_index(index)) {
        return {
            ((index % 2000) - 1000) / 33.0,
            ((index % 200) - 100) / 22.0,
            ((index % 20) - 10) / 11.0
        };
    }
    else {
        return { 0, 0, 0 };
    }
}

enum class FunctionType {
    NoException, Normal, FullException
};

// -----------------------------------------------------------------------

//! решение уравнения при a, b, c одновременно не равных нулю
std::vector<double> solve_correct_equation(double a, double b, double c) noexcept {
    assert(
        !(
            isEqual(a, 0) &&
            isEqual(b, 0) &&
            isEqual(c, 0)
            )
        );

    if (isEqual(a, 0) && isEqual(b, 0)) {
        return std::vector<double>(0);
    }

    if (isEqual(a,0)) {
        return std::vector<double>{ 0, -c/b };
    }

    const double discriminant = (b*b) - (4*a*c);

    if (isEqual(discriminant, 0)) {
        return std::vector<double>{ 0, -b/(2*a) };
    }
    if (discriminant < 0) {
        return std::vector<double>(0);
    }

    return std::vector<double>{
        (-b + sqrt(discriminant)) / (2 * a),
        (-b - sqrt(discriminant)) / (2 * a)
    };
}

std::vector<double> solve_no_exception(double a, double b, double c, bool& ok) noexcept {
    ok = true;

    if (isEqual(a, 0) && isEqual(b, 0) && isEqual(c, 0)) {
        ok = false;
        return std::vector<double>(0);
    }

    return solve_correct_equation(a, b, c);
}

std::vector<double> solve(double a, double b, double c) {
    if (isEqual(a, 0) && isEqual(b, 0) && isEqual(c, 0)) {
        throw std::logic_error("root is any value");
    }

    return solve_correct_equation(a, b, c);
}

void solve_full_exception(double a, double b, double c) {
    if (isEqual(a, 0) && isEqual(b, 0) && isEqual(c, 0)) {
        throw std::logic_error("root is any value");
    }

    throw solve_correct_equation(a, b, c); // lvalue?
}

// -----------------------------------------------------------------------

double roots_sum_no_exception(double a, double b, double c) noexcept {
    bool ok;
    auto roots = solve_no_exception(a, b, c, ok);
    if (true == ok) {
        return sum_vec(roots);
    }
    return 0;
}

double roots_sum(double a, double b, double c) noexcept {
    try {
        auto roots = solve(a, b, c);
        return sum_vec(roots);
    }
    catch(std::logic_error& err) {
        static_cast<void>(err);
        return 0;
    }
}

double roots_sum_full_exception(double a, double b, double c) {
    try {
        solve_full_exception(a, b, c);
    }
    catch(std::logic_error& err) {
        static_cast<void>(err);
        return 0;
    }
    catch(std::vector<double>& roots) {
        return sum_vec(roots);
    }
    throw std::runtime_error(
        "wrong exception type"
        ); // impossible.
}

// -----------------------------------------------------------------------

//! вызывает функцию в соответствии с типом
double call_solver(FunctionType type, double a, double b, double c) noexcept {
    static const std::map<
        FunctionType,
        std::function<
            double(double, double, double)>
        > type2function = {
            { FunctionType::NoException, roots_sum_no_exception },
            { FunctionType::Normal, roots_sum },
            { FunctionType::FullException, roots_sum_full_exception }
        };

    return type2function.at(type)(a, b, c);
}

long long run(long long n, FunctionType type) noexcept {
    using namespace std::chrono;
    const auto begin = steady_clock::now();

    double sum{ 0 };
#pragma omp parallel for reduction (+: sum)
    for (long long i = 0; i < n; i++) {
        const auto [a, b, c] = generate_coeffs(i);
        sum += call_solver(type, a, b, c);
    }

    const auto end = steady_clock::now();
    const auto elapsed_us = duration_cast<microseconds>(end - begin);

    static_cast<void>(sum);
    return
        elapsed_us.count();
}

int main() {
#ifdef _OPENMP
    std::cout << "OpenMP is supported!\n";

    //omp_set_num_threads(8);
    std::cout << "max_threads: " << omp_get_max_threads() << '\n';
    std::cout << "num_threads: " << omp_get_num_threads() << '\n';
#endif

    std::map<FunctionType, std::vector<long long>> results;
    results[FunctionType::Normal].reserve(Environment::run_count);
    results[FunctionType::NoException].reserve(Environment::run_count);
    results[FunctionType::FullException].reserve(Environment::run_count);

    const auto n = Environment::equation_count;
    for (int i = 0; i < Environment::run_count; ++i) {
        long long elapsed_us{ 0 };

        //std::cout << "\t\t\t--------No exception-----------" << std::endl;
        elapsed_us = run(n, FunctionType::NoException);
        results[FunctionType::NoException].push_back(elapsed_us);
        //std::cout << n << "\t" << elapsed_us << std::endl;

        //std::cout << "\t\t\t--------Normal-----------" << std::endl;
        elapsed_us = run(n, FunctionType::Normal);
        results[FunctionType::Normal].push_back(elapsed_us);
        //std::cout << n << "\t" << elapsed_us << std::endl;

        //std::cout << "\t\t\t--------Full exception-----------" << std::endl;
        elapsed_us = run(n, FunctionType::FullException);
        results[FunctionType::FullException].push_back(elapsed_us);
        //std::cout << n << "\t" << elapsed_us << std::endl;
    }

    // ***

    std::cout << "run_count: " << Environment::run_count << std::endl;
    std::cout << "equation_count: " << Environment::equation_count << std::endl;
    std::cout << "exception_count: " << Environment::exception_count << std::endl;

    std::cout << "n\ttime (us)" << std::endl;

    const auto& vec_no_exc = results[FunctionType::NoException];
    const auto avg_no_exc_us = std::accumulate(
                                   vec_no_exc.begin(),
                                   vec_no_exc.end(),
                                   0) / double(vec_no_exc.size());
    std::cout << "\t\t\t--------No exception-----------" << std::endl;
    std::cout << n << "\t" << avg_no_exc_us << std::endl;

    const auto& vec_normal = results[FunctionType::Normal];
    const auto avg_normal_us = std::accumulate(
                                   vec_normal.begin(),
                                   vec_normal.end(),
                                   0) / double(vec_normal.size());
    std::cout << "\t\t\t--------Normal-----------" << std::endl;
    std::cout << n << "\t" << avg_normal_us << std::endl;

    const auto& vec_full_exc = results[FunctionType::FullException];
    const auto avg_full_exc_us = std::accumulate(
                                     vec_full_exc.begin(),
                                     vec_full_exc.end(),
                                     0) / vec_full_exc.size();
    std::cout << "\t\t\t--------Full exception-----------" << std::endl;
    std::cout << n << "\t" << avg_full_exc_us << std::endl;
}
