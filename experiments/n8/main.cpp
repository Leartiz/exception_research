#include <vector>
#include <iostream>

void print(const std::vector<int>& vec)
{
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";
    } std::cout << std::endl;
}

int main()
{
    std::vector<int> vec{ 0 };
    print(vec);
    return 0;
}
