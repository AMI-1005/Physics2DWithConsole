#include "Matrix.h"

/**
 * @brief Construct a new Matrix object representing a 2D rotation matrix.
 * @param rad Angle in radians for the rotation.
 */
Matrix::Matrix(double rad)
{
    set(rad); // Initialize matrix with rotation angle
}

/**
 * @brief Destroy the Matrix object.
 */
Matrix::~Matrix()
{
    // No dynamic resources to free
}

/**
 * @brief Set the matrix to represent a rotation by the given angle.
 * @param rad Angle in radians.
 */
void Matrix::set(double rad) {
    double c = cos(rad); // Cosine of angle
    double s = sin(rad); // Sine of angle
    components[0] = c;   // Row 1, Col 1
    components[1] = -s;  // Row 1, Col 2
    components[2] = s;   // Row 2, Col 1
    components[3] = c;   // Row 2, Col 2
    angle = rad;         // Store angle
}

/**
 * @brief Return the transpose of this matrix (swap off-diagonal elements).
 * @return Matrix The transposed matrix.
 */
Matrix Matrix::transpose()
{
    Matrix A(angle); // Copy with same angle
    std::swap(A.components[1], A.components[2]); // Swap off-diagonal
    return A;
}

/**
 * @brief Transpose this matrix in place (swap off-diagonal elements).
 */
void Matrix::transposeThis()
{
    std::swap(components[1], components[2]);
}

/**
 * @brief Multiply this matrix by another matrix (matrix multiplication).
 * @param rhs The right-hand side matrix.
 * @return Matrix The result of the multiplication.
 */
Matrix Matrix::operator*(Matrix rhs) {
    Matrix M(0.0); // Result matrix
    // Standard 2x2 matrix multiplication
    M.components[0] = components[0] * rhs.components[0] + components[1] * rhs.components[2];
    M.components[1] = components[0] * rhs.components[1] + components[1] * rhs.components[3];
    M.components[2] = components[2] * rhs.components[0] + components[3] * rhs.components[2];
    M.components[3] = components[2] * rhs.components[1] + components[3] * rhs.components[3];
    return M;
}