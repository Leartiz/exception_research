#include <iostream>
#include <stdexcept>
#include <string>

#include "argh.h"

enum Type { sequential, parallel };
std::string type_to_str(const Type tp) {
    switch (tp) {
    case parallel: return "parallel";
    default: return "sequential";
    }
}

double str_to_time_ratio(const std::string& str) {
    if (str == "s") return 1'000'000;
    if (str == "ms") return 1'000;
    return 1;
}

// -----------------------------------------------------------------------

int main(int argc, char* argv[])
{
    argh::parser cmdl(argc, argv);

    std::cout << "\nParameters:\n";
    for (const auto& param : cmdl.params())
        std::cout << '\t'
                  << param.first << " : "
                  << param.second
                  << '\n';
    std::cout << std::endl;

    // ***

    size_t run_count = 0;
    std::string run_type_as_str;
    Type run_type = Type::sequential;

    size_t equation_count = 0;
    double error_percentage = 0.0;
    std::string time_unit_as_str;
    double time_ratio = 1;

    // ***

    cmdl({ "rc", "run_count" }, 1) >> run_count;
    cmdl({ "rt", "run_type" }, "sequential") >> run_type_as_str;
    std::transform(run_type_as_str.begin(),run_type_as_str.end(),
                   run_type_as_str.begin(), std::tolower);
    if (run_type_as_str == "parallel")
        run_type = Type::parallel;

    cmdl({ "ec", "equation_count" }, 1'000) >> equation_count;
    cmdl({ "ep", "error_percentage" }, 5.0) >> error_percentage;

    cmdl({ "tu", "time_unit" }, "us") >> time_unit_as_str;
    time_ratio = str_to_time_ratio(time_unit_as_str);

    const size_t exception_count =
        static_cast<size_t>(
            std::round(error_percentage / 100 * equation_count));

    // ***

    std::cout << "run_count: " << run_count << "\n";
    std::cout << "run_type: " << type_to_str(run_type) << "\n";

    std::cout << "equation_count: " << equation_count << "\n";
    std::cout << "error_percentage: " << error_percentage << "\n";
    std::cout << "exception_count: " << exception_count << "\n";

    std::cout << "time_ratio: " << time_ratio << "\n";
    std::cout << std::endl;

    // ***

    std::cout << "[OK]\n";
}
