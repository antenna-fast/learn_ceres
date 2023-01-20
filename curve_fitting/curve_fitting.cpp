// solve the curve fitting problem
// y = e^{mx + c}
// observations: [[x, y], ...] 
// optimize parameters: [m, n] 
// we want to recover the value of m and c from observation data

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "ceres/ceres.h"
#include <gflags/gflags.h>
#include "glog/logging.h"

using namespace std;

using ceres::AutoDiffCostFunction;
using ceres::CostFunction;
using ceres::CauchyLoss;
using ceres::Problem;
using ceres::Solve;
using ceres::Solver;


// Step1: define the cost functor
struct ExponentionalResidual
{
    // struct constructor
    // parameters: observed data
    ExponentionalResidual(double x, double y)
        : x_(x), y_(y) {}

    // calculate residual 
    // parameters: 
    //      variable need to be optimized
    //      residual
    template <typename T>
    bool operator()(const T* const m, const T* const c, T* residual) const {
        residual[0] = y_ - exp(m[0] * x_ + c[0]);  // residual = y - e^{mx + c}
        return true;
    }

// Observation of a sample
private:
    const double x_;
    const double y_;
};


template <typename T>
bool read_txt(std::string file_path, vector<vector<T>>& data, size_t num_per_line)
{
    ifstream in_file;
    in_file.open(file_path);

    if(in_file.fail())
    {
        cout << "failed to open file: " << file_path << endl;
        return false;
    }

    string line;
    while (!in_file.eof())
    {
        getline(in_file, line);
        if(line.empty())
        {
            break;
        }
        // cout << "line: " << line << " : ";
        istringstream str_line(line);
        string str;

        vector<float> vLineData(num_per_line);

        for (size_t i = 0; i < num_per_line; i++)
        {
            getline(str_line, str, ' ');
            // cout << " " << str << " ";
            vLineData.at(i) = std::stof(str);
        }
        // cout << vLineData.at(0) << " " << vLineData.at(1) << endl;
        data.emplace_back(vLineData); 
    }
    return true;
}


int main(){

    // Step1: Init parameter
    double m = 0;
    double c = 0;

    // read data
    string data_path = "data.txt";
    vector<vector<float>> data;

    if(!read_txt(data_path, data, 2))
    {
        return 0;
    }

    size_t len_data = data.size();
    cout << "data len: " << len_data << endl;

    // Step2: Define the problem, feed the data
    Problem problem;

    for (int i = 0; i<len_data; i++) 
    {
        // 使用结构体构造函数传入observation data (x, y), 构造cost function
        CostFunction *cost_function = new AutoDiffCostFunction<ExponentionalResidual, 1, 1, 1>
            (new ExponentionalResidual(data.at(i).at(0), data.at(i).at(1)));

        // problem传入待优化变量，这里是operator parameter list
        // problem.AddResidualBlock(cost_function, nullptr, &m, &c);  // without loss function
        problem.AddResidualBlock(cost_function, new CauchyLoss(0.5), &m, &c);     // with robust loss function: CauchyLoss
    }

    // Step3: Option
    ceres::Solver::Options options;
    options.linear_solver_type = ceres::DENSE_SCHUR;
    options.minimizer_progress_to_stdout = true;

    // Step4: Solve the problem
    ceres::Solver::Summary summary;
    Solve(options, &problem, &summary);

    std::cout << summary.BriefReport() << "\n";
    std::cout << "Final result: m = " << m << ", c = " << c << "\n";

    return 0;
}
