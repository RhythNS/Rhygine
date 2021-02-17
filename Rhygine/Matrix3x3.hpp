/**
 *  ============================================================================
 *  MIT License
 *
 *  Copyright (c) 2016 Eric Phillips
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a
 *  copy of this software and associated documentation files (the "Software"),
 *  to deal in the Software without restriction, including without limitation
 *  the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *  and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *  DEALINGS IN THE SOFTWARE.
 *  ============================================================================
 *
 *
 *  This file implements a series of math functions for manipulating a
 *  3x3 matrix.
 *
 *  Created by Eric Phillips on November 8, 2016.
 */

#pragma once

#define _USE_MATH_DEFINES
#include <math.h>


/**
 * Attempt to include a header file if the file exists.
 * If the file does not exist, create a dummy data structure for that type.
 * If it cannot be determined if it exists, just attempt to include it.
 */
#ifdef __has_include
#   if __has_include("Vector3.hpp")
#       include "Vector3.hpp"
#   elif !defined(GMATH_VECTOR3)
        #define GMATH_VECTOR3
        struct Vector3
        {
            union
            {
                struct
                {
                    float X;
                    float Y;
                    float Z;
                };
                float data[3];
            };

            inline Vector3() : X(0), Y(0), Z(0) {}
            inline Vector3(float data[]) : X(data[0]), Y(data[1]), Z(data[2])
                {}
            inline Vector3(float value) : X(value), Y(value), Z(value) {}
            inline Vector3(float x, float y) : X(x), Y(y), Z(0) {}
            inline Vector3(float x, float y, float z) : X(x), Y(y), Z(z) {}

            static inline Vector3 Cross(Vector3 lhs, Vector3 rhs)
            {
                float x = lhs.Y * rhs.Z - lhs.Z * rhs.Y;
                float y = lhs.Z * rhs.X - lhs.X * rhs.Z;
                float z = lhs.X * rhs.Y - lhs.Y * rhs.X;
                return Vector3(x, y, z);
            }

            static inline float Dot(Vector3 lhs, Vector3 rhs)
            {
                return lhs.X * rhs.X + lhs.Y * rhs.Y + lhs.Z * rhs.Z;
            }

            static inline Vector3 Normalized(Vector3 v)
            {
                float mag = sqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z);
                if (mag == 0)
                    return Vector3::Zero();
                return v / mag;
            }

            static inline Vector3 Orthogonal(Vector3 v)
            {
                return v.Z < v.X ?
                    Vector3(v.Y, -v.X, 0) : Vector3(0, -v.Z, v.Y);
            }

            static inline float SqrMagnitude(Vector3 v)
            {
                return v.X * v.X + v.Y * v.Y + v.Z * v.Z;
            }
        };


        inline Vector3 operator+(Vector3 lhs, const Vector3 rhs)
        {
            return Vector3(lhs.X + rhs.X, lhs.Y + rhs.Y, lhs.Z + rhs.Z);
        }

        inline Vector3 operator*(Vector3 lhs, const float rhs)
        {
            return Vector3(lhs.X * rhs, lhs.Y * rhs, lhs.Z * rhs);
        }
#   endif
#else
#   include "Vector3.hpp"
#endif


/**
 * Attempt to include a header file if the file exists.
 * If the file does not exist, create a dummy data structure for that type.
 * If it cannot be determined if it exists, just attempt to include it.
 */
#ifdef __has_include
#   if __has_include("Quaternion.hpp")
#       include "Quaternion.hpp"
#   elif !defined(GMATH_QUATERNION)
        #define GMATH_QUATERNION
        struct Quaternion
        {
            union
            {
                struct
                {
                    float X;
                    float Y;
                    float Z;
                    float W;
                };
                float data[4];
            };

            inline Quaternion() : X(0), Y(0), Z(0), W(1) {}
            inline Quaternion(float data[]) : X(data[0]), Y(data[1]),
                Z(data[2]), W(data[3]) {}
            inline Quaternion(Vector3 vector, float scalar) : X(vector.X),
                Y(vector.Y), Z(vector.Z), W(scalar) {}
            inline Quaternion(float x, float y, float z, float w) : X(x),
                Y(y), Z(z), W(w) {}
        };
#   endif
#else
#   include "Quaternion.hpp"
#endif


struct Matrix3x3
{
    union
    {
        struct
        {
            float D00;
            float D01;
            float D02;
            float D10;
            float D11;
            float D12;
            float D20;
            float D21;
            float D22;
        };
        float data[3][3];
    };


    /**
     * Constructors.
     */
    inline Matrix3x3();
    inline Matrix3x3(float data[]);
    inline Matrix3x3(Vector3 row0, Vector3 row1, Vector3 row2);
    inline Matrix3x3(float d00, float d01, float d02, float d10, float d11,
        float d12, float d20, float d21, float d22);


    /**
     * Constants for common Matrix3x3.
     */
    static inline Matrix3x3 Identity();
    static inline Matrix3x3 Zero();
    static inline Matrix3x3 One();


    /**
     * Returns the determinate of a matrix.
     * @param matrix: The input matrix.
     * @return: A scalar value.
     */
    static inline float Determinate(Matrix3x3 matrix);

    /**
     * Converts a quaternion to a rotation matrix.
     * @param rotation: The input quaternion.
     * @return: A new rotation matrix.
     */
    static inline Matrix3x3 FromQuaternion(Quaternion rotation);

    /**
     * Returns the inverse of a matrix.
     * @param matrix: The input matrix.
     * @return: A new matrix.
     */
    static inline Matrix3x3 Inverse(Matrix3x3 matrix);

    /**
     * Returns true if a matrix is invertible.
     * @param matrix: The input matrix.
     * @return: A new matrix.
     */
    static inline bool IsInvertible(Matrix3x3 matrix);

    /**
     * Multiplies two matrices element-wise.
     * @param a: The left-hand side of the multiplication.
     * @param b: The right-hand side of the multiplication.
     * @return: A new matrix.
     */
    static inline Matrix3x3 Scale(Matrix3x3 a, Matrix3x3 b);

    /**
     * Converts a rotation matrix to a quaternion.
     * @param rotation: The input rotation matrix.
     * @return: A new quaternion.
     */
    static inline Quaternion ToQuaternion(Matrix3x3 rotation);

    /**
     * Returns the transpose of a matrix.
     * @param matrix: The input matrix.
     * @return: A new matrix.
     */
    static inline Matrix3x3 Transpose(Matrix3x3 matrix);

    /**
     * Operator overloading.
     */
    inline struct Matrix3x3& operator+=(const float rhs);
    inline struct Matrix3x3& operator-=(const float rhs);
    inline struct Matrix3x3& operator*=(const float rhs);
    inline struct Matrix3x3& operator/=(const float rhs);
    inline struct Matrix3x3& operator+=(const Matrix3x3 rhs);
    inline struct Matrix3x3& operator-=(const Matrix3x3 rhs);
    inline struct Matrix3x3& operator*=(const Matrix3x3 rhs);
};

inline Matrix3x3 operator-(Matrix3x3 rhs);
inline Matrix3x3 operator+(Matrix3x3 lhs, const float rhs);
inline Matrix3x3 operator-(Matrix3x3 lhs, const float rhs);
inline Matrix3x3 operator*(Matrix3x3 lhs, const float rhs);
inline Matrix3x3 operator/(Matrix3x3 lhs, const float rhs);
inline Matrix3x3 operator+(const float lhs, Matrix3x3 rhs);
inline Matrix3x3 operator-(const float lhs, Matrix3x3 rhs);
inline Matrix3x3 operator*(const float lhs, Matrix3x3 rhs);
inline Matrix3x3 operator+(Matrix3x3 lhs, const Matrix3x3 rhs);
inline Matrix3x3 operator-(Matrix3x3 lhs, const Matrix3x3 rhs);
inline Matrix3x3 operator*(Matrix3x3 lhs, const Matrix3x3 rhs);
inline Vector3 operator*(Matrix3x3 lhs, const Vector3 rhs);
inline bool operator==(const Matrix3x3 lhs, const Matrix3x3 rhs);
inline bool operator!=(const Matrix3x3 lhs, const Matrix3x3 rhs);



/*******************************************************************************
 * Implementation
 */

Matrix3x3::Matrix3x3() : D00(1), D01(0), D02(0), D10(0), D11(1), D12(0), D20(0),
    D21(0), D22(1) {}
Matrix3x3::Matrix3x3(float data[]) : D00(data[0]), D01(data[1]), D02(data[2]),
    D10(data[3]), D11(data[4]), D12(data[5]), D20(data[6]), D21(data[7]),
    D22(data[8]) {}
Matrix3x3::Matrix3x3(Vector3 row0, Vector3 row1, Vector3 row2) : D00(row0.x),
    D01(row0.y), D02(row0.z), D10(row1.x), D11(row1.y), D12(row1.z),
    D20(row2.x), D21(row2.y), D22(row2.z) {}
Matrix3x3::Matrix3x3(float d00, float d01, float d02, float d10, float d11,
    float d12, float d20, float d21, float d22) : D00(d00), D01(d01),
    D02(d02), D10(d10), D11(d11), D12(d12), D20(d20), D21(d21), D22(d22) {}


Matrix3x3 Matrix3x3::Identity()
{
    return Matrix3x3(1, 0, 0, 0, 1, 0, 0, 0, 1);
}

Matrix3x3 Matrix3x3::Zero()
{
    return Matrix3x3(0, 0, 0, 0, 0, 0, 0, 0, 0);
}

Matrix3x3 Matrix3x3::One()
{
    return Matrix3x3(1, 1, 1, 1, 1, 1, 1, 1, 1);
}


float Matrix3x3::Determinate(Matrix3x3 matrix)
{
    float v1 = matrix.D00 * (matrix.D22 * matrix.D11 -
        matrix.D21 * matrix.D12);
    float v2 = matrix.D10 * (matrix.D22 * matrix.D01 -
        matrix.D21 * matrix.D02);
    float v3 = matrix.D20 * (matrix.D12 * matrix.D01 -
        matrix.D11 * matrix.D02);
    return v1 - v2 + v3;
}

Matrix3x3 Matrix3x3::FromQuaternion(Quaternion rotation)
{
    Matrix3x3 m;
    float sqw = rotation.w * rotation.w;
    float sqx = rotation.x * rotation.x;
    float sqy = rotation.y * rotation.y;
    float sqz = rotation.z * rotation.z;

    float invSqr = 1 / (sqx + sqy + sqz + sqw);
    m.D00 = (sqx - sqy - sqz + sqw) * invSqr;
    m.D11 = (-sqx + sqy - sqz + sqw) * invSqr;
    m.D22 = (-sqx - sqy + sqz + sqw) * invSqr;

    float tmp1 = rotation.x * rotation.y;
    float tmp2 = rotation.z * rotation.w;
    m.D10 = 2.0f * (tmp1 + tmp2) * invSqr;
    m.D01 = 2.0f * (tmp1 - tmp2) * invSqr;

    tmp1 = rotation.x * rotation.z;
    tmp2 = rotation.y * rotation.w;
    m.D20 = 2.0f * (tmp1 - tmp2) * invSqr;
    m.D02 = 2.0f * (tmp1 + tmp2) * invSqr;
    tmp1 = rotation.y * rotation.z;
    tmp2 = rotation.x * rotation.w;
    m.D21 = 2.0f * (tmp1 + tmp2) * invSqr;
    m.D12 = 2.0f * (tmp1 - tmp2) * invSqr;
    return m;
}

Matrix3x3 Matrix3x3::Inverse(Matrix3x3 matrix)
{
    Matrix3x3 a;
    a.D00 = matrix.D22 * matrix.D11 - matrix.D21 * matrix.D12;
    a.D01 = matrix.D21 * matrix.D02 - matrix.D22 * matrix.D01;
    a.D02 = matrix.D12 * matrix.D01 - matrix.D11 * matrix.D02;
    a.D10 = matrix.D20 * matrix.D12 - matrix.D22 * matrix.D10;
    a.D11 = matrix.D22 * matrix.D00 - matrix.D20 * matrix.D02;
    a.D12 = matrix.D10 * matrix.D02 - matrix.D12 * matrix.D00;
    a.D20 = matrix.D21 * matrix.D10 - matrix.D20 * matrix.D11;
    a.D21 = matrix.D20 * matrix.D01 - matrix.D21 * matrix.D00;
    a.D22 = matrix.D11 * matrix.D00 - matrix.D10 * matrix.D01;
    return 1 / Determinate(matrix) * a;
}

bool Matrix3x3::IsInvertible(Matrix3x3 matrix)
{
    return fabs(Determinate(matrix)) > 0.00001;
}

Matrix3x3 Matrix3x3::Scale(Matrix3x3 a, Matrix3x3 b)
{
    Matrix3x3 m;
    m.D00 = a.D00 * b.D00;
    m.D01 = a.D01 * b.D01;
    m.D02 = a.D02 * b.D02;
    m.D10 = a.D10 * b.D10;
    m.D11 = a.D11 * b.D11;
    m.D12 = a.D12 * b.D12;
    m.D20 = a.D20 * b.D20;
    m.D21 = a.D21 * b.D21;
    m.D22 = a.D22 * b.D22;
    return m;
}

Quaternion Matrix3x3::ToQuaternion(Matrix3x3 rotation)
{
    Quaternion q;
    float trace = rotation.D00 + rotation.D11 + rotation.D22;
    if (trace > 0)
    {
        float s = 0.5f / sqrt(trace + 1);
        q.w = 0.25f / s;
        q.x = (rotation.D21 - rotation.D12) * s;
        q.y = (rotation.D02 - rotation.D20) * s;
        q.z = (rotation.D10 - rotation.D01) * s;
    }
    else
    {
        if (rotation.D00 > rotation.D11 && rotation.D00 > rotation.D22)
        {
            float s = 2 * sqrt(1 + rotation.D00 - rotation.D11 - rotation.D22);
            q.w = (rotation.D21 - rotation.D12) / s;
            q.x = 0.25f * s;
            q.y = (rotation.D01 + rotation.D10) / s;
            q.z = (rotation.D02 + rotation.D20) / s;
        }
        else if (rotation.D11 > rotation.D22)
        {
            float s = 2 * sqrt(1 + rotation.D11 - rotation.D00 - rotation.D22);
            q.w = (rotation.D02 - rotation.D20) / s;
            q.x = (rotation.D01 + rotation.D10) / s;
            q.y = 0.25f * s;
            q.z = (rotation.D12 + rotation.D21) / s;
        }
        else
        {
            float s = 2 * sqrt(1 + rotation.D22 - rotation.D00 - rotation.D11);
            q.w = (rotation.D10 - rotation.D01) / s;
            q.x = (rotation.D02 + rotation.D20) / s;
            q.y = (rotation.D12 + rotation.D21) / s;
            q.z = 0.25f * s;
        }
    }
    return q;
}

Matrix3x3 Matrix3x3::Transpose(Matrix3x3 matrix)
{
    float tmp;
    tmp = matrix.D01;
    matrix.D01 = matrix.D10;
    matrix.D10 = tmp;
    tmp = matrix.D02;
    matrix.D02 = matrix.D20;
    matrix.D20 = tmp;
    tmp = matrix.D12;
    matrix.D12 = matrix.D21;
    matrix.D21 = tmp;
    return matrix;
}


struct Matrix3x3& Matrix3x3::operator+=(const float rhs)
{
    D00 += rhs; D01 += rhs; D02 += rhs;
    D10 += rhs; D11 += rhs; D12 += rhs;
    D20 += rhs; D21 += rhs; D22 += rhs;
    return *this;
}

struct Matrix3x3& Matrix3x3::operator-=(const float rhs)
{
    D00 -= rhs; D01 -= rhs; D02 -= rhs;
    D10 -= rhs; D11 -= rhs; D12 -= rhs;
    D20 -= rhs; D21 -= rhs; D22 -= rhs;
    return *this;
}

struct Matrix3x3& Matrix3x3::operator*=(const float rhs)
{
    D00 *= rhs; D01 *= rhs; D02 *= rhs;
    D10 *= rhs; D11 *= rhs; D12 *= rhs;
    D20 *= rhs; D21 *= rhs; D22 *= rhs;
    return *this;
}

struct Matrix3x3& Matrix3x3::operator/=(const float rhs)
{
    D00 /= rhs; D01 /= rhs; D02 /= rhs;
    D10 /= rhs; D11 /= rhs; D12 /= rhs;
    D20 /= rhs; D21 /= rhs; D22 /= rhs;
    return *this;
}

struct Matrix3x3& Matrix3x3::operator+=(const Matrix3x3 rhs)
{
    D00 += rhs.D00; D01 += rhs.D01; D02 += rhs.D02;
    D10 += rhs.D10; D11 += rhs.D11; D12 += rhs.D12;
    D20 += rhs.D20; D21 += rhs.D21; D22 += rhs.D22;
    return *this;
}

struct Matrix3x3& Matrix3x3::operator-=(const Matrix3x3 rhs)
{
    D00 -= rhs.D00; D01 -= rhs.D01; D02 -= rhs.D02;
    D10 -= rhs.D10; D11 -= rhs.D11; D12 -= rhs.D12;
    D20 -= rhs.D20; D21 -= rhs.D21; D22 -= rhs.D22;
    return *this;
}

struct Matrix3x3& Matrix3x3::operator*=(const Matrix3x3 rhs)
{
    Matrix3x3 m;
    m.D00 = D00 * rhs.D00 + D01 * rhs.D10 + D02 * rhs.D20;
    m.D01 = D00 * rhs.D01 + D01 * rhs.D11 + D02 * rhs.D21;
    m.D02 = D00 * rhs.D02 + D01 * rhs.D12 + D02 * rhs.D22;
    m.D10 = D10 * rhs.D00 + D11 * rhs.D10 + D12 * rhs.D20;
    m.D11 = D10 * rhs.D01 + D11 * rhs.D11 + D12 * rhs.D21;
    m.D12 = D10 * rhs.D02 + D11 * rhs.D12 + D12 * rhs.D22;
    m.D20 = D20 * rhs.D00 + D21 * rhs.D10 + D22 * rhs.D20;
    m.D21 = D20 * rhs.D01 + D21 * rhs.D11 + D22 * rhs.D21;
    m.D22 = D20 * rhs.D02 + D21 * rhs.D12 + D22 * rhs.D22;
    *this = m;
    return *this;
}

Matrix3x3 operator-(Matrix3x3 rhs) { return rhs * -1; }
Matrix3x3 operator+(Matrix3x3 lhs, const float rhs) { return lhs += rhs; }
Matrix3x3 operator-(Matrix3x3 lhs, const float rhs) { return lhs -= rhs; }
Matrix3x3 operator*(Matrix3x3 lhs, const float rhs) { return lhs *= rhs; }
Matrix3x3 operator/(Matrix3x3 lhs, const float rhs) { return lhs /= rhs; }
Matrix3x3 operator+(const float lhs, Matrix3x3 rhs) { return rhs += lhs; }
Matrix3x3 operator-(const float lhs, Matrix3x3 rhs) { return rhs -= lhs; }
Matrix3x3 operator*(const float lhs, Matrix3x3 rhs) { return rhs *= lhs; }
Matrix3x3 operator+(Matrix3x3 lhs, const Matrix3x3 rhs) { return lhs += rhs; }
Matrix3x3 operator-(Matrix3x3 lhs, const Matrix3x3 rhs) { return lhs -= rhs; }
Matrix3x3 operator*(Matrix3x3 lhs, const Matrix3x3 rhs) { return lhs *= rhs; }

Vector3 operator*(Matrix3x3 lhs, const Vector3 rhs)
{
    Vector3 v;
    v.x = lhs.D00 * rhs.x + lhs.D01 * rhs.y + lhs.D02 * rhs.z;
    v.y = lhs.D10 * rhs.x + lhs.D11 * rhs.y + lhs.D12 * rhs.z;
    v.z = lhs.D20 * rhs.x + lhs.D21 * rhs.y + lhs.D22 * rhs.z;
    return v;
}

bool operator==(const Matrix3x3 lhs, const Matrix3x3 rhs)
{
    return lhs.D00 == rhs.D00 &&
        lhs.D01 == rhs.D01 &&
        lhs.D02 == rhs.D02 &&
        lhs.D10 == rhs.D10 &&
        lhs.D11 == rhs.D11 &&
        lhs.D12 == rhs.D12 &&
        lhs.D20 == rhs.D20 &&
        lhs.D21 == rhs.D21 &&
        lhs.D22 == rhs.D22;
}

bool operator!=(const Matrix3x3 lhs, const Matrix3x3 rhs)
{
    return !(lhs == rhs);
}
