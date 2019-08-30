//
// Created by sitan on 19-8-29.
//

#ifndef RAYTRACING_MATH_H
#define RAYTRACING_MATH_H

#include <iostream>
#include <cmath>
#include <limits>

using REAL = float;


struct vec3 {
    vec3() {
        x = 0;
        y = 0;
        z = 0;
    }

    vec3( REAL _x, REAL _y , REAL _z )
    {
        x = _x;
        y = _y;
        z = _z;
    }

    explicit  vec3( REAL v)
    {
        x = v;
        y = v;
        z = v;
    }

    union {
        REAL r;
        REAL x;
    };
    union {
        REAL g;
        REAL y;
    };
    union {
        REAL b;
        REAL z;
    };

    bool operator==(const vec3 other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const vec3 other) const
    {
        return !operator==(other);
    }

    vec3 operator+( const vec3 other ) const
    {
        return vec3{x+other.x, y +other.y , z +other.z};
    }

    vec3 operator-(const vec3 other) const
    {
        return vec3{x-other.x, y-other.y , z-other.z};
    }

    void normalize()
    {
        if( square_length() == 0)
        {
            x = std::numeric_limits<REAL>::quiet_NaN();
            y = x;
            z = x;
        }
        else
        {
            REAL k = 1/length();
            x*=k;
            y*=k;
            z*=k;
        }

    }

    REAL length() const
    {
        return std::sqrt( square_length());
    }

    REAL square_length() const
    {
        return x*x + y*y + z*z;
    }

    REAL dot( const vec3& other) const
    {
        return x*other.x + y*other.y + z*other.z;
    }

    vec3 product( const vec3 other) const
    {
        return vec3( y*other.z - other.y*z ,
                     z*other.x - other.z*x,
                     x*other.y - y*other.x
                );
    }

    const vec3 operator+() const {
        return *this;
    }

    const vec3 operator-() const{
        return vec3(-x,-y,-z);
    }

    vec3 getNormal() const
    {
        vec3 n(*this);
        n.normalize();
        return n;
    }

};

template <class T>
vec3 operator*(const vec3 vector, const T v)
{
    return vec3{vector.r*v,vector.g*v,vector.b*v};
}

template<typename T>
vec3 operator*( const T v, const vec3 vector)
{
    return vec3{vector.r*v,vector.g*v,vector.b*v};
}

std::ostream& operator<<(std::ostream& os, const vec3 vector)
{
    os << "[" << vector.x << "," << vector.y << "," << vector.z <<"]";
    return os;
}

template<typename T>
T lerp(const T v1, const T v2, REAL t)
{
    return v1*t+v2*(1-t);
}

const vec3 V3_ZERO= vec3{0,0,0};
const vec3 V3_UNIT_X = vec3{1.f,0,0};
const vec3 V3_UNIT_Y = vec3{0,1.f,0};
const vec3 V3_UNIT_Z = vec3{0,0,1.f};


inline bool isLess(const REAL t1, const REAL t2)
{
    return t1 < t2;
}

inline bool isEqual(const REAL t1, const REAL t2)
{
    return t1 == t2;
}

inline bool isGreat(const REAL t1, const REAL t2)
{
    return t1 > t2;
}
#endif //RAYTRACING_MATH_H
