#include <iostream>
#include <iomanip>

#include <cassert>
#include <exception>

#include <algorithm>
#include <functional>
#include <numeric>
#include <cctype>
#include <cmath>

#include <chrono>
#include <vector>
#include <tuple>
#include <map>

#include <omp.h>

#include "argh.h"
#include "utils.h"
#include "params.h"

using namespace Utils;

enum class FunctionType {
    NoException, Normal, FullException
};

// -----------------------------------------------------------------------

//! решение уравнения при a, b, c одновременно не равных нулю
std::vector<double> solve_correct_equation(double a, double b, double c) noexcept {
    assert(
        !(
            double_equals(a, 0) &&
            double_equals(b, 0) &&
            double_equals(c, 0)
            )
        );

    if (double_equals(a, 0) && double_equals(b, 0)) {
        return std::vector<double>();
    }

    if (double_equals(a,0)) {
        return std::vector<double>{ 0, -c / b };
    }

    const double discriminant = (b * b) - (4 * a * c);

    if (double_equals(discriminant, 0)) {
        return std::vector<double>{ 0, -b / (2 * a) };
    }
    if (discriminant < 0) {
        return std::vector<double>(); // no roots!
    }

    return std::vector<double>{
        (-b + sqrt(discriminant)) / (2 * a),
        (-b - sqrt(discriminant)) / (2 * a)
    };
}

// -----------------------------------------------------------------------

std::vector<double> solve_no_exception(double a, double b, double c, bool& ok) noexcept {
    ok = true;

    if (double_equals(a, 0) && double_equals(b, 0) && double_equals(c, 0)) {
        ok = false;
        return {};
    }

    return solve_correct_equation(a, b, c);
}

std::vector<double> solve_normal(double a, double b, double c) {
    if (double_equals(a, 0) && double_equals(b, 0) && double_equals(c, 0)) {
        throw std::logic_error("root is any value");
    }

    return solve_correct_equation(a, b, c);
}

void solve_full_exception(double a, double b, double c) {
    if (double_equals(a, 0) && double_equals(b, 0) && double_equals(c, 0)) {
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

double roots_sum_normal(double a, double b, double c) noexcept {
    try {
        auto roots = solve_normal(a, b, c);
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
            { FunctionType::Normal, roots_sum_normal },
            { FunctionType::FullException, roots_sum_full_exception }
        };

    return type2function.at(type)(a, b, c);
}

// starts
// -----------------------------------------------------------------------

std::map<FunctionType, std::vector<long long>> build_zero_results(const size_t n)
{
    return {
        { FunctionType::Normal, std::vector<long long>(n, 0) },
        { FunctionType::NoException, std::vector<long long>(n, 0) },
        { FunctionType::FullException, std::vector<long long>(n, 0) }
    };
}

void println_one_result(const std::vector<long long>& vec, const std::string& title)
{
    const double avg = std::accumulate(vec.begin(), vec.end(),
                                     0) / double(vec.size());
    std::cout << title << " ";
    std::cout << std::setprecision(25)
              << avg / Params::time_ratio
              << std::endl;
}

void println_results(std::map<FunctionType, std::vector<long long>>& results)
{
    println_one_result(results[FunctionType::NoException], "no_exception:");
    println_one_result(results[FunctionType::Normal], "normal:");
    println_one_result(results[FunctionType::FullException], "full_exception:");
}

long long run_parallel(size_t n, FunctionType type) noexcept;
long long run_sequential(size_t n, FunctionType type) noexcept;
void some_runs(std::map<FunctionType, std::vector<long long>>& results) noexcept
{
    const auto run = Params::run_type == Params::sequential ?
                         run_sequential : run_parallel;

    // ***

    const auto n = Params::equation_count;
    for (size_t i = 0; i < Params::run_count; ++i) {
        results[FunctionType::NoException][i] = run(n, FunctionType::NoException);
        results[FunctionType::Normal][i] = run(n, FunctionType::Normal);
        results[FunctionType::FullException][i] = run(n, FunctionType::FullException);
    }
}

// -----------------------------------------------------------------------

long long run_parallel(size_t n, FunctionType type) noexcept {
    omp_set_num_threads(Params::max_thread_count);

    using namespace std::chrono;
    const auto begin = steady_clock::now();

    double sum{ 0 };
    const int signed_n = static_cast<int>(n);

#pragma omp parallel for reduction (+: sum)
    for (int i = 0; i < signed_n; i++) {
        const auto [a, b, c] = Params::generate_coeffs(i);
        sum += call_solver(type, a, b, c);
    }

    const auto end = steady_clock::now();
    const auto elapsed_us = duration_cast<microseconds>(end - begin).count();

    static_cast<void>(sum);
    return elapsed_us;
}

long long run_sequential(size_t n, FunctionType type) noexcept {
    using namespace std::chrono;
    const auto begin = steady_clock::now();

    double sum{ 0 };
    const int signed_n = static_cast<int>(n);

    for (int i = 0; i < signed_n; i++) {
        const auto [a, b, c] = Params::generate_coeffs(i);
        sum += call_solver(type, a, b, c);
    }

    const auto end = steady_clock::now();
    const auto elapsed_us = duration_cast<microseconds>(end - begin).count();

    static_cast<void>(sum);
    return elapsed_us;
}


// -----------------------------------------------------------------------

void println_launch_params();
void println_about_openmp();

int main(int argc, char* argv[]) {
    Params::initialize(argc, argv);
    println_about_openmp();

    // ***

    std::cout << "\tinput:\n";
    println_launch_params();

    // ***

    auto results = build_zero_results(
        Params::run_count);
    some_runs(results);

    // ***

    std::cout << "\toutput:\n";
    println_results(results);
}

void println_about_openmp()
{
#ifdef _OPENMP
    std::cout << "OpenMP is supported!\n";
    std::cout << "omp default max threads: " << omp_get_max_threads() << "\n";
#else
    std::cout << "OpenMP is not supported!\n";
#endif
    std::cout << std::endl;
}

void println_launch_params()
{
    std::cout << "run_count: " << Params::run_count << std::endl;
    std::cout << "run_type: " << Params::type_to_str(Params::run_type) << std::endl;
    std::cout << "max_thread_count: " << Params::max_thread_count << std::endl;
    std::cout << "equation_count: " << Params::equation_count << std::endl;
    std::cout << "error_percentage: " << Params::error_percentage << std::endl;
    std::cout << "exception_count: " << Params::exception_count << std::endl;
    std::cout << "time_ratio: " << Params::time_ratio << std::endl;
    std::cout << "time_unit: " << Params::time_unit << std::endl;
    std::cout << std::endl;
}
