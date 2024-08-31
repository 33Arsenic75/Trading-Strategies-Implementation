#ifndef MACD_H
#define MACD_H
#include"../Database/Database.h"

class Macd
{
public:
    Macd(const std::string &symb, const std::string &strt, const std::string &end, double x);
    ~Macd();
    void execute_strategy(double x);
    double max_position;
    Database data;
    std::string symbol;
    std::string start_date;
    std::string end_date;
    int current_position;
    std::vector<double> short_ewm(int index);
    std::vector<double> long_ewm(int index);
    std::vector<double> macd_arr(std::vector<double> &s, std::vector<double> &l, int actual_start);
    std::vector<double> signal(int index, std::vector<double> &macd);
    std::vector<std::string>daily_cashflow;
    std::vector<std::string>order_statics;
    double pnl;
    void generate_result();
};

#endif // Macd_H