#include <numeric>
#include <algorithm>

#include "utils.h"

namespace Utils
{

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

bool double_equals(double a, double b, double eps) {
    return std::abs(a - b) < eps;
}

double sum_vec(const std::vector<double>& vec) {
    return std::accumulate(vec.begin(), vec.end(), 0);
}

};
