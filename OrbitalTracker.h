#pragma once

#include "OrbitalBodyRecord.h"

#include <pqxx/pqxx>

#include <string>

class OrbitalTracker {

public:
    explicit OrbitalTracker(const std::string& connection_url);

    /// initialises database tables for current connection
    void CreateTables();
    /// deletes database tables for current connection if they exist
    void DropTables();

    /// performs an INSERT query into special orbital body table
    /// @param body [in] orbital body structure to insert
    void InsertNew(const OrbitalBodyRecord& body);
    /// performs an UPDATE query at orbital body table using name as primary key
    /// @param body [in] orbital body structure to update
    void Update(const OrbitalBodyRecord& body);
    /// performs DELETE query using name as primary key
    /// @param name [in] name of object to delete
    void DeleteByName(const std::string& name);
    /// performs SELECT query for orbital body by name
    /// @param name [in] name to search
    OrbitalBodyRecord GetByName(const std::string& name);

    /// searches all orbital objects and tries to predict if they can hit given sphere in space
    /// @param sphere_center [in] a fixed point in space
    /// @param radius [in] radius relative to sphere_center
    /// @param prediction_time [in] a double precision timestamp when we need to check for collisions
    std::vector<OrbitalBodyRecord> PredictCollisions(const Vector3<float> &sphere_center,
                                                     float radius,
                                                     double prediction_time);

private:
    pqxx::connection m_connection;
    const std::string m_table_name {"object"};
    const std::string m_insert_name {"insert"};
    const std::string m_update_name {"update"};
    const std::string m_delete_name {"delete"};
    const std::string m_get_name {"get_by_name"};

    static bool rayHitsSphere(const Vector3<float>& center,
                              float radius,
                              const Vector3<float>& from,
                              const Vector3<float>& direction);

    static bool checkTimePeriod(const OrbitalBodyRecord &body, float max_distance, double prediction_time) ;
};
