#pragma once

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
};
