#include "improved_dma.h"
#include "../Database/Database.h"
#include <math.h>
#include <queue>
Improved_Dma::Improved_Dma(const std::string &symb, const std::string &strt, const std::string &end, int n_years, double x, int past_days,
                           double percent_change, double c1_para, double c2_para, int max_hold_can_days)
{
    n_days = past_days;
    max_position = x;
    symbol = symb;
    start_date = strt;
    end_date = end;
    n = n_years;
    c1 = c1_para;
    c2 = c2_para;
    max_hold_days = max_hold_can_days;
    p = percent_change;
    data.generate_data(symbol, start_date, end_date, n, "improved_dma");
}

Improved_Dma::~Improved_Dma()
{
}

void Improved_Dma::efficiency_ratio(int index, int n_days)
{
    std::vector<StockData> &d = data.data;
    int i = index - n_days - 1;
    for (; i < index; i++)
    {
        changing_price += (d[i + 1].close - d[i].close);
        abs_changing_price += abs(d[i + 1].close - d[i].close);
    }
}

void Improved_Dma::execute_strategy(double x)
{
    std::vector<StockData> &d = data.data;
    int i = data.start_idx;
    efficiency_ratio(i, n);
    int stocks_inhand = 0;

    double cash_flow = 0;

    // this calculates for the start date
    double er = (changing_price / abs_changing_price);
    double sf = 0.5;
    double ama = d[i].close;
    i++;
    int stocks_held_for_days = 0;

    // account to keep track of the sell && buy
    std::queue<StockData> account_buying;
    std::queue<StockData> account_selling;

    for (; i < d.size(); i++)
    {
        const double current_price = d[i].close - d[i - 1].close;
        const double to_remove = d[i - n].close - d[i - n - 1].close;

        changing_price += (current_price - to_remove);
        abs_changing_price += (abs(current_price) - abs(to_remove));
        if (abs_changing_price != 0)
        {
            er = changing_price / abs_changing_price;
            sf = sf + c1 * (-sf + (((2 * er) / (1 + c2)) - 1) / (((2 * er) / (1 + c2)) + 1));
            ama = ama + sf * (d[i].close - ama);

            if ((d[i].close - ama) >= p && stocks_inhand < x)
            {
                StockData s;
                s.close = i; // dont get confused. i will store "i" here.
                s.date = d[i].date;
                account_buying.push(s);

                if (!account_selling.empty() && stocks_inhand < -1)
                {
                    account_selling.pop();
                }
                current_position++;
                stocks_inhand++;
                cash_flow -= d[i].close;
                order_statics.push_back(d[i].date + ",BUY,1," + std::to_string(d[i].close));
            }
            else if ((d[i].close - ama) <= -p && stocks_inhand > -x)
            {

                StockData s;
                s.close = i; // storing "i" here.
                s.date = d[i].date;
                account_selling.push(s);

                if (!account_buying.empty() && stocks_inhand > 1)
                {
                    account_buying.pop();
                }
                current_position--;
                stocks_inhand--;
                cash_flow += d[i].close;
                order_statics.push_back(d[i].date + ",SELL,1," + std::to_string(d[i].close));
            }

            while (!account_buying.empty())
            {
                if (i - account_buying.front().close >= max_hold_days && stocks_inhand > -x)
                {
                    cash_flow += d[i].close;
                    order_statics.push_back(d[i].date + ",SELL,1," + std::to_string(d[i].close));
                    stocks_inhand--;
                    account_buying.pop();
                }
                else
                {
                    break;
                }
            }

            while (!account_selling.empty())
            {
                if (i - account_selling.front().close >= max_hold_days && stocks_inhand < x)
                {
                    cash_flow -= d[i].close;
                    stocks_inhand++;
                    order_statics.push_back(d[i].date + ",BUY,1," + std::to_string(d[i].close));
                    account_selling.pop();
                }
                else
                {
                    break;
                }
            }
            daily_cashflow.push_back(d[i].date + "," + std::to_string(cash_flow));
        }
    }
    cash_flow += (stocks_inhand * d[d.size() - 1].close);
    pnl = cash_flow;
}

void Improved_Dma::generate_result()
{
    order_stats(order_statics, "order_statistics.csv");
    daily_flow(daily_cashflow, "daily_cashflow.csv");
    final_pnl(pnl, "final_pnl.txt");
}

int main(int argc, char *argv[])
{
    int n_years = (std::stoi(argv[2]) / 365) + 1;
    Improved_Dma Improved_Dma(argv[1], argv[8], argv[9], n_years, std::stod(argv[3]), std::stoi(argv[2]), std::stod(argv[4]), std::stod(argv[6]), std::stod(argv[7]), std::stoi(argv[5]));
    Improved_Dma.execute_strategy(std::stod(argv[3]));
    Improved_Dma.generate_result();
    return 0;
}