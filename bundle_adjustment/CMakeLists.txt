cmake_minimum_required(VERSION 3.16)

project(learn_ceres)

set(CMAKE_CXX_STANDARD 14)

add_executable(learn_ceres 
        bundle_adjuster.cc
        bal_problem.cc
        )


find_package(Ceres REQUIRED)

#target_link_libraries(learn_ceres Ceres::ceres)
#target_link_libraries(learn_ceres Ceres::ceres gflags)

target_include_directories(learn_ceres
        PRIVATE
        ${CERES_INCLUDE_DIRS}
        )

target_link_libraries(learn_ceres
        PRIVATE
        ${CERES_LIBRARIES}
        )
