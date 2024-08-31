#include "Dma.h"
#include "../Database/Database.h"
#include <iomanip>
#include <math.h>
Dma::Dma(const std::string &symb, const std::string &strt, const std::string &end, int n_years, double x, int past_days, double p_value)
{
    n_days = past_days;
    max_position = x;
    symbol = symb;
    start_date = strt;
    end_date = end;
    n = n_years;
    moving_sum = 0;
    moving_sum_of_squares = 0;
    p = p_value;
    data.generate_data(symbol, start_date, end_date, n, "Dma");
}

Dma::~Dma()
{
}

void initialization1(double &moving_sum, double &moving_sum_squares, const int idx, const std::vector<StockData> &d, const int n_days)
{
    for (int i = idx - n_days; i < idx; i++)
    {
        moving_sum += d[i].close;
        moving_sum_squares += (d[i].close) * (d[i].close);
    }
}

void Dma::execute_strategy(const double &x)
{
    std::vector<StockData> &d = data.data;
    int i = data.start_idx;
    initialization1(moving_sum, moving_sum_of_squares, i, d, n_days);
    int stocks_inhand = 0;
    double n_value = n_days;

    double cash_flow = 0;
    for (; i < d.size(); i++)
    {
        double current_price = d[i].close;
        double to_remove = d[i - n_days].close;
        moving_sum += current_price - to_remove;
        moving_sum_of_squares += (current_price * current_price) - (to_remove * to_remove);
        double squares_mean = moving_sum_of_squares / n_value;
        double mean_square = pow((moving_sum / n_value), 2);
        double std_deviation = sqrtl(squares_mean - mean_square);
        if (d[i].close >= (moving_sum / n_value) + p * (std_deviation) && stocks_inhand < x)
        {
            current_position++;
            stocks_inhand++;
            cash_flow -= d[i].close;
            order_statics.push_back(d[i].date + ",BUY,1," + std::to_string(d[i].close));
        }
        else if (d[i].close <= (moving_sum / n_value) - p * (std_deviation) && stocks_inhand > -x)
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
    std::cout << "Generated." << std::endl;
}

void Dma::generate_result()
{
    order_stats(order_statics, "order_statistics.csv");
    daily_flow(daily_cashflow, "daily_cashflow.csv");
    final_pnl(pnl, "final_pnl.txt");
}

// int main(int argc, char *argv[])
// {
//     Dma Dma(argv[1], argv[5], argv[6], std::stoi(argv[2]), std::stod(argv[3]), std::stoi(argv[2]), std::stod(argv[4]));
//     Dma.execute_strategy(std::stod(argv[3]));
//     Dma.generate_result();
//     return 0;
// }
