#include <functional>
#include <stdexcept>

// ---> https://godbolt.org/z/qcKTfKGdP

int foo() {
    throw std::runtime_error("");
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
