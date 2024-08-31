#include "macd.h"
#include "../Database/Database.h"
#include <algorithm>
#include <cstdio>
#include <fstream>

Macd::Macd(const std::string &symb, const std::string &strt, const std::string &end, double x)
{
    max_position = x;
    symbol = symb;
    start_date = strt;
    end_date = end;
    data.generate_data(symbol, start_date, end_date, 0, "macd");
}

Macd::~Macd()
{
}

void Macd::generate_result()
{
    order_stats(order_statics, "order_statistics.csv");
    daily_flow(daily_cashflow, "daily_cashflow.csv");
    final_pnl(pnl, "final_pnl.txt");
}

// std::vector<double> Macd::short_ewm(int index)
// {
//     std::vector<StockData> &d = data.data;
//     std::vector<double> shortevms(d.size());
//     for (; index < d.size(); index++)
//     {
//         int start_index = index - 12;
//         double ewm = d[start_index].close;
//         for (; start_index < index + 1; start_index++)
//         {
//             ewm = (2.0 / 13) * (d[start_index].close - ewm) + ewm;
//         }
//         shortevms[index] = ewm;
//     }
//     return shortevms;
// }

// std::vector<double> Macd::long_ewm(int index)
// {
//     std::vector<StockData> &d = data.data;
//     std::vector<double> longevms(d.size());
//     for (; index < d.size(); index++)
//     {
//         int start_index = index - 26;
//         double ewm = d[start_index].close;
//         for (; start_index < index + 1; start_index++)
//         {
//             ewm = (2.0 / 13) * (d[start_index].close - ewm) + ewm;
//         }
//         longevms[index] = ewm;
//     }
//     return longevms;
// }

// std::vector<double> Macd::macd_arr(std::vector<double> &s, std::vector<double> &l, int actual_start)
// {
//     std::vector<StockData> &d = data.data;
//     std::vector<double> macd(d.size());
//     for (; actual_start < d.size(); actual_start++)
//     {
//         macd[actual_start] = (s[actual_start] - l[actual_start]);
//     }
//     return macd;
// }

// std::vector<double> Macd::signal(int index, std::vector<double> &macd_arr)
// {
//     std::vector<StockData> &d = data.data;
//     std::vector<double> signal_arr(d.size());
//     for (; index < d.size(); index++)
//     {
//         int start_index = index - 9;
//         double ewm = macd_arr[start_index];
//         for (; start_index < index + 1; start_index++)
//         {
//             ewm = (2.0 / 13.0) * (macd_arr[start_index] - ewm) + ewm;
//         }
//         signal_arr[index] = ewm;
//     }
//     return signal_arr;
//}

void Macd::execute_strategy(double x)
{
    std::vector<StockData> &d = data.data;
    int i = data.start_idx;
    int actual_start = i - 9;
    int stocks_inhand = 0;
    double cash_flow = 0;

    // calculated for start date
    double shortewm = d[i].close;
    double longewm = d[i].close;
    double macd = shortewm - longewm;
    double signal = 0;
    daily_cashflow.push_back(d[i].date + "," + std::to_string(cash_flow));
    i++;
    for (; i < d.size(); i++)
    {
        shortewm = (2.0 / 13.0) * (d[i].close - shortewm) + shortewm;
        longewm = (2.0 / 27.0) * (d[i].close - longewm) + longewm;
        macd = shortewm - longewm;
        signal = (2.0 / 10.0) * (macd - signal) + signal;
        if (macd > signal && stocks_inhand < x)
        {
            current_position++;
            stocks_inhand++;
            cash_flow -= d[i].close;
            order_statics.push_back(d[i].date + ",BUY,1," + std::to_string(d[i].close));
        }
        else if (macd < signal && stocks_inhand > -x)
        {
            current_position--;
            stocks_inhand--;
            cash_flow += d[i].close;
            order_statics.push_back(d[i].date + ",SELL,1," + std::to_string(d[i].close));
        }
        daily_cashflow.push_back(d[i].date + "," + std::to_string(cash_flow));
    }
    cash_flow += (stocks_inhand * d[d.size() - 1].close);
    pnl = cash_flow;
}

int main(int argc, char *argv[])
{
    Macd Macd(argv[1], argv[3], argv[4], std::stod(argv[2]));
    Macd.execute_strategy(std::stod(argv[2]));
    Macd.generate_result();
    return 0;
}