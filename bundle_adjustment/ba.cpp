//
// Created by yaohua on 2021/5/29.
//

#include <iostream>
#include <algorithm>
#include <cmath>
#include <string>
#include <vector>

//#include <bal_problem.h>
#include "ceres/ceres.h"
#include <gflags/gflags.h>
#include "glog/logging.h"
#include "../snavely_reprojection_error.h"


using ceres::AutoDiffCostFunction;
using ceres::CostFunction;
using ceres::CauchyLoss;
using ceres::Problem;
using ceres::Solve;
using ceres::Solver;

using std::cout;
using std::endl;


struct SnavelyReprojectionError{
    // 构造函数
    SnavelyReprojectionError(double obsv_x, double obsv_y):
    observed_x(obsv_x), observed_y(obsv_y){}

    // cost function
    template<typename T>
    bool operator()(const T* const camera, const T* const points, T* residuals)
    const {
        // camera
        T p[3];
        ceres::AngleAxisRotatePoint();
        return true;
    }

    double observed_x;
    double observed_y;
};


int main(){
    cout << "BA ... " << endl;
    
    // Problem

    // Options
    ceres::Solver::Options options;
    options.linear_solver_type = ceres::DENSE_SCHUR;
    options.minimizer_progress_to_stdout = true;
    ceres::Solver::Summary summary;
    ceres::Solve(options, &problem, &summary);
    std::cout << summary.FullReport() << "\n";

    // Solve
    
    return 0;
}
