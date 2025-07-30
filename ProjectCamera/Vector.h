#pragma once
#include <cmath>
#include <algorithm>
#include <iostream> //debug

/**
 * @class Vector
 * @brief Template class for mathematical vectors of arbitrary dimension.
 * @tparam C Numeric type (e.g., double, float, int)
 *
 * Provides vector operations such as addition, subtraction, dot product,
 * cross product, normalization, and more. Used for physics and geometry.
 */
template<typename C>
class Vector
{
public:
    C x; ///< X component
    C y; ///< Y component
    C z; ///< Z component

    /**
     * @brief Default constructor. Initializes all components to zero.
     */
    Vector();
    /**
     * @brief Construct a vector with a given dimension (size).
     * @param s Number of dimensions (2 or 3 typical)
     */
    Vector(int s);
    /**
     * @brief Destructor. No dynamic memory to free.
     */
    ~Vector();

    /**
     * @brief Assignment operator.
     * @param B Vector to assign from
     * @return Reference to this vector
     */
    Vector<C>& operator=(const Vector<C>& B);

    /**
     * @brief Update the length and squared length of the vector.
     */
    void updatelen();
    /**
     * @brief Set x and y components.
     * @param x1 X value
     * @param y1 Y value
     */
    void set(C x1, C y1);
    /**
     * @brief Set all components from an array.
     * @param _components Array of values
     */
    void set(C _components[]);
    /**
     * @brief Set x, y, and z components.
     * @param x1 X value
     * @param y1 Y value
     * @param z1 Z value
     */
    void set(C x1, C y1, C z1);

    /**
     * @brief Return a normalized copy of this vector.
     * @return Normalized vector
     */
    Vector<C> normalizedVec();
    /**
     * @brief Multiply all components by a scalar.
     * @param n Scalar value
     */
    void scalarMult(C n);
    /**
     * @brief Normalize this vector in place.
     */
    void normalize();
    /**
     * @brief Return a zero vector of the given dimension.
     * @param dimension Number of dimensions
     * @return Zero vector
     */
    static Vector<C> Zero(int dimension = 3);

    /**
     * @brief Compute the dot product with another vector.
     * @param B Other vector
     * @return Dot product
     */
    C dotProduct(Vector<C> B);
    /**
     * @brief Compute the cross product with another vector.
     * @param B Other vector
     * @return Cross product vector
     */
    Vector<C> crossProduct(Vector<C> B);
    /**
     * @brief Compute the cosine of the angle with another vector.
     * @param B Other vector
     * @return Cosine of angle
     */
    double cosine(Vector<C> B);
    /**
     * @brief Compute the sine of the angle with another vector.
     * @param B Other vector
     * @return Sine of angle
     */
    double sine(Vector<C> B);

    /**
     * @brief Vector addition.
     * @param B Other vector
     * @return Sum vector
     */
    Vector<C> operator+(const Vector<C>& B) const;
    /**
     * @brief Vector subtraction.
     * @param B Other vector
     * @return Difference vector
     */
    Vector<C> operator-(const Vector<C>& B) const;
    /**
     * @brief Scalar multiplication.
     * @param scalar Scalar value
     * @return Scaled vector
     */
    Vector<C> operator*(C scalar) const;
    /**
     * @brief Scalar division.
     * @param scalar Scalar value
     * @return Scaled vector
     */
    Vector<C> operator/(C scalar) const;

    int size; ///< Number of dimensions
    C components[12]; ///< Component array (up to 12D, but usually 2 or 3)
    double length; ///< Vector length
    double sqr_length; ///< Squared length

    /**
     * @brief Sync x, y, z with the components array and update length.
     */
    void syncComponents();
};

template<typename C>
Vector<C>::Vector()
    : size(0)
    , x(C{})
    , y(C{})
    , z(C{})
    , length(0.0)
    , sqr_length(0.0)
{
    for (int i = 0; i < 12; ++i)
        components[i] = C{};
}

template<typename C>
Vector<C>::Vector(int s)
    : size(s)
    , x(C{})
    , y(C{})
    , z(C{})
    , length(0.0)
    , sqr_length(0.0)
{
    for (int i = 0; i < 12; ++i)
        components[i] = C{};
    if (size > 0) x = components[0];
    if (size > 1) y = components[1];
    if (size > 2) z = components[2];
    syncComponents();
}

template<typename C>
Vector<C>::~Vector() {
    // No dynamic memory to free
}

template<typename C>
void Vector<C>::syncComponents() {
    if (size > 0) components[0] = x;
    if (size > 1) components[1] = y;
    if (size > 2) components[2] = z;
    updatelen();
}

template<typename C>
void Vector<C>::set(C x1, C y1) {
    if (size < 2) return;
    x = x1;
    y = y1;
    syncComponents();
}

template<typename C>
void Vector<C>::set(C x1, C y1, C z1) {
    if (size < 3) return;
    x = x1;
    y = y1;
    z = z1;
    syncComponents();
}

template<typename C>
void Vector<C>::set(C _components[]) {
    for (int i = 0; i < size; i++) {
        components[i] = _components[i];
    }
    if (size > 0) x = components[0];
    if (size > 1) y = components[1];
    if (size > 2) z = components[2];
    updatelen();
}

template<typename C>
void Vector<C>::updatelen()
{
    sqr_length = 0;
    for (int i = 0; i < size; i++)
        sqr_length += components[i] * components[i];
    length = sqrt(sqr_length);  // Update length as well
}

template<typename C>
Vector<C> Vector<C>::normalizedVec() {
    Vector<C> B(size);
    double len = sqrt(sqr_length);  // Use local variable instead of member
    for (int i = 0; i < size; i++)
        B.components[i] = components[i] / len;
    return B;
}

template<typename C>
void Vector<C>::normalize()
{
    double len = sqrt(sqr_length);
    scalarMult(1 / len);
}

template<typename C>
void Vector<C>::scalarMult(C n)
{
    for (int i = 0; i < size; i++)
        components[i] *= n;
    updatelen();  // Update length after scaling
}

template<typename C>
C Vector<C>::dotProduct(Vector<C> B)
{
    int minimum = std::min(B.size, size);
    C sum = 0;
    for (int i = 0; i < minimum; i++)
        sum += B.components[i] * components[i];
    return sum;
}

template<typename C>
Vector<C> Vector<C>::crossProduct(Vector<C> B) {
    if (size == 2 && B.size == 2) {
        Vector<C> result(1);
        result.components[0] = x * B.y - y * B.x;
        return result;
    }
    else if (size == 3 && B.size == 3) {
        Vector<C> result(3);
        result.components[0] = y * B.z - z * B.y;
        result.components[1] = z * B.x - x * B.z;
        result.components[2] = x * B.y - y * B.x;
        return result;
    }
    else {
        return Vector<C>(0);
    }
}

template<typename C>
double Vector<C>::cosine(Vector<C> B)
{
    return this->normalizedVec().dotProduct(B.normalizedVec());
}

template<typename C>
double Vector<C>::sine(Vector<C> B)
{
    double c = cosine(B);
    return sqrt(1 - c * c);
}

template<typename C>
Vector<C> Vector<C>::operator+(const Vector<C>& B) const {
    Vector<C> result(size);
    int minimum = std::min(size, B.size);
    for (int i = 0; i < minimum; ++i)
        result.components[i] = this->components[i] + B.components[i];
    result.updatelen();  // Update length after operation
    return result;
}

template<typename C>
Vector<C> Vector<C>::operator-(const Vector<C>& B) const {
    Vector<C> result(size);
    int minimum = std::min(size, B.size);
    for (int i = 0; i < minimum; ++i)
        result.components[i] = this->components[i] - B.components[i];
    result.updatelen();  // Update length after operation
    return result;
}

template<typename C>
Vector<C> Vector<C>::operator*(C scalar) const {
    Vector<C> result(size);
    for (int i = 0; i < size; ++i)
        result.components[i] = this->components[i] * scalar;
    result.updatelen();  // Update length after operation
    return result;
}

template<typename C>
Vector<C> Vector<C>::operator/(C scalar) const {
    Vector<C> result(size);
    for (int i = 0; i < size; ++i)
        result.components[i] = this->components[i] / scalar;
    result.updatelen();  // Update length after operation
    return result;
}

template<typename C>
Vector<C>& Vector<C>::operator=(const Vector<C>& B) {
    if (this == &B)
        return *this;

    size = B.size;
    for (int i = 0; i < size; i++)
        components[i] = B.components[i];

    // Update references after reallocation
    if (size > 0) x = components[0];
    if (size > 1) y = components[1];
    if (size > 2) z = components[2];

    sqr_length = B.sqr_length;
    length = B.length;

    return *this;
}

template<typename C>
Vector<C> Vector<C>::Zero(int dimension) {
    Vector<C> zeroVec(dimension);
    for (int i = 0; i < dimension; ++i)
        zeroVec.components[i] = C{};
    zeroVec.updatelen();
    return zeroVec;
}