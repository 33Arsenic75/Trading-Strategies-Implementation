#ifndef DMA_H
#define DMA_H
#include"../Database/Database.h"
class Dma {
public:
    Dma(const std::string& symb,const std::string& strt,const std::string& end ,int n_years, double x,int past_days,double p_value);
    ~Dma();
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
    double moving_sum;
    double moving_sum_of_squares;
    double p;
    std::vector<std::string>daily_cashflow;
    std::vector<std::string>order_statics;
    double pnl;
    void generate_result();
};

#endif  // DMA_H
