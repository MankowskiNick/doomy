package com.models;

public class Mat2 {
    private float m00, m01;
    private float m10, m11;

    public Mat2(float m00, float m01, float m10, float m11) {
        this.m00 = m00;
        this.m01 = m01;
        this.m10 = m10;
        this.m11 = m11;
    }

    // Method to calculate the determinant of the matrix
    public float Det() {
        return (m00 * m11) - (m01 * m10);
    }

    // Method to calculate the inverse of the matrix
    public Mat2 Inv() {
        float det = this.Det();
        if (det == 0) {
            throw new ArithmeticException("Matrix is not invertible");
        }
        return new Mat2(
            m11 / det, -m01 / det,
            -m10 / det, m00 / det
        );
    }

    // Method to multiply the matrix by a vector
    public static Vec2 Mult(Mat2 mat, Vec2 vec) {
        float x = (mat.m00 * vec.x0) + (mat.m01 * vec.x1);
        float y = (mat.m10 * vec.x0) + (mat.m11 * vec.x1);
        return new Vec2(x, y);
    }
}
