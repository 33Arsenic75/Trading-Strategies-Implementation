#include "Basic.h"
#include "../Database/Database.h"
#include <algorithm>
Basic::Basic(const std::string &symb, const std::string &strt, const std::string &end, int n_years, double x, int past_days)
{
    n_days = past_days;
    max_position = x;
    symbol = symb;
    start_date = strt;
    end_date = end;
    n = n_years;
    data.generate_data(symbol, start_date, end_date, n, "basic");
}

Basic::~Basic()
{
}

bool Basic::is_increasing(int idx, const std::vector<StockData> &d)
{
    if (idx < n_days)
    {
        return false;
    }
    int index = idx - n_days + 1;
    bool def = true;
    for (; index < idx + 1; index++)
    {
        if (d[index].close <= d[index - 1].close)
        {
            def = false;
            break;
        }
    }
    return def;
}

bool Basic::is_decreasing(int idx, const std::vector<StockData> &d)
{
    if (idx < n_days)
    {
        return false;
    }
    int index = idx - n_days + 1;
    bool def = true;
    for (; index < idx + 1; index++)
    {
        if (d[index].close >= d[index - 1].close)
        {
            def = false;
            break;
        }
    }
    return def;
}

void Basic::execute_strategy(const double &x)
{
    std::vector<StockData> &d = data.data;
    int i = data.start_idx;
    int stocks_inhand = 0;

    double cash_flow = 0;

    for (; i < d.size(); i++)
    {
        if (is_increasing(i, d) && stocks_inhand < x)
        {
            stocks_inhand++;
            cash_flow -= d[i].close;
            order_statics.push_back(d[i].date + ",BUY,1," + std::to_string(d[i].close));
        }
        else if (is_decreasing(i, d) && stocks_inhand > -x)
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

void Basic::generate_result()
{
    order_stats(order_statics, "order_statistics.csv");
    daily_flow(daily_cashflow, "daily_cashflow.csv");
    final_pnl(pnl, "final_pnl.txt");
}

int main(int argc, char *argv[])
{
    int n_years = (std::stoi(argv[2]) / 365) + 1;
    Basic basic(argv[1], argv[4], argv[5], n_years, std::stod(argv[3]), std::stoi(argv[2]));
    basic.execute_strategy(std::stod(argv[3]));
    basic.generate_result();
    return 0;
}
