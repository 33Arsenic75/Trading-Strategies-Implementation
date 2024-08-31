#include "Adx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "../Database/Database.h"

Adx::Adx(const std::string &symb, const std::string &strt, const std::string &end, int n_years, double x, int past_days)
{
    n_days = past_days;
    max_position = x;
    symbol = symb;
    start_date = strt;
    end_date = end;
    n = n_years;
    start_idx = 0;
    generate_data();
}

Adx::~Adx()
{
}

void Adx::generate_data()
{
    std::string pythonCommand = "python3 get_stock_data_adx.py " + symbol + " " + start_date + " " + end_date + " " + std::to_string(n) + " " + "adx";
    FILE *pipe = popen(pythonCommand.c_str(), "r");
    if (!pipe)
    {
        std::cerr << "Error executing command!" << std::endl;
    }
    int start_date_index;
    fscanf(pipe, "%d", &start_date_index);
    pclose(pipe);
    start_idx = start_date_index - 1;
    read_data(symbol + "_adx.csv");
    /// std::cerr<<start_idx<<"YO"<<std::endl;
}

void Adx::initialization()
{
    tr = std::max(std::max(high_t[start_idx] - prev_close[start_idx], low_t[start_idx] - prev_close[start_idx]), high_t[start_idx] - low_t[start_idx]);
    dm_p = std::max(0.0, high_t[start_idx] - high_t1[start_idx]);
    dm_m = std::max(0.0, low_t[start_idx] - low_t1[start_idx]);
    atr = 0;
    di_p = 0;
    di_m = 0;
    dx = 0;
    adx = 0;
    // atr = tr;
    // di_p = dm_p / atr;
    // di_m = dm_m / atr;
    // dx = 100 * ( di_p - di_m )/( di_p + di_m );
    // adx = dx;
    /// std::cerr<<tr<<' '<<dm_p<<' '<<dm_m<<' '<<atr<<' '<<di_p<<' '<<di_m<<' '<<dx<<' '<<adx<<'\n';
}

void Adx::execute_strategy(const double &threshold, const double &x)
{
    int i = start_idx;
    initialization();
    int stocks_inhand = 0;
    double cash_flow = 0;
    const double alpha = 2 * static_cast<double>(n_days) / (1 + static_cast<double>(n_days));
    for (; i < high_t.size(); i++)
    {
        /// std::cerr<<date[i]<<" "<<adx<<std::endl;
        if (adx > threshold && stocks_inhand < x)
        {
            stocks_inhand++;
            cash_flow -= close[i];
            order_statics.push_back(date[i] + ",BUY,1," + std::to_string(close[i]));
        }
        else if (adx < threshold && stocks_inhand > -x)
        {
            stocks_inhand--;
            cash_flow += close[i];
            order_statics.push_back(date[i] + ",SELL,1," + std::to_string(close[i]));
        }
        daily_cashflow.push_back(date[i] + "," + std::to_string(cash_flow));
        next_iteration(i + 1, alpha);
    }
    cash_flow += (stocks_inhand * close[close.size() - 1]);
    pnl = cash_flow;
}

/// x_t = alpha ( y - x_t1) + x_t1
double ewm(double &x, double &y, const double &alpha)
{
    if (x == 0)
    {
        return y;
    }
    return alpha * (y - x) + x;
}

void Adx::next_iteration(const int &i, const double &alpha)
{
    tr = std::max(std::max(high_t[i] - prev_close[i], low_t[i] - prev_close[i]), high_t[i] - low_t[i]);
    dm_p = std::max(0.0, high_t[i] - high_t1[i]);
    dm_m = std::max(0.0, low_t[i] - low_t1[i]);
    atr = ewm(atr, tr, alpha);
    double temp = dm_p / atr;
    di_p = ewm(di_p, temp, alpha);
    temp = dm_m / atr;
    di_m = ewm(di_m, temp, alpha);
    if (di_p + di_m != 0)
    {
        dx = (di_p - di_m) / (di_p + di_m);
    }
    adx = ewm(adx, dx, alpha);
}

void Adx::read_data(const std::string &filepath)
{
    std::ifstream infile(filepath);
    if (!infile.is_open())
    {
        std::cerr << "Error opening file!" << std::endl;
    }
    std::string header;
    std::getline(infile, header);
    std::string line;
    while (std::getline(infile, line))
    {
        std::stringstream ss(line);
        std::string dateStr, low_tStr, high_tStr, low_t_1_Str, high_t_1_Str, prev_closeStr, closeStr;
        std::getline(ss, dateStr, ',');
        std::getline(ss, low_tStr, ',');
        std::getline(ss, high_tStr, ',');
        std::getline(ss, low_t_1_Str, ',');
        std::getline(ss, high_t_1_Str, ',');
        std::getline(ss, prev_closeStr, ',');
        std::getline(ss, closeStr, ',');
        date.push_back(dateStr);
        low_t.push_back(std::stod(low_tStr));
        high_t.push_back(std::stod(high_tStr));
        low_t1.push_back(std::stod(low_t_1_Str));
        high_t1.push_back(std::stod(high_t_1_Str));
        prev_close.push_back(std::stod(prev_closeStr));
        close.push_back(std::stod(closeStr));
    }
    infile.close();
}

void Adx::generate_result()
{
    order_stats(order_statics, "order_statistics.csv");
    daily_flow(daily_cashflow, "daily_cashflow.csv");
    final_pnl(pnl, "final_pnl.txt");
}

int main(int argc, char *argv[])
{
    int n_years = (std::stoi(argv[2]) / 365) + 1;
    Adx Adx(argv[1], argv[5], argv[6], n_years, std::stod(argv[3]), std::stoi(argv[2]));
    Adx.execute_strategy(std::stod(argv[4]), std::stod(argv[3]));
    Adx.generate_result();
    return 0;
}