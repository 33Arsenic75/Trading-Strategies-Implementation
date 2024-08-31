#ifndef LINEAR_H
#define LINEAR_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include"Linear_Regression.h"
#include"../Database/Database.h"
class LinearModel {
public:
    LinearModel(const std::string& symb,const std::string& end,const std::string& strt,const std::string&tr_end_date,const std::string&tr_start_date);
    ~LinearModel();
    void execute_strategy(const double&x,const double&p);
///    void set_training_data();
    void read_data(const std::string&filepath);
    void read_prediction_data(const std::string&filepath);
    void generate_training_data();
    void generate_prediction_data();
    LinearRegression model;
    std::string symbol;
    std::string start_date;
    std::string end_date;
    std::string train_start_date;
    std::string train_end_date;
    std::vector<std::vector<double>>training_X;
    std::vector<double>training_Y;
    std::vector<std::string>training_Dates;
    std::vector<std::vector<double>>predict_X;
    std::vector<double>predict_Y;
    std::vector<double>real_Y;
    std::vector<std::string>predict_Dates;
    std::vector<std::string>daily_cashflow;
    std::vector<std::string>order_statics;
    double pnl;
    void generate_result();
};

#endif  // LINEAR_H
