cmake_minimum_required(VERSION 3.16)

project(learn_ceres)

set(CMAKE_CXX_STANDARD 11)

find_package(Ceres REQUIRED)

target_include_directories(learn_ceres
        PRIVATE
        ${CERES_INCLUDE_DIRS}
        )

add_executable(learn_ceres
        main.cpp
        )

target_link_libraries(learn_ceres
        PRIVATE
        ${CERES_LIBRARIES}
        )
