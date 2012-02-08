#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>

namespace dmcr {

template<typename T>
class Vector3
{
public:
    /*!
      \brief Construct a vector with default values
      */
    Vector3() : m_x(), m_y(), m_z() { }
    /*!
      \brief Construct a vector with values
      \param x x coordinate
      \param y y coordinate
      \param z z coordinate
      */
    Vector3(T x, T y, T z) : m_x(x), m_y(y), m_z(z) { }

    /*!
      \brief Get vector x coordinate
      */
    T x() const { return m_x; }
    /*!
      \brief Get vector y coordinate
      */
    T y() const { return m_y; }
    /*!
      \brief Get vector z coordinate
      */
    T z() const { return m_z; }

    /*!
      \brief Set vector x coordinate
      */
    void setX(T x) { m_x = x; }
    /*!
      \brief Set vector y coordinate
      */
    void setY(T y) { m_y = y; }
    /*!
      \brief Set vector z coordinate
      */
    void setZ(T z) { m_z = z; }

    /*!
      \brief Add two vectors
      \return Result of addition
      */
    Vector3<T> operator+(const Vector3<T>& v) const {
        return Vector3<T>(m_x + v.x(), m_y + v.y(), m_z + v.z());
    }
    /*!
      \brief Add a vector in place
      */
    void operator+=(const Vector3<T>& v) {
        m_x += v.x(); m_y += v.y(); m_z += v.z();
    }
    /*!
      \brief Subtract two vectors
      \return Result of subtraction
      */
    Vector3<T> operator-(const Vector3<T>& v) const {
        return Vector3<T>(m_x - v.x(), m_y - v.y(), m_z - v.z());
    }
    /*!
      \brief Subtract a vector in place
      */
    void operator-=(const Vector3<T>& v) {
        m_x -= v.x(); m_y -= v.y(); m_z -= v.z();
    }
    /*!
      \brief Multiply by a scalar
      \return Result of scalar multiplication
      */
    Vector3<T> operator*(T s) const {
        return Vector3<T>(s * m_x, s * m_y, s * m_z);
    }
    /*!
      \brief Multiply by a scalar in place
      */
    void operator*=(T s) {
        m_x *= s; m_y *= s; m_z *= s;
    }
    /*!
      \brief Divide by a scalar
      \return Result of scalar division
      */
    Vector3<T> operator/(T s) const {
        return Vector3<T>(s / m_x, s / m_y, s / m_z);
    }
    /*!
      \brief Divide by a scalar in place
      */
    void operator/=(T s) {
        m_x /= s; m_y /= s; m_z /= s;
    }

    /*!
      \brief Perform a dot product
      \return Result of dot product
      */
    T dot(const Vector3<T>& v) {
        return m_x * v.x() + m_y * v.y() + m_z * v.z();
    }
    /*!
      \brief Perform a cross product
      \return Result of cross product
      */
    Vector3<T> cross(const Vector3<T>& v) {
        return Vector3<T>(m_y * v.z() - m_z * v.y(),
                         m_z * v.x() - m_x * v.z(),
                         m_x * v.y() - m_y * v.x());
    }

    /*!
      \brief Calculate length of vector
      \return Length of vector
      */
    T length() const { return sqrt(m_x * m_x + m_y * m_y + m_z * m_z); }
    /*!
      \brief Normalize vector in place
      */
    void normalize() {
        T length = length();
        m_x /= length; m_y /= length; m_z /= length;
    }
    /*!
      \brief Normalize vector
      \return Normalized vector
      */
    Vector3<T> normalized() const {
        T length = length();
        return Vector3<T>(m_x / length, m_y / length, m_z / length);
    }

private:
    T m_x, m_y, m_z;
};

typedef Vector3<float> Vector3f;

}

#endif // VECTOR_H
