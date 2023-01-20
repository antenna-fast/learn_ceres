cmake_minimum_required(VERSION 3.16)

project(curve_fitting)

set(CMAKE_CXX_STANDARD 11)

find_package(Ceres REQUIRED)

add_executable(curve_fitting
        # powell_function.cpp
        curve_fitting.cpp
        )

target_include_directories(curve_fitting
        PRIVATE
        ${CERES_INCLUDE_DIRS}
        )

target_link_libraries(curve_fitting
        PRIVATE
        ${CERES_LIBRARIES}
        )

