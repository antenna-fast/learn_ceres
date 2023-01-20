// solve powell function

#include <iostream>
#include <vector>

#include "ceres/ceres.h"
#include <gflags/gflags.h>
#include "glog/logging.h"

using ceres::AutoDiffCostFunction;
using ceres::CostFunction;
using ceres::Problem;
using ceres::Solve;
using ceres::Solver;


// 1. 定义cost functors
// 多个f的情况:
// 每个式子写一个functor

// const T*: you can NOT modify ptr->value, but can modify ptr itself
// T* const: 指针所指向的位置不能改变，但是，指针所指向的内容可以改变
// const T* const x：地址不能变，内容不能变: // you can neither modify the pointer, nor the thing it points to.

struct F1{  // 第1个等式
    template<typename T>
    bool operator()(const T* const x1, const T* const x2, T* residual)const{
        residual[0] = x1[0] + 10.0 * x2[0];
        return true;
    }
};

struct F2{  // 第2个等式
    template<typename T>
    bool operator()(const T* const x3, const T* const x4, T* residual)const{
        residual[0] = sqrt(5.0) * (x3[0] - x4[0]);
        return true;
    }
};

struct F3{  // 第3个等式
    template<typename T>
    bool operator()(const T* const x2, const T* const x3, T* residual)const{
        residual[0] = (x2[0] - 2.0 * x3[0]) * (x2[0] - 2.0 * x3[0]);
        return true;
    }
};

struct F4{  // 第4个等式
    template<typename T>
    bool operator()(const T* const x1, const T* const x4, T* residual) const{
        residual[0] = sqrt(10.0) * (x1[0] - x4[0]) * (x1[0] - x4[0]);
        return true;
    }
};


int main(int argc, char ** argv) {
    std::cout << "Powell Function Solver" << std::endl;

    GFLAGS_NAMESPACE::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging(argv[0]);

    // Powell function
    double x1 = 3.0;
    double x2 = -1.0;
    double x3 = 0.0;
    double x4 = 1.0;

    // 第二步:构建问题
    Problem problem;

    // Add residual terms to the problem using the using the autodiff
    // wrapper to get the derivatives automatically.
    // <CostFunction, out_dim, input_dim>(new CostFunctor), LossFunction, &var1, &var2 ...
    problem.AddResidualBlock(new AutoDiffCostFunction<F1, 1, 1, 1>(new F1), nullptr, &x1, &x2);
    problem.AddResidualBlock(new AutoDiffCostFunction<F2, 1, 1, 1>(new F2), nullptr, &x3, &x4);
    problem.AddResidualBlock(new AutoDiffCostFunction<F3, 1, 1, 1>(new F3), nullptr, &x2, &x3);
    problem.AddResidualBlock(new AutoDiffCostFunction<F4, 1, 1, 1>(new F4), nullptr, &x1, &x4);

    // 第三步：设定参数
    Solver::Options options; 
    options.max_num_iterations = 100;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_progress_to_stdout = true;

    std::cout << "Initial x1 = " << x1 << ", x2 = " << x2 << ", x3 = " << x3 << ", x4 = " << x4 << "\n";

    // 第四步：求解
    Solver::Summary summary;
    Solve(options, &problem, &summary);

    std::cout << summary.FullReport() << "\n";
    std::cout << "Final x1 = " << x1 << ", x2 = " << x2 << ", x3 = " << x3 << ", x4 = " << x4 << "\n";

    return 0;
}
