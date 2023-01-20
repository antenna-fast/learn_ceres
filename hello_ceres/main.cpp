// Hello Ceres!
// minimum y = (10 - x)^x using auto derivative

#include <iostream>
#include "glog/logging.h"

#include "ceres/ceres.h"

using ceres::AutoDiffCostFunction;
using ceres::CostFunction;
using ceres::CauchyLoss;
using ceres::Problem;
using ceres::Solve;
using ceres::Solver;


/*
 * 1. 构建代价函数
 * operator() is a template method
 * which assumes that all its inputs and outputs are of some type T
 * The first step is to write a functor that will evaluate this the function f(x)=10-x
 
 * 对操作符()进行重载
 * 这样的话, 该结构体的一个实例就能具有类似一个函数的性质，在代码编写过程中就能当做一个函数一样来使用
 * 它能利用对象的成员变量来储存更多的函数内部参数
 * */
struct CostFunctor {
    template <typename T>  // 类模板声明: typename
    // 返回值类型   operator   重载()操作符   (参数表：待优化变量x, 残差的指针)
    // 计算残差，也即代价函数的输出
    // const：只读函数，该成员函数不修改成员变量的值
    // 重载了()符号之后，CostFunction就可以传入AutoDiffCostFunction方法来构建寻优问题了
    bool operator()(const T* const x, T* residual) const {
        residual[0] = 10.0 - x[0];
        return true;  
    }
};


int main(int argc, char ** argv) {
    std::cout << "Hello, Ceres!" << std::endl;
    // google log
    google::InitGoogleLogging(argv[0]);

    // The variable to solve with its initial value.
    double initial_x = 50.0;
    double x = initial_x;

    // Set up the only cost function (also known as residual).
    // This uses auto-differentiation to obtain the derivative (jacobian).
    // 自动微分
    // 第一个1：输出维度(即，残差维度)
    // 第二个1：输入维度(待寻优参数x的维度)
    CostFunction* cost_function = new AutoDiffCostFunction<CostFunctor, 1, 1>(new CostFunctor);

    // Build the problem.
    Problem problem;

    // 向问题添加误差项
    // 1. NULL表示不使用损失函数(Loss Function)，&x表示待寻优参数
    problem.AddResidualBlock(cost_function, nullptr, &x);
    // 2. 如果使用LossFunction(eg: CauchyLoss)
    // problem.AddResidualBlock(cost_function, new CauchyLoss(0.5), &x);

    // set options
    Solver::Options options;  
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_progress_to_stdout = true;
    Solver::Summary summary;  // 创建一个summary 用于log

    // Run the solver!
    Solve(options, &problem, &summary);

    std::cout << summary.BriefReport() << "\n";
    std::cout << "x : " << initial_x << " -> " << x << "\n";

    return 0;
}
