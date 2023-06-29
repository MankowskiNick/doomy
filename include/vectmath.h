#ifndef VECTMATH_H
#define VECTMATH_H

// Formatted like
// a b
// c d
template <typename T>
struct Matrix2x2 { T a, b, c, d; };

template <typename T>
struct Vect2 { T a, b; };

template <typename T>
T Det(const Matrix2x2<T>& mat) {
    return (mat.a * mat.d) - (mat.c * mat.b);
}

template <typename T>
Matrix2x2<T> Inverse(Matrix2x2<T> mat) {
    T inverse_det = 1 / Det(mat);

    return Matrix2x2<T> {
        .a = inverse_det * mat.d,       .b = -1 * inverse_det * mat.b,
        .c = -1 * inverse_det * mat.c,  .d = inverse_det * mat.a
    };
}

template <typename T>
Vect2<T> Scale(const Vect2<T>& vect, T scalar) {
    return {
        .a = scalar * vect.a,
        .b = scalar * vect.b
    };
}

template <typename T>
Vect2<T> Mult(const Matrix2x2<T>& mat, const Vect2<T>& vect) {
    return {
        .a = (mat.a * vect.a) + (mat.b * vect.b),
        .b = (mat.c * vect.a) + (mat.d * vect.b)
    };
}

template <typename T>
Vect2<T> Mult(const Matrix2x2<T>& m1, const Matrix2x2<T>& m2) {
    return {
        .a = (m1.a * m2.a) + (m1.b * m2.c), .b = (m1.a * m2.b) + (m1.b * m2.d),
        .c = (m1.c * m2.a) + (m1.d * m2.c), .d = (m1.c * m2.b) + (m1.d * m2.d)
    };
}

template <typename T>
Vect2<T> Add(const Vect2<T>& v1, const Vect2<T>& v2) {
    return {
        .a = v1.a + v2.a,
        .b = v1.b + v2.b
    };
}

#endif