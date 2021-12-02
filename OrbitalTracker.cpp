#include <functional>
#include "OrbitalTracker.h"


OrbitalTracker::OrbitalTracker(const std::string &connection_url) :
    m_connection(connection_url) {

    auto all_fields = "($1, $2, $3, $4, $5, $6, $7)";
    m_connection.prepare(m_insert_name, "INSERT INTO " + m_table_name + " VALUES " + all_fields);
    m_connection.prepare(m_get_name, "SELECT * from " + m_table_name + " WHERE name=$1");
    m_connection.prepare(m_delete_name, "DELETE FROM " + m_table_name + " WHERE name=$1");
    m_connection.prepare(m_update_name, "UPDATE " + m_table_name + " SET " + all_fields + " WHERE name=$8");
}

void OrbitalTracker::InsertNew(const OrbitalBody &body) {
    pqxx::work w {m_connection};
    try{
        w.exec_prepared(m_insert_name,
                        body.name,
                        body.position.x,
                        body.position.y,
                        body.position.z,
                        body.speed.x,
                        body.speed.y,
                        body.speed.z);
        w.commit();
    } catch (std::exception& exc) {
        throw std::runtime_error("cannot insert item");
    }
}

void OrbitalTracker::Update(const OrbitalBody &body) {
    pqxx::work w {m_connection};
    w.exec_prepared(m_insert_name,
                    body.name,
                    body.position.x,
                    body.position.y,
                    body.position.z,
                    body.speed.x,
                    body.speed.y,
                    body.speed.z);
    w.commit();
}

void OrbitalTracker::DeleteByName(const std::string &name) {
    pqxx::work w {m_connection};
    try {
        w.exec_prepared(m_delete_name, name);
        w.commit();
    } catch (std::exception& exc) {
        throw std::runtime_error("cannot delete item");
    }
}

OrbitalBody OrbitalTracker::GetByName(const std::string &name) {
    pqxx::work w {m_connection};
    try{
        auto result = w.exec_prepared1(m_get_name, name);
        return OrbitalBody::from_row(result);
    } catch (pqxx::plpgsql_no_data_found& exc) {
        throw std::runtime_error("cannot get item");
    }
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
           "  \"z_speed\" real NOT NULL\n"
           ");");
    w.commit();
}

void OrbitalTracker::DropTables() {
    pqxx::work w {m_connection};
    w.exec("DROP TABLE IF EXISTS \"" + m_table_name + "\";");
    w.commit();
}
