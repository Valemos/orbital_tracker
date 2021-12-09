#include <gtest/gtest.h>

#include "OrbitalTracker.h"

const char* connection_url = "postgresql://postgres:password@localhost:5432/test";


class TestBasicOperations : public ::testing::Test {
protected:
    OrbitalTracker tracker {connection_url};

    OrbitalBodyRecord body_test {"b", {1, 0, 0}, {1, 0, 0}, 1};

    void SetUp() override {
        tracker.DropTables();
        tracker.CreateTables();
    }

    void TearDown() override {
        tracker.DropTables();
    }
};

TEST_F(TestBasicOperations, TestReadNonExistent) {
    ASSERT_ANY_THROW(tracker.GetByName(body_test.name));
}

TEST_F(TestBasicOperations, TestCreate) {
    tracker.InsertNew(body_test);

    auto new_body = tracker.GetByName(body_test.name);
    ASSERT_EQ(new_body, body_test);
}

TEST_F(TestBasicOperations, TestUpdate) {
    tracker.InsertNew(body_test);

    auto new_body = tracker.GetByName(body_test.name);
    ASSERT_EQ(new_body, body_test);

    body_test.position.x = 1000;
    tracker.Update(body_test);

    new_body = tracker.GetByName(body_test.name);
    ASSERT_EQ(new_body.position.x, 1000);
}

TEST_F(TestBasicOperations, TestDelete) {
    tracker.InsertNew(body_test);

    auto new_body = tracker.GetByName(body_test.name);
    ASSERT_EQ(new_body, body_test);

    tracker.DeleteByName(body_test.name);
    ASSERT_ANY_THROW(tracker.GetByName(body_test.name));
}

TEST_F(TestBasicOperations, TestPredictionCollided) {
    tracker.InsertNew(body_test);

    auto collided = tracker.PredictCollisions(body_test.position, 1, 1);
    EXPECT_EQ(collided.size(), 1);

    collided = tracker.PredictCollisions(body_test.position, 1, 1);
    EXPECT_EQ(collided.size(), 1);
}

TEST_F(TestBasicOperations, TestPredictionNotCollided) {
    tracker.InsertNew(body_test);

    auto collided = tracker.PredictCollisions({-1, 0, 0}, 1, 1);
    EXPECT_EQ(collided.size(), 0) << "sphere before error";

    collided = tracker.PredictCollisions({5, 0, 0}, 1, 1);
    EXPECT_EQ(collided.size(), 0) << "sphere after error";
}
