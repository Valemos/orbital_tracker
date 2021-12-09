#pragma once

#import <cmath>

template<class T>
struct Vector3 {
    T x = 0;
    T y = 0;
    T z = 0;

    bool operator==(const Vector3 &rhs) const {
        return x == rhs.x &&
               y == rhs.y &&
               z == rhs.z;
    }

    bool operator!=(const Vector3 &rhs) const {
        return rhs != *this;
    }

    Vector3 operator+(const Vector3 &rhs) const {
        return {
            x + rhs.x,
            y + rhs.y,
            z + rhs.z
        };
    }

    Vector3 operator-(const Vector3 &rhs) const {
        return this->operator+({-rhs.x, -rhs.y, -rhs.z});
    }

    [[nodiscard]] float Magnitude() const {
        return std::sqrt(x*x + y*y + z*z);
    }

    [[nodiscard]] float Dot(const Vector3<float> &other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    [[nodiscard]] Vector3 Normal() const {
        auto magnitude = Magnitude();
        return {x / magnitude, y / magnitude, z / magnitude};
    }
};
