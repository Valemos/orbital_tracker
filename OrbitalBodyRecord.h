#pragma once

#include "Vector3.h"

#include <string>
#include <pqxx/row.hxx>


struct OrbitalBodyRecord {
    std::string name;
    Vector3<float> position {};
    Vector3<float> speed {};
    double timestamp{};

    bool operator==(const OrbitalBodyRecord &rhs) const {
        return name == rhs.name &&
               position == rhs.position &&
               speed == rhs.speed &&
               timestamp == rhs.timestamp;
    }

    bool operator!=(const OrbitalBodyRecord &rhs) const {
        return !(rhs == *this);
    }

    static OrbitalBodyRecord from_row(const pqxx::row& row) {
        OrbitalBodyRecord body {};
        body.name = row[0].as<std::string>();
        body.position.x = row[1].as<float>();
        body.position.y = row[2].as<float>();
        body.position.z = row[3].as<float>();
        body.speed.x = row[4].as<float>();
        body.speed.y = row[5].as<float>();
        body.speed.z = row[6].as<float>();
        body.timestamp = row[7].as<double>();
        return body;
    }

    [[nodiscard]] std::tuple<std::string, float, float, float, float, float, float, double> to_tuple() const {
        return {
            name,
            position.x,
            position.y,
            position.z,
            speed.x,
            speed.y,
            speed.z,
            timestamp,
        };
    }
};



