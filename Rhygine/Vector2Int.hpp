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
 *  2D vector.
 *
 *  Created by Eric Phillips on October 15, 2016.
 */

#pragma once

#define _USE_MATH_DEFINES
#include <math.h>


struct Vector2Int
{
    union
    {
        struct
        {
            int x;
            int y;
        };
        int data[2];
    };


    /**
     * Constructors.
     */
    inline Vector2Int();
    inline Vector2Int(int data[]);
    inline Vector2Int(int value);
    inline Vector2Int(int x, int y);


    /**
     * Constants for common vectors.
     */
    static inline Vector2Int Zero();
    static inline Vector2Int One();
    static inline Vector2Int Right();
    static inline Vector2Int Left();
    static inline Vector2Int Up();
    static inline Vector2Int Down();

    /**
     * Returns the magnitude of a vector.
     * @param v: The vector in question.
     * @return: A scalar value.
     */
    static inline float Magnitude(Vector2Int v);

    /**
     * Returns a vector made from the largest components of two other vectors.
     * @param a: The first vector.
     * @param b: The second vector.
     * @return: A new vector.
     */
    static inline Vector2Int Max(Vector2Int a, Vector2Int b);

    /**
     * Returns a vector made from the smallest components of two other vectors.
     * @param a: The first vector.
     * @param b: The second vector.
     * @return: A new vector.
     */
    static inline Vector2Int Min(Vector2Int a, Vector2Int b);

    /**
     * Returns a vector "maxDistanceDelta" units closer to the target. This
     * interpolation is in a straight line, and will not overshoot.
     * @param current: The current position.
     * @param target: The destination position.
     * @param maxDistanceDelta: The maximum distance to move.
     * @return: A new vector.
     */
    static inline Vector2Int MoveTowards(Vector2Int current, Vector2Int target,
        float maxDistanceDelta);

    /**
    /**
     * Multiplies two vectors component-wise.
     * @param a: The lhs of the multiplication.
     * @param b: The rhs of the multiplication.
     * @return: A new vector.
     */
    static inline Vector2Int Scale(Vector2Int a, Vector2Int b);

    /**
     * Returns the squared magnitude of a vector.
     * This is useful when comparing relative lengths, where the exact length
     * is not important, and much time can be saved by not calculating the
     * square root.
     * @param v: The vector in question.
     * @return: A scalar value.
     */
    static inline float SqrMagnitude(Vector2Int v);

    /**
     * Operator overloading.
     */
    inline struct Vector2Int& operator+=(const int rhs);
    inline struct Vector2Int& operator-=(const int rhs);
    inline struct Vector2Int& operator*=(const int rhs);
    inline struct Vector2Int& operator/=(const int rhs);
    inline struct Vector2Int& operator+=(const Vector2Int rhs);
    inline struct Vector2Int& operator-=(const Vector2Int rhs);
};

inline Vector2Int operator-(Vector2Int rhs);
inline Vector2Int operator+(Vector2Int lhs, const int rhs);
inline Vector2Int operator-(Vector2Int lhs, const int rhs);
inline Vector2Int operator*(Vector2Int lhs, const int rhs);
inline Vector2Int operator/(Vector2Int lhs, const int rhs);
inline Vector2Int operator+(const int lhs, Vector2Int rhs);
inline Vector2Int operator-(const int lhs, Vector2Int rhs);
inline Vector2Int operator*(const int lhs, Vector2Int rhs);
inline Vector2Int operator/(const int lhs, Vector2Int rhs);
inline Vector2Int operator+(Vector2Int lhs, const Vector2Int rhs);
inline Vector2Int operator-(Vector2Int lhs, const Vector2Int rhs);
inline bool operator==(const Vector2Int lhs, const Vector2Int rhs);
inline bool operator!=(const Vector2Int lhs, const Vector2Int rhs);



/*******************************************************************************
 * Implementation
 */

Vector2Int::Vector2Int() : x(0), y(0) {}
Vector2Int::Vector2Int(int data[]) : x(data[0]), y(data[1]) {}
Vector2Int::Vector2Int(int value) : x(value), y(value) {}
Vector2Int::Vector2Int(int x, int y) : x(x), y(y) {}


Vector2Int Vector2Int::Zero() { return Vector2Int(0, 0); }
Vector2Int Vector2Int::One() { return Vector2Int(1, 1); }
Vector2Int Vector2Int::Right() { return Vector2Int(1, 0); }
Vector2Int Vector2Int::Left() { return Vector2Int(-1, 0); }
Vector2Int Vector2Int::Up() { return Vector2Int(0, 1); }
Vector2Int Vector2Int::Down() { return Vector2Int(0, -1); }


float Vector2Int::Magnitude(Vector2Int v)
{
    return static_cast<float>(sqrt(SqrMagnitude(v)));
}

Vector2Int Vector2Int::Max(Vector2Int a, Vector2Int b)
{
    int x = a.x > b.x ? a.x : b.x;
    int y = a.y > b.y ? a.y : b.y;
    return Vector2Int(x, y);
}

Vector2Int Vector2Int::Min(Vector2Int a, Vector2Int b)
{
    int x = a.x > b.x ? b.x : a.x;
    int y = a.y > b.y ? b.y : a.y;
    return Vector2Int(x, y);
}

Vector2Int Vector2Int::MoveTowards(Vector2Int current, Vector2Int target,
    float maxDistanceDelta)
{
    Vector2Int d = target - current;
    float m = Magnitude(d);
    if (m < maxDistanceDelta || m == 0)
        return target;
    return current + (d * static_cast<int>(round(maxDistanceDelta / m)));
}

Vector2Int Vector2Int::Scale(Vector2Int a, Vector2Int b)
{
    return Vector2Int(a.x * b.x, a.y * b.y);
}

float Vector2Int::SqrMagnitude(Vector2Int v)
{
    return static_cast<float>(v.x * v.x + v.y * v.y);
}

struct Vector2Int& Vector2Int::operator+=(const int rhs)
{
    x += rhs;
    y += rhs;
    return *this;
}

struct Vector2Int& Vector2Int::operator-=(const int rhs)
{
    x -= rhs;
    y -= rhs;
    return *this;
}

struct Vector2Int& Vector2Int::operator*=(const int rhs)
{
    x *= rhs;
    y *= rhs;
    return *this;
}

struct Vector2Int& Vector2Int::operator/=(const int rhs)
{
    x /= rhs;
    y /= rhs;
    return *this;
}

struct Vector2Int& Vector2Int::operator+=(const Vector2Int rhs)
{
    x += rhs.x;
    y += rhs.y;
    return *this;
}

struct Vector2Int& Vector2Int::operator-=(const Vector2Int rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}

Vector2Int operator-(Vector2Int rhs) { return rhs * -1; }
Vector2Int operator+(Vector2Int lhs, const int rhs) { return lhs += rhs; }
Vector2Int operator-(Vector2Int lhs, const int rhs) { return lhs -= rhs; }
Vector2Int operator*(Vector2Int lhs, const int rhs) { return lhs *= rhs; }
Vector2Int operator/(Vector2Int lhs, const int rhs) { return lhs /= rhs; }
Vector2Int operator+(const int lhs, Vector2Int rhs) { return rhs += lhs; }
Vector2Int operator-(const int lhs, Vector2Int rhs) { return rhs -= lhs; }
Vector2Int operator*(const int lhs, Vector2Int rhs) { return rhs *= lhs; }
Vector2Int operator/(const int lhs, Vector2Int rhs) { return rhs /= lhs; }
Vector2Int operator+(Vector2Int lhs, const Vector2Int rhs) { return lhs += rhs; }
Vector2Int operator-(Vector2Int lhs, const Vector2Int rhs) { return lhs -= rhs; }

bool operator==(const Vector2Int lhs, const Vector2Int rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator!=(const Vector2Int lhs, const Vector2Int rhs)
{
    return !(lhs == rhs);
}
