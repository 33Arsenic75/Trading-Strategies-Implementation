#include "mean_reverting_stoploss.h"
#include "../Database/Database.h"
#include <math.h>
#include <queue>

mean_reverting_stoploss::mean_reverting_stoploss(const std::string &symb1, const std::string &symb2, const std::string &strt, const std::string &end, int n_years, double x, int past_days)
{
    n_days = past_days;
    max_position = x;
    symbol1 = symb1;
    symbol2 = symb2;
    start_date = strt;
    end_date = end;
    n = n_years;
    moving_sum = 0;
    moving_sum_of_squares = 0;
    data1.generate_data(symbol1, start_date, end_date, n, "mean_reverting_stoploss");
    data2.generate_data(symbol2, start_date, end_date, n, "mean_reverting_stoploss");
}

mean_reverting_stoploss::~mean_reverting_stoploss()
{
}

void mean_reverting_stoploss::generate_result()
{
    order_stats(order_statics1, "order_statistics_1.csv");
    order_stats(order_statics2, "order_statistics_2.csv");
    daily_flow(daily_cashflow, "daily_cashflow.csv");
    final_pnl(pnl, "final_pnl.txt");
}

void initialization(double &moving_sum, double &moving_sum_squares, const int idx, const std::vector<StockData> &d1, const std::vector<StockData> &d2, const int n_days)
{
    for (int i = idx - n_days; i < idx; i++)
    {
        moving_sum += d1[i].close - d2[i].close;
        moving_sum_squares += (d1[i].close - d2[i].close) * (d1[i].close - d2[i].close);
    }
}

void mean_reverting_stoploss::execute_strategy(const double &x, const double &threshold, const double &stop_loss_threshold)
{
    std::vector<StockData> &d1 = data1.data;
    std::vector<StockData> &d2 = data2.data;
    int i = data1.start_idx;
    initialization(moving_sum, moving_sum_of_squares, i, d1, d2, n_days);
    int stocks_inhand = 0;

    std::priority_queue<double> store_spread_buying;
    std::priority_queue<double> store_spread_selling;

    double cash_flow = 0;

    for (; i < d1.size(); i++)
    {
        const double current_price = d1[i].close - d2[i].close;
        const double to_remove = d1[i - n_days].close - d2[i - n_days].close;
        moving_sum += current_price - to_remove;
        moving_sum_of_squares += current_price * current_price - to_remove * to_remove;
        const double std_deviation = sqrtl(moving_sum_of_squares / n - pow((moving_sum / n), 2));
        const double z_score = (current_price - (moving_sum / n)) / std_deviation;

        int stocks_traded_today = 0;
        if (z_score > threshold && stocks_inhand > -x)
        {
            current_position++;
            stocks_inhand--;
            cash_flow += current_price;
            store_spread_selling.push(current_price);

            stocks_traded_today--;
        }
        else if (z_score < -threshold && stocks_inhand < x)
        {
            current_position--;
            stocks_inhand++;
            cash_flow -= current_price;
            store_spread_buying.push(current_price);

            stocks_traded_today++;
        }

        while (!store_spread_selling.empty())
        {
            double zscore = (store_spread_selling.top() - (moving_sum / n)) / std_deviation;
            if (zscore > stop_loss_threshold && stocks_inhand < x)
            {
                store_spread_selling.pop();
                stocks_traded_today++; // buy kiya
                stocks_inhand++;
            }
            else
            {
                break;
            }
        }

        while (!store_spread_buying.empty())
        {
            double zscore = (store_spread_buying.top() - (moving_sum / n)) / std_deviation;
            if (zscore > stop_loss_threshold && stocks_inhand > -x)
            {
                stocks_traded_today--; // sell kiya
                store_spread_buying.pop();
                stocks_inhand--;
            }
            else
            {
                break;
            }
        }

        if (stocks_traded_today > 0)
        {
            // net buy signal
            order_statics1.push_back(d1[i].date + ",BUY," + std::to_string(abs(stocks_traded_today)) + "," + std::to_string(d1[i].close));
            order_statics2.push_back(d2[i].date + ",SELL," + std::to_string(abs(stocks_traded_today)) + "," + std::to_string(d2[i].close));
        }
        else if (stocks_traded_today < 0)
        {
            // net sell signal
            order_statics1.push_back(d1[i].date + ",SELL," + std::to_string(abs(stocks_traded_today)) + "," + std::to_string(d1[i].close));
            order_statics2.push_back(d2[i].date + ",BUY," + std::to_string(abs(stocks_traded_today)) + "," + std::to_string(d2[i].close));
        }
        daily_cashflow.push_back(d1[i].date + "," + std::to_string(cash_flow));
    }
    cash_flow += (stocks_inhand * (d1[d1.size() - 1].close - d2[d1.size() - 1].close));
    pnl = cash_flow;
}

int main(int argc, char *argv[])
{
    mean_reverting_stoploss mean_reverting_stoploss(argv[1], argv[2], argv[7], argv[8], std::stoi(argv[4]), std::stod(argv[3]), std::stoi(argv[4]));
    mean_reverting_stoploss.execute_strategy(std::stod(argv[3]), std::stod(argv[5]), std::stod(argv[6]));
    mean_reverting_stoploss.generate_result();
    return 0;
}