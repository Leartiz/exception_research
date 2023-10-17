#include <functional>
#include <stdexcept>

// ---> https://godbolt.org/z/oKsabfY34

int foo() {
    if (std::rand() % 2 == 0) {
        throw std::runtime_error("some err");
    }
    return 0;
}

int main() {
    try {
        static_cast<void>(foo());
    }
    catch(const std::runtime_error& err) {
        static_cast<void>(err);
    }
}
