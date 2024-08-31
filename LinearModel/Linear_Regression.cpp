#include <iostream>
#include <vector>
#include"Linear_Regression.h"

LinearRegression::LinearRegression(const std::vector<std::vector<double>>& input, const std::vector<double>& output)
    : X(input), y(output), intercept(0.0) {
    size_t num_features = X[0].size();
    coefficients.resize(num_features, 0.0);
}
LinearRegression::LinearRegression(){
}

void LinearRegression::set_data(const std::vector<std::vector<double>>& input, const std::vector<double>& output){
    X=input;
    y=output;
    intercept=0.0;
    size_t num_features = X[0].size();
    coefficients.resize(num_features, 0.0);
}

void LinearRegression::train() {
    size_t num_samples = X.size();
    size_t num_features = X[0].size();
    std::vector<std::vector<double>> X_transpose(num_features, std::vector<double>(num_samples, 0.0));
    std::vector<std::vector<double>> X_transpose_X(num_features, std::vector<double>(num_features, 0.0));
    std::vector<double> X_transpose_y(num_features, 0.0);
    coefficients.resize(num_features, 0.0);
    std::vector<double> means(num_features, 0.0);
    for (size_t j = 0; j < num_features; ++j) {
        for (size_t i = 0; i < num_samples; ++i) {
            means[j] += X[i][j];
        }
        means[j] /= num_samples;
    }
    for (size_t i = 0; i < num_samples; ++i) {
        for (size_t j = 0; j < num_features; ++j) {
            X_transpose[j][i] = X[i][j];
        }
    }
    for (size_t i = 0; i < num_features; ++i) {
        for (size_t j = 0; j < num_samples; ++j) {
            for (size_t k = 0; k < num_features; ++k) {
                X_transpose_X[i][k] += X_transpose[i][j] * X[j][k];
            }
            X_transpose_y[i] += X_transpose[i][j] * y[j];
        }
    }
    for (size_t i = 0; i < num_features; ++i) {
        double pivot = X_transpose_X[i][i];
        if(pivot==0)continue;
        for (size_t k = 0; k < num_features; ++k) {
            X_transpose_X[i][k] /= pivot;
        }
        X_transpose_y[i] /= pivot;
        for (size_t j = i + 1; j < num_features; ++j) {
            double ratio = X_transpose_X[j][i];
            for (size_t k = 0; k < num_features; ++k) {
                X_transpose_X[j][k] -= ratio * X_transpose_X[i][k];
            }
            X_transpose_y[j] -= ratio * X_transpose_y[i];
        }
    }
    for (int i = num_features - 1; i >= 0; --i) {
        coefficients[i] = X_transpose_y[i];
        for (size_t j = i + 1; j < num_features; ++j) {
            coefficients[i] -= X_transpose_X[i][j] * coefficients[j];
        }
    }
    intercept = 0.0;
    for (size_t j = 0; j < num_features; ++j) {
        intercept -= coefficients[j] * means[j];
    }
}

double LinearRegression::predict(const std::vector<double>& input) {
    double prediction = intercept;
    size_t num_features = coefficients.size();
    for (size_t j = 0; j < num_features; ++j) {
        prediction += coefficients[j] * input[j];
    }
    return prediction;
}

std::vector<double> LinearRegression::predict(const std::vector<std::vector<double>>&input){
    size_t input_size = input.size();
    std::vector<double>result(input_size);
    for(size_t j=0; j < input_size; ++j){
        result[j]=predict(input[j]);
    }
    return result;
}

std::vector<double> LinearRegression::getCoefficients() const {
    return coefficients;
}

double LinearRegression::getIntercept() const {
    return intercept;
}

// int main() {
//     std::vector<std::vector<double>> input = {
//         {1.0, 2.0, 3.0},
//         {2.0, 9.0, 7.0},
//     };

//     std::vector<double> output = {5.0, 6.0 /* Add more output values as needed */};

//     LinearRegression model(input, output);

//     model.train();

//     std::cout << "Coefficients:\n";
//     for (double coefficient : model.getCoefficients()) {
//         std::cout << coefficient << " ";
//     }
//     std::cout << std::endl;

//     std::cout << "Intercept: " << model.getIntercept() << std::endl;

//     std::vector<double> newInput = {4.0, 5.0, 100.0};
//     double prediction = model.predict(newInput);

//     std::cout << "Prediction: " << prediction << std::endl;

//     return 0;
// }
