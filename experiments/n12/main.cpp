#include <string>
#include <vector>

#include <thread>
#include <cstdlib>
#include <iostream>

using namespace std;

struct Error
{
    int code{ 0 };
    string text;
};

struct Payload
{
    string str;
    vector<int> vec;
};

// -----------------------------------------------------------------------

pair<Error, Payload> doo()
{
    // actions...

    return {
        Error{
            .code = 5,
            .text = "...",
        },
        Payload{
            .str = {},
            .vec = {}
        }
    };
}

Payload doo(Error& err)
{
    // actions...

    bool condition{
        rand() % 2 == 0
    };

    if (condition) {
        err = Error{
            .code = 5,
            .text = "...",
        };
    }

    return Payload{
        .str = {},
        .vec = {}
    };
}

Payload doo_throwing_err()
{
    // actions...

    bool condition{
        rand() % 2 == 0
    };

    if (condition) {
        throw Error{
            .code = 5,
            .text = "",
        };
    }

    return Payload{
        .str = {},
        .vec = {}
    };
}

void doo_throwing_all()
{
    // actions...

    bool condition{
        rand() % 2 == 0
    };

    if (condition) {
        throw Error{
            .code = 5,
            .text = "...",
        };
    }

    throw Payload{
        .str = {},
        .vec = {}
    };
}

// -----------------------------------------------------------------------

int main()
{
    return 0;
}
