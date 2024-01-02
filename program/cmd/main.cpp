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

bool string_equals(std::string lhs, std::string rhs);

struct LaunchParams final
{
    enum Type { sequential, parallel };
    static std::string type_to_str(const Type tp) {
        switch (tp) {
        case parallel: return "parallel";
        default: return "sequential";
        }
    }

    static double str_to_time_ratio(const std::string& str) {
        if (string_equals(str, "s")) return 1'000'000;
        if (string_equals(str, "ms")) return 1'000;
        return 1;
    }

public:
    static size_t run_count;
    static Type run_type;
    static int max_thread_count;

    static size_t equation_count;
    static double error_percentage;
    static size_t exception_count; // <--- calculated!

    using time_unit_t = std::chrono::microseconds;
    static std::string time_unit;
    static double time_ratio; // maybe uint.

public:
    static void initialize(int argc, char* argv[])
    {
        argh::parser cmdl(argc, argv);

        // ***

        cmdl({"rc", "run_count"}, 1) >> run_count;

        run_type = Type::sequential;
        std::string run_type_as_str;
        cmdl({"rt", "run_type" }, "sequential") >> run_type_as_str;
        if (string_equals(run_type_as_str, "parallel"))
            run_type = Type::parallel;

        cmdl({ "mtc", "max_thread_count" }, omp_get_max_threads()) >> max_thread_count;

        cmdl({"ec", "equation_count" }, 1'000) >> equation_count;
        cmdl({"ep", "error_percentage" }, 5.0) >> error_percentage;

        cmdl({ "tu", "time_unit" }, "us") >> time_unit;
        time_ratio = str_to_time_ratio(time_unit);

        exception_count =
            static_cast<size_t>(
                std::round(error_percentage / 100 * equation_count));

        // ***

        exception_case_indexs =
            build_exception_case_indexs();
    }

    static std::vector<int> exception_case_indexs;
    static const std::vector<int> build_exception_case_indexs() {
        if (exception_count == 0) {
            return {};
        }

        std::vector<int> result;
        result.reserve(exception_count);
        for (size_t i = 1; i <= exception_count; ++i) {
            result.push_back(
                int(equation_count / i));
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

size_t LaunchParams::run_count;
LaunchParams::Type LaunchParams::run_type;
int LaunchParams::max_thread_count;

size_t LaunchParams::equation_count;
double LaunchParams::error_percentage;
size_t LaunchParams::exception_count; // <--- calculated!

std::string LaunchParams::time_unit;
double LaunchParams::time_ratio;

std::vector<int> LaunchParams::exception_case_indexs;


// utils
// -----------------------------------------------------------------------

std::string str_to_lower(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return str;
}

bool string_equals(std::string lhs, std::string rhs) {
    lhs = str_to_lower(lhs);
    rhs = str_to_lower(rhs);
    return lhs == rhs;
}

bool double_equals(double a, double b, double eps = 0.00001) {
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

    if (double_equals(a, 0) && double_equals(b, 0) && double_equals(c, 0)) {
        ok = false;
        return std::vector<double>(0);
    }

    return solve_correct_equation(a, b, c);
}

std::vector<double> solve(double a, double b, double c) {
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
    const double avg = std::accumulate(vec.begin(), vec.end(),
                                     0) / double(vec.size());
    std::cout << title << std::endl;
    std::cout << "\t" << std::setprecision(25)
              << avg / LaunchParams::time_ratio
              << std::endl;
}

void println_results(std::map<FunctionType, std::vector<long long>>& results)
{
    println_one_result(results[FunctionType::NoException], "No exception");
    println_one_result(results[FunctionType::Normal], "Normal");
    println_one_result(results[FunctionType::FullException], "Full exception");
}

long long run_parallel(size_t n, FunctionType type) noexcept;
long long run_sequential(size_t n, FunctionType type) noexcept;
void some_runs(std::map<FunctionType, std::vector<long long>>& results) noexcept
{
    const auto run = LaunchParams::run_type == LaunchParams::sequential ?
                         run_sequential : run_parallel;

    // ***

    const auto n = LaunchParams::equation_count;
    for (size_t i = 0; i < LaunchParams::run_count; ++i) {
        results[FunctionType::NoException][i] = run(n, FunctionType::NoException);
        results[FunctionType::Normal][i] = run(n, FunctionType::Normal);
        results[FunctionType::FullException][i] = run(n, FunctionType::FullException);
    }
}

// -----------------------------------------------------------------------

long long run_parallel(size_t n, FunctionType type) noexcept {
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

long long run_sequential(size_t n, FunctionType type) noexcept {
    using namespace std::chrono;
    const auto begin = steady_clock::now();

    double sum{ 0 };
    const int signed_n = static_cast<int>(n);

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

void println_launch_params();
void println_about_openmp();

int main(int argc, char* argv[]) {
    LaunchParams::initialize(argc, argv);
    omp_set_num_threads(LaunchParams::max_thread_count);

    // ***

    println_about_openmp();
    println_launch_params();

    // ***

    auto results = build_zero_results(
        LaunchParams::run_count);
    some_runs(results);

    // ***

    println_results(results);
}

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
    std::cout << "run_count: " << LaunchParams::run_count << std::endl;
    std::cout << "run_type: " << LaunchParams::type_to_str(LaunchParams::run_type) << std::endl;
    std::cout << "max_thread_count: " << LaunchParams::max_thread_count << std::endl;
    std::cout << "equation_count: " << LaunchParams::equation_count << std::endl;
    std::cout << "error_percentage: " << LaunchParams::error_percentage << std::endl;
    std::cout << "exception_count: " << LaunchParams::exception_count << std::endl;
    std::cout << "time_ratio: " << LaunchParams::time_ratio << std::endl;
    std::cout << "time_unit: " << LaunchParams::time_unit << std::endl;
    std::cout << std::endl;
}
