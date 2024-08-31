#include "Rsi.h"
#include "../Database/Database.h"
Rsi::Rsi(const std::string &symb, const std::string &strt, const std::string &end, int n_years, double x, int past_days, double p_value, double up_bound, double low_bound)
{
    n_days = past_days;
    max_position = x;
    symbol = symb;
    start_date = strt;
    end_date = end;
    n = n_years;
    moving_gain = 0;
    moving_loss = 0;
    p = p_value;
    upped_bound = up_bound;
    lower_bound = low_bound;
    data.generate_data(symbol, start_date, end_date, n, "rsi");
}

Rsi::~Rsi()
{
}

void initialization(double &moving_gain, double &moving_loss, const int idx, const std::vector<StockData> &d, const int n_days)
{
    for (int i = idx - n_days; i < idx; i++)
    {
        /// std::cerr<<d[i].date<<' ';
        if (d[i].close > d[i - 1].close)
        {
            moving_gain += d[i].close - d[i - 1].close;
        }
        else
        {
            moving_loss += d[i - 1].close - d[i].close;
        }
    }
    /// std::cerr<<'\n';
}

void Rsi::execute_strategy(const double &x)
{
    std::vector<StockData> &d = data.data;
    int i = data.start_idx;
    initialization(moving_gain, moving_loss, i, d, n_days);
    int stocks_inhand = 0;
    /// std::cerr<<upped_bound<<' '<<lower_bound<<std::endl;
    double cash_flow = 0;
    for (; i < d.size(); i++)
    {
        const double current_change = d[i].close - d[i - 1].close;
        const double to_remove_change = d[i - n_days].close - d[i - n_days - 1].close;
        if (current_change > 0)
        {
            moving_gain += current_change;
        }
        else
        {
            moving_loss -= current_change;
        }
        if (to_remove_change > 0)
        {
            moving_gain -= to_remove_change;
        }
        else
        {
            moving_loss += to_remove_change;
        }
        const double rsi = 100 * (moving_gain) / (moving_gain + moving_loss);
        /// std::cerr<<d[i].date<<" "<<rsi<<' '<<moving_gain<<' '<<moving_loss<<std::endl;
        if (rsi < lower_bound && stocks_inhand < x)
        {
            stocks_inhand++;
            cash_flow -= d[i].close;
            order_statics.push_back(d[i].date + ",BUY,1," + std::to_string(d[i].close));
        }
        else if (rsi > upped_bound && stocks_inhand > -x)
        {
            stocks_inhand--;
            cash_flow += d[i].close;
            order_statics.push_back(d[i].date + ",SELL,1," + std::to_string(d[i].close));
        }
        daily_cashflow.push_back(d[i].date + "," + std::to_string(cash_flow));
    }
    cash_flow += (stocks_inhand * d[d.size() - 1].close);
    pnl = cash_flow;
}

void Rsi::generate_result()
{
    order_stats(order_statics, "order_statistics.csv");
    daily_flow(daily_cashflow, "daily_cashflow.csv");
    final_pnl(pnl, "final_pnl.txt");
}

int main(int argc, char *argv[])
{
    int n_years = (std::stoi(argv[2]) / 365) + 1;
    Rsi Rsi(argv[1], argv[6], argv[7], n_years, std::stoi(argv[3]), std::stod(argv[2]), 0.0, std::stod(argv[5]), std::stod(argv[4]));
    Rsi.execute_strategy(std::stod(argv[3]));
    Rsi.generate_result();
    return 0;
}
