#include <cmath>
#include <omp.h>

#include "argh.h"

#include "params.h"

#include "utils.h"
using namespace Utils;

size_t Params::run_count;
Params::Type Params::run_type;
int Params::max_thread_count;

size_t Params::equation_count;
double Params::error_percentage;
size_t Params::exception_count; // <--- calculated!

std::string Params::time_unit;
double Params::time_ratio;

// -----------------------------------------------------------------------

std::string Params::type_to_str(const Type tp) {
    switch (tp) {
    case parallel: return "parallel";
    default: return "sequential";
    }
}

std::string Params::normalize_read_time_unit(const std::string& str) {
    if (string_equals(str, "s")) return "s";
    if (string_equals(str, "ms")) return "ms";
    return "us";
}

double Params::normalized_time_unit_to_ratio(const std::string& str) {
    if (string_equals(str, "s")) return 1'000'000;
    if (string_equals(str, "ms")) return 1'000;
    return 1;
}

double Params::normalize_error_percentage(const double val) {
    if (val < 0) return 0;
    if (val > 100) return 100;
    return val;
}

int Params::normalize_max_thread_count(const int val) {
    if (val < 0) return omp_get_max_threads();
    return val;
}

// -----------------------------------------------------------------------

void Params::initialize(int argc, char* argv[])
{
    argh::parser cmdl(argc, argv);

    // ***

    cmdl({"rc", "run_count"}, 1) >> run_count;

    run_type = Type::sequential;
    std::string run_type_as_str; // <--- temp.
    cmdl({"rt", "run_type" }, "sequential") >> run_type_as_str;
    if (string_equals(run_type_as_str, "parallel"))
        run_type = Type::parallel;

    cmdl({ "mtc", "max_thread_count" }, omp_get_max_threads()) >> max_thread_count;
    max_thread_count = normalize_max_thread_count(max_thread_count);

    cmdl({"ec", "equation_count" }, 1'000) >> equation_count;
    cmdl({"ep", "error_percentage" }, 5.0) >> error_percentage;
    error_percentage = normalize_error_percentage(error_percentage);

    cmdl({ "tu", "time_unit" }, "us") >> time_unit;
    time_unit = normalize_read_time_unit(time_unit);
    time_ratio = normalized_time_unit_to_ratio(time_unit);

    exception_count =
        static_cast<size_t>(
            std::round(error_percentage / 100 * equation_count));

    // ***

    exception_case_indexs =
        build_exception_case_indexs();
}

// -----------------------------------------------------------------------

std::vector<int> Params::exception_case_indexs;
const std::vector<int> Params::build_exception_case_indexs() {
    if (exception_count == 0) {
        return {};
    }

    std::vector<int> result;
    result.reserve(exception_count);

    const auto step = equation_count / exception_count;
    for (size_t i = 0; i < exception_count; ++i) {
        result.push_back(int(step * i) + int(step / 2));
    }

    std::sort(result.begin(), result.end());
    return result;
}

bool Params::is_exception_case_index(const int index) {
    return std::binary_search(
        exception_case_indexs.begin(),
        exception_case_indexs.end(),
        index
    );
}

std::tuple<double, double, double>
Params::generate_coeffs(const int index) {
    if (!Params::is_exception_case_index(index)) {
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
