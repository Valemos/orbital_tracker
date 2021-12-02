#pragma once

#include "Vector3.h"

#include <string>
#include <pqxx/row.hxx>


struct OrbitalBody {
    std::string name;
    Vector3<double> position {};
    Vector3<double> speed {};

    bool operator==(const OrbitalBody &rhs) const {
        return name == rhs.name &&
               position == rhs.position &&
               speed == rhs.speed;
    }

    bool operator!=(const OrbitalBody &rhs) const {
        return !(rhs == *this);
    }

    static OrbitalBody from_row(const pqxx::row& row) {
        OrbitalBody body {};
        body.name = row[0].as<std::string>();
        body.position.x = row[1].as<double>();
        body.position.y = row[2].as<double>();
        body.position.z = row[3].as<double>();
        body.speed.x = row[4].as<double>();
        body.speed.y = row[5].as<double>();
        body.speed.z = row[6].as<double>();
        return body;
    }

    [[nodiscard]] std::tuple<std::string, double, double, double, double, double, double> to_tuple() const {
        return {
            name,
            position.x,
            position.y,
            position.z,
            speed.x,
            speed.y,
            speed.z,
        };
    }
};



