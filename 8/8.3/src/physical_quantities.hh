#pragma once


template<int M, int K, int S>
struct Unit {
    static constexpr int m = M;
    static constexpr int kg = K;
    static constexpr int s = S;
};


template<typename UnitType>
struct Value {
    double val;

    constexpr explicit Value(double d) : val(d) {}

    constexpr explicit operator double() const { return val; }
};


template <typename D1, typename D2>
constexpr auto operator*(Value<D1> a, Value<D2> b) {

    using ResultUnit = Unit<
        D1::m + D2::m,
        D1::kg + D2::kg,
        D1::s + D2::s
    >;

    return Value<ResultUnit>{ static_cast<double>(a) * static_cast<double>(b) };
}
