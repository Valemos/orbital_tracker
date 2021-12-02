#pragma once

#include "OrbitalBody.h"

#include <pqxx/pqxx>

#include <string>

class OrbitalTracker {

public:
    explicit OrbitalTracker(const std::string& connection_url);

    void InsertNew(const OrbitalBody& body);
    void Update(const OrbitalBody& body);
    void DeleteByName(const std::string& name);
    OrbitalBody GetByName(const std::string& name);

    void DropTables();

    void CreateTables();

private:
    pqxx::connection m_connection;
    const std::string m_table_name {"object"};
    const std::string m_insert_name {"insert"};
    const std::string m_update_name {"update"};
    const std::string m_delete_name {"delete"};
    const std::string m_get_name {"get_by_name"};
};
