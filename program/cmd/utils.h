#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

namespace Utils
{

std::string str_to_lower(std::string str);
bool string_equals(std::string lhs, std::string rhs);
bool double_equals(double a, double b, double eps = 0.00001);
double sum_vec(const std::vector<double> &vec);

};

#endif // UTILS_H
