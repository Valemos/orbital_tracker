#include <gtest/gtest.h>

#include "OrbitalTracker.h"

const char* connection_url = "postgresql://postgres:password@localhost:5432/orbital_objects";


class TestBasicOperations : public ::testing::Test {
protected:
    OrbitalTracker tracker {connection_url};

    void SetUp() override {
        tracker.DropTables();
        tracker.CreateTables();
    }

    void TearDown() override {
        tracker.DropTables();
    }
};


TEST_F(TestBasicOperations, TestCreate) {
    OrbitalBody body {"b", {1, 1, 1}, {2, 2, 2}};

    tracker.InsertNew(body);

    auto new_body = tracker.GetByName(body.name);
    ASSERT_EQ(new_body, body);
}
