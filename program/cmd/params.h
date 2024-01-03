#ifndef PARAMS_H
#define PARAMS_H

#include <string>
#include <chrono>
#include <vector>

// TODO: can edit types...
struct Params final
{
public:
    enum Type { sequential, parallel };
    static std::string type_to_str(const Type tp);

public:
    static std::string normalize_read_time_unit(const std::string& str);
    static double normalized_time_unit_to_ratio(const std::string& str);
    static double normalize_error_percentage(const double val);
    static int normalize_max_thread_count(const int val);

public:
    static size_t run_count;
    static Type run_type;
    static int max_thread_count; // only for parallel

    static size_t equation_count;
    static double error_percentage;
    static size_t exception_count; // <--- will be calculated!

    // TODO: in a separate class?
    using time_unit_t = std::chrono::microseconds;
    static std::string time_unit;
    static double time_ratio;

public:
    static std::vector<int> exception_case_indexs;
    static const std::vector<int> build_exception_case_indexs();

    static bool is_exception_case_index(const int index);
    static std::tuple<double, double, double> generate_coeffs(const int index);

public:
    static void initialize(int argc, char* argv[]);
};


#endif // PARAMS_H
