// LinearRegression.h

#ifndef LINEARREGRESSION_H
#define LINEARREGRESSION_H

#include <vector>

class LinearRegression {
private:
    std::vector<std::vector<double>> X; 
    std::vector<double> y;              
    std::vector<double> coefficients;  
    double intercept;                  
public:
    LinearRegression(const std::vector<std::vector<double>>& input, const std::vector<double>& output);
    LinearRegression();
    void set_data(const std::vector<std::vector<double>>& input, const std::vector<double>& output);
    void train();
    double predict(const std::vector<double>& input);
    std::vector<double> predict(const std::vector<std::vector<double>>& inputs);
    std::vector<double> getCoefficients() const;
    double getIntercept() const;
};
#endif
