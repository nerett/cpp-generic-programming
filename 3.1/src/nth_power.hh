#pragma once

#include <concepts>
#include <type_traits>


template <typename T>
concept Integer = std::is_integral_v<T>;

template <typename T>
concept FloatingPoint = std::is_floating_point_v<T>;

template <typename T>
concept Matrix = requires(T m) {
    { m * m } -> std::same_as<T>;
    { T(1) } -> std::same_as<T>;
    m.rows;
    m.cols;
};

template <typename T>
    requires Integer<T> || FloatingPoint<T>
T nth_power(T base, unsigned int exp) {
    if (exp == 0)
        return static_cast<T>(1);
    T res = static_cast<T>(1);
    while (exp > 0) {
        if (exp % 2 == 1)
            res *= base;
        base *= base;
        exp /= 2;
    }
    return res;
}

template <Matrix T>
T nth_power(T base, unsigned int exp) {
    if (base.rows != base.cols)
        throw std::invalid_argument("Matrix must be square for exponentiation.");
    if (exp == 0)
        return T(base.rows);

    T res = T(base.rows);
    while (exp > 0) {
        if (exp % 2 == 1)
            res = res * base;
        base = base * base;
        exp /= 2;
    }
    return res;
}
