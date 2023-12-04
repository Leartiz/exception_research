#include <iostream>
#include <stdexcept>
#include <cstdlib>

using namespace std;

class Exception
{
public:
    Exception(const int data) noexcept
        : m_data{ data } {}
    ~Exception() {}
    int get() const noexcept { return m_data; }

private:
    int m_data;
};

void thrower() {
    if (rand() % 2)
        throw Exception(99);
}

int main()
{
    try {
        thrower();
    }
    catch(Exception&) {
    }
    catch(exception&) {

    }
}
