#ifndef IMPROVED_DMA_H
#define IMPROVED_DMA_H
#include"../Database/Database.h"
class Improved_Dma
{
public:
    Improved_Dma(const std::string &symb, const std::string &strt, const std::string &end, int n_years, double x, int past_days, double percent_change, double c1, double c2, int max_hold_days);
    ~Improved_Dma();
    void execute_strategy(double x);
    int n_days;
    double max_position;
    Database data;
    std::string symbol;
    std::string start_date;
    std::string end_date;
    int n;
    int current_position;
    void efficiency_ratio(int index, int n_days);
    double changing_price;
    double abs_changing_price;
    int max_hold_days;
    double c1;
    double c2;
    double p;
    std::vector<std::string>daily_cashflow;
    std::vector<std::string>order_statics;
    double pnl;
    void generate_result();
};

#endif // DMA_H