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
#include <iomanip>

#include "omp.h"

struct LaunchParams final
{
    static const size_t run_count = 5;
    static const size_t equation_count = 3'000'000;
    static const size_t exception_count = 750'000;

    // ***

    static const std::vector<int> exception_case_indexs;
    static const std::vector<int> build_exception_case_indexs() {
        const auto cap = exception_count;
        std::vector<int> result; result.reserve(cap);
        for (size_t i = 1; i <= cap; ++i) {
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

    // ***

    using time_unit = std::chrono::microseconds;
    static constexpr char time_unit_name[] = "us";
};

const std::vector<int> LaunchParams::exception_case_indexs =
    build_exception_case_indexs();

// utils
// -----------------------------------------------------------------------

bool isEqual(double a, double b, double eps = 0.00001) {
    return abs(a - b) < eps;
}

double sum_vec(const std::vector<double>& vec) {
    return std::accumulate(vec.begin(), vec.end(), 0);
}

std::tuple<double, double, double>
generate_coeffs(const int index) {
    if (!LaunchParams::is_exception_case_index(index)) {
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
        return std::vector<double>();
    }

    if (isEqual(a,0)) {
        return std::vector<double>{ 0, -c / b };
    }

    const double discriminant = (b * b) - (4 * a * c);

    if (isEqual(discriminant, 0)) {
        return std::vector<double>{ 0, -b / (2 * a) };
    }
    if (discriminant < 0) {
        return std::vector<double>();
    }

    return std::vector<double>{
        (-b + sqrt(discriminant)) / (2 * a),
        (-b - sqrt(discriminant)) / (2 * a)
    };
}

// -----------------------------------------------------------------------

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
    const auto avg = std::accumulate(vec.begin(), vec.end(),
                                     0) / double(vec.size());
    std::cout << title << std::endl;
    std::cout << "\t" << std::setprecision(25) << avg << std::endl;
}

void println_results(std::map<FunctionType, std::vector<long long>>& results)
{
    println_one_result(results[FunctionType::NoException], "No exception");
    println_one_result(results[FunctionType::Normal], "Normal");
    println_one_result(results[FunctionType::FullException], "Full exception");
}

long long run(size_t n, FunctionType type) noexcept;
void some_runs(std::map<FunctionType, std::vector<long long>>& results) noexcept
{
    const auto n = LaunchParams::equation_count;
    for (size_t i = 0; i < LaunchParams::run_count; ++i) {
        results[FunctionType::NoException][i] = run(n, FunctionType::NoException);
        results[FunctionType::Normal][i] = run(n, FunctionType::Normal);
        results[FunctionType::FullException][i] = run(n, FunctionType::FullException);
    }
}

long long run(size_t n, FunctionType type) noexcept {
    using namespace std::chrono;
    const auto begin = steady_clock::now();

    double sum{ 0 };
    const int signed_n = static_cast<int>(n);
#pragma omp parallel for reduction (+: sum)
    for (int i = 0; i < signed_n; i++) {
        const auto [a, b, c] = generate_coeffs(i);
        sum += call_solver(type, a, b, c);
    }

    const auto end = steady_clock::now();
    const auto elapsed_us = duration_cast<microseconds>(end - begin).count();

    static_cast<void>(sum);
    return elapsed_us;
}

// -----------------------------------------------------------------------

void println_about_openmp()
{
#ifdef _OPENMP
    std::cout << "OpenMP is supported!\n";

    std::cout << "max_threads: " << omp_get_max_threads() << '\n';
    std::cout << "num_threads: " << omp_get_num_threads() << '\n';
#else
    std::cout << "OpenMP is not supported!\n";
#endif
    std::cout << std::endl;
}

void println_launch_params()
{
    std::cout << "run count: " << LaunchParams::run_count << std::endl;
    std::cout << "equation count: " << LaunchParams::equation_count << std::endl;
    std::cout << "exception count: " << LaunchParams::exception_count << std::endl;

    std::cout << "exceptions percentage: "
              << double(LaunchParams::exception_count) /
                     (LaunchParams::equation_count) * 100
              << std::endl;
    std::cout << std::endl;
}

int main() {
    println_about_openmp();
    println_launch_params();

    // ***

    auto results = build_zero_results(LaunchParams::run_count);
    some_runs(results);

    // ***

    std::cout << "time (" << LaunchParams::time_unit_name << ")"
              << std::endl;
    println_results(results);
}
