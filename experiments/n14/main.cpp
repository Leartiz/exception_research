#include <iostream>

using namespace std;

class Error {
public:
    Error(int data) : m_data{ data } {};
    int get() const { return m_data; }
    void set(int data) { m_data = data; }

private:
    int m_data;
};

int foo(Error& err) {
    if (err.get()) return 0;
    if (rand() % 2) {
        err.set(1);
        return 0;
    }
    return 99; // <--- useful
}

int bar(Error& err) {
    if (err.get()) return 0;
    return foo(err); // <--- useful
}

int main()
{
    Error err{ 0 };
    bar(err);

    if (!err.get()) { /* useful */ }
    else if (err.get() == 1) { }
}
