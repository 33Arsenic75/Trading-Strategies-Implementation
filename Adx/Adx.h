#ifndef Adx_H
#define Adx_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

class Adx {
public:
    Adx(const std::string& symb,const std::string& strt,const std::string& end ,int n, double x,int past_days);
    ~Adx();
    void execute_strategy(const double&threshold, const double&x);
    int n_days;
    double max_position;
    std::string symbol;
    std::string start_date;
    std::string end_date;
    int n;
    int start_idx;
    void generate_data();
    void read_data(const std::string& filepath);
    std::vector<double>low_t;
    std::vector<double>high_t;
    std::vector<double>low_t1;
    std::vector<double>high_t1;
    std::vector<double>prev_close;
    std::vector<double>close;
    std::vector<std::string>date;
    double tr,dm_p,dm_m,atr,di_p,di_m,dx,adx;
    std::vector<std::string>daily_cashflow;
    std::vector<std::string>order_statics;
    double pnl;
    void generate_result();
    void initialization();
    void next_iteration(const int&idx,const double&alpha);
};

#endif  // Adx_H