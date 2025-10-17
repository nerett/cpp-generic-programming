#pragma once

#include <cstddef>
#include <stdexcept>


struct Matrix2x2 {
    static constexpr size_t rows = 2;
    static constexpr size_t cols = 2;

    int n[rows][cols];

    static Matrix2x2 I() {
        return {1, 0, 0, 1};
    }

    bool operator==(const Matrix2x2& m) const {
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
                if (n[i][j] != m.n[i][j])
                    return false;
        return true;
    }

    Matrix2x2& operator*=(const Matrix2x2& m) {
        Matrix2x2 r;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < m.cols; j++) {
                int acc = 0;
                for (int k = 0; k < cols; k++)
                    acc += n[i][k] * m.n[k][j];
                r.n[i][j] = acc;
            }
        }
        *this = r;
        return *this;
    }
};

inline Matrix2x2 operator*(Matrix2x2 lhs, const Matrix2x2& rhs) {
    lhs *= rhs;
    return lhs;
}
