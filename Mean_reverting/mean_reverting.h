#ifndef mean_reverting_H
#define mean_reverting_H
#include"../Database/Database.h"
class mean_reverting
{
public:
    mean_reverting(const std::string &symb1, const std::string &symb2, const std::string &strt, const std::string &end, int n_years, double x, int past_days);
    ~mean_reverting();
    void execute_strategy(const double &x, const double &threshold);
    int n_days;
    double max_position;
    Database data1;
    Database data2;
    std::string symbol1;
    std::string symbol2;
    std::string start_date;
    std::string end_date;
    int n;
    int current_position;
    double moving_sum;
    double moving_sum_of_squares;
    std::vector<std::string>daily_cashflow;
    std::vector<std::string>order_statics1;
    std::vector<std::string>order_statics2;
    double pnl;
    void generate_result();
};

#endif // mean_reverting_H