#include <limits>

int foo(int *a, int base, int off) {
    if (off > 0) {
        if (base > std::numeric_limits<int>::max() - off)
            return 42;
    }
    else if (off < 0) {
        if (base < std::numeric_limits<int>::min() - off)
            return 42;
    }

    return a[base + off];
}
