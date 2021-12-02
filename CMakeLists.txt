cmake_minimum_required(VERSION 3.20)
project(orbital_tracking)

set(CMAKE_CXX_STANDARD 20)

enable_testing()
find_package(GTest REQUIRED)

find_library(PQ_LIB pq REQUIRED)
find_library(PQXX_LIB pqxx REQUIRED)

add_executable(test_orbital_tracking tests.cpp OrbitalTracker.cpp)
target_link_libraries(test_orbital_tracking ${PQ_LIB} ${PQXX_LIB} ${GTEST_BOTH_LIBRARIES})
