#include <functional>
#include "OrbitalTracker.h"


OrbitalTracker::OrbitalTracker(const std::string &connection_url) :
    m_connection(connection_url) {

    m_connection.prepare(m_insert_name, "INSERT INTO " + m_table_name +
                            " VALUES ($1, $2, $3, $4, $5, $6, $7, $8)");
    m_connection.prepare(m_get_name, "SELECT * from " + m_table_name + " WHERE name=$1");
    m_connection.prepare(m_delete_name, "DELETE FROM " + m_table_name + " WHERE name=$1");

    m_connection.prepare(m_update_name,
                         "UPDATE " + m_table_name + "\n"
                        "SET "
                        "\"name\"=$1,"
                        "\"x_pos\"=$2,"
                        "\"y_pos\"=$3,"
                        "\"z_pos\"=$4,"
                        "\"x_speed\"=$5,"
                        "\"y_speed\"=$6,"
                        "\"z_speed\"=$7,"
                        "\"record_time\"=$8\n"
                        " WHERE name=$1");
}

void OrbitalTracker::CreateTables() {
    pqxx::work w {m_connection};
    w.exec("CREATE TABLE \"" + m_table_name + "\" (\n"
                                              "  \"name\" character varying(100) NOT NULL,\n"
                                              "  \"x_pos\" real NOT NULL,\n"
                                              "  \"y_pos\" real NOT NULL,\n"
                                              "  \"z_pos\" real NOT NULL,\n"
                                              "  \"x_speed\" real NOT NULL,\n"
                                              "  \"y_speed\" real NOT NULL,\n"
                                              "  \"z_speed\" real NOT NULL,\n"
                                              "  \"record_time\" double precision NOT NULL\n"
                                              ");");
    w.commit();
}

void OrbitalTracker::DropTables() {
    pqxx::work w {m_connection};
    w.exec("DROP TABLE IF EXISTS \"" + m_table_name + "\";");
    w.commit();
}

void OrbitalTracker::InsertNew(const OrbitalBodyRecord &body) {
    pqxx::work w {m_connection};
    w.exec_prepared(m_insert_name,
                    body.name,
                    body.position.x,
                    body.position.y,
                    body.position.z,
                    body.speed.x,
                    body.speed.y,
                    body.speed.z,
                    body.timestamp);
    w.commit();
}

void OrbitalTracker::Update(const OrbitalBodyRecord &body) {
    pqxx::work w {m_connection};
    w.exec_prepared(m_update_name,
                    body.name,
                    body.position.x,
                    body.position.y,
                    body.position.z,
                    body.speed.x,
                    body.speed.y,
                    body.speed.z,
                    body.timestamp);
    w.commit();
}

void OrbitalTracker::DeleteByName(const std::string &name) {
    pqxx::work w {m_connection};
    w.exec_prepared(m_delete_name, name);
    w.commit();
}

OrbitalBodyRecord OrbitalTracker::GetByName(const std::string &name) {
    pqxx::work w {m_connection};
    auto result = w.exec_prepared1(m_get_name, name);
    return OrbitalBodyRecord::from_row(result);
}

std::vector<OrbitalBodyRecord> OrbitalTracker::PredictCollisions(
        const Vector3<float> &sphere_center,
        float radius,
        double prediction_time) {

    std::vector<OrbitalBodyRecord> results{};

    pqxx::work w {m_connection};
    for (const auto& row : w.exec("SELECT * FROM \"" + m_table_name + "\"")) {
        auto body = OrbitalBodyRecord::from_row(row);

        if (not rayHitsSphere(sphere_center, radius, body.position, body.speed.Normal())) {
            continue;
        }

        if (not checkTimePeriod(body, radius, prediction_time)) {
            continue;
        }

        auto distance_to_center = (sphere_center - body.position).Magnitude();
        if (distance_to_center < radius) {
            results.emplace_back(body);
        }
    }

    return results;
}

bool OrbitalTracker::checkTimePeriod(const OrbitalBodyRecord &body,
                                     float max_distance,
                                     double prediction_time) {
    auto body_movement_time = max_distance / body.speed.Magnitude();
    return body.timestamp >= prediction_time - body_movement_time and
           body.timestamp <= prediction_time + body_movement_time;
}

bool OrbitalTracker::rayHitsSphere(const Vector3<float>& center,
                                   float radius,
                                   const Vector3<float>& from,
                                   const Vector3<float>& direction) {
    Vector3<float> m = from - center;
    float b = m.Dot(direction);
    float c = m.Dot(m) - radius * radius;

    if (c > 0.0f && b > 0.0f)
        return false;

    float discriminant = b * b - c;
    return discriminant >= 0.0f;
}
