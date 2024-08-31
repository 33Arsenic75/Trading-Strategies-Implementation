#ifndef RSI_H
#define RSI_H
#include "../Database/Database.h"
class Rsi
{
public:
    Rsi(const std::string &symb, const std::string &strt, const std::string &end, int n_years, double x, int past_days, double p_value, double up_bound, double low_bound);
    ~Rsi();
    void execute_strategy(const double &x);
    int n_days;
    double max_position;
    Database data;
    std::string symbol;
    std::string start_date;
    std::string end_date;
    int n;
    int current_position;
    double moving_gain;
    double moving_loss;
    double p;
    double upped_bound;
    double lower_bound;
    std::vector<std::string> daily_cashflow;
    std::vector<std::string> order_statics;
    double pnl;
    void generate_result();
};

#endif // RSI_H
