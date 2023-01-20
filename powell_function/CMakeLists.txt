cmake_minimum_required(VERSION 3.16)

project(powell)

set(CMAKE_CXX_STANDARD 11)

find_package(Ceres REQUIRED)

add_executable(powell
        powell_function.cpp
)

target_include_directories(powell
        PRIVATE
        ${CERES_INCLUDE_DIRS}
        )

target_link_libraries(powell
        PRIVATE
        ${CERES_LIBRARIES}
        )

