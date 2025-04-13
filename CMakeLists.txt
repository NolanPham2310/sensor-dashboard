cmake_minimum_required(VERSION 3.10)
project(sensor_dashboard)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM gtkmm-3.0 REQUIRED)

add_executable(sensor_dashboard sensor_dashboard.cpp)
target_include_directories(sensor_dashboard PRIVATE ${GTKMM_INCLUDE_DIRS})
target_link_libraries(sensor_dashboard ${GTKMM_LIBRARIES})
