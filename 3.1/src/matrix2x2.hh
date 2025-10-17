#pragma once

#include <cstddef>
#include <stdexcept>


struct Matrix2x2 {
    int n[2][2];

    bool operator==(const Matrix2x2& m) const {
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                if (n[i][j] != m.n[i][j])
                    return false;
        return true;
    }

    Matrix2x2& operator*=(const Matrix2x2& m) {
        Matrix2x2 r;
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                int acc = 0;
                for (int k = 0; k < 2; k++)
                    acc += n[i][k] * m.n[k][j];
                r.n[i][j] = acc;
            }
        }
        *this = r;
        return *this;
    }

    Matrix2x2(int n00, int n01, int n10, int n11) {
        n[0][0] = n00;
        n[0][1] = n01;
        n[1][0] = n10;
        n[1][1] = n11;
    }

    explicit Matrix2x2(size_t n_size) {
        if (n_size != 2)
            throw std::invalid_argument("Matrix2x2 can only be initialized with size 2.");
        n[0][0] = 1; n[0][1] = 0;
        n[1][0] = 0; n[1][1] = 1;
    }

    Matrix2x2() = default;

    static constexpr size_t rows = 2;
    static constexpr size_t cols = 2;
};

inline Matrix2x2 operator*(Matrix2x2 lhs, const Matrix2x2& rhs) {
    lhs *= rhs;
    return lhs;
}
