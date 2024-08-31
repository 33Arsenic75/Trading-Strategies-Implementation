#ifndef BASIC_H
#define BASIC_H
#include"../Database/Database.h"
class Basic {
public:
    Basic(const std::string& symb,const std::string& strt,const std::string& end ,int n, double x,int past_days);
    ~Basic();
    void execute_strategy(const double&x);
    int n_days;
    double max_position;
    Database data;
    std::string symbol;
    std::string start_date;
    std::string end_date;
    int n;
    int current_position;
    bool is_increasing(int idx,const std::vector<StockData>&d);
    bool is_decreasing(int idx,const std::vector<StockData>&d);
    std::vector<std::string>daily_cashflow;
    std::vector<std::string>order_statics;
    double pnl;
    void generate_result();
};

#endif  // BASIC_H
