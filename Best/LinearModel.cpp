#include "LinearModel.h"
#include "../Database/Database.h"
#include "Linear_Regression.h"

LinearModel::LinearModel(const std::string &symb, const std::string &end, const std::string &strt, const std::string &tr_end_date, const std::string &tr_start_date)
{
    symbol = symb;
    start_date = strt;
    end_date = end;
    train_end_date = tr_end_date;
    train_start_date = tr_start_date;
    training_X = std::vector<std::vector<double>>();
    training_Y = std::vector<double>();
    predict_X = std::vector<std::vector<double>>();
    predict_Y = std::vector<double>();
    real_Y = std::vector<double>();
    training_Dates = std::vector<std::string>();
    predict_Dates = std::vector<std::string>();
    generate_training_data();
}

void LinearModel::read_data(const std::string &filepath)
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
        std::string dateStr, close_t, closeStr, openStr, vwapStr, lowStr, highStr, no_of_tradeStr, open_t;
        std::getline(ss, dateStr, ',');
        std::getline(ss, close_t, ',');
        std::getline(ss, closeStr, ',');
        std::getline(ss, openStr, ',');
        std::getline(ss, vwapStr, ',');
        std::getline(ss, lowStr, ',');
        std::getline(ss, highStr, ',');
        std::getline(ss, no_of_tradeStr, ',');
        std::getline(ss, open_t, ',');
        std::vector<double> temp_X;
        training_Dates.push_back(dateStr);
        training_Y.push_back(std::stod(close_t));
        temp_X.push_back(std::stod(closeStr));
        temp_X.push_back(std::stod(openStr));
        temp_X.push_back(std::stod(vwapStr));
        temp_X.push_back(std::stod(lowStr));
        temp_X.push_back(std::stod(highStr));
        temp_X.push_back(std::stod(no_of_tradeStr));
        temp_X.push_back(std::stod(open_t));
        training_X.push_back(temp_X);
    }
    infile.close();
    model.set_data(training_X, training_Y);
    model.train();
    generate_prediction_data();
}

void LinearModel::read_prediction_data(const std::string &filepath)
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
        std::string dateStr, close_t, closeStr, openStr, vwapStr, lowStr, highStr, no_of_tradeStr, open_t;
        std::getline(ss, dateStr, ',');
        std::getline(ss, close_t, ',');
        std::getline(ss, closeStr, ',');
        std::getline(ss, openStr, ',');
        std::getline(ss, vwapStr, ',');
        std::getline(ss, lowStr, ',');
        std::getline(ss, highStr, ',');
        std::getline(ss, no_of_tradeStr, ',');
        std::getline(ss, open_t, ',');
        std::vector<double> temp_X;
        predict_Dates.push_back(dateStr);
        real_Y.push_back(std::stod(close_t));
        temp_X.push_back(std::stod(closeStr));
        temp_X.push_back(std::stod(openStr));
        temp_X.push_back(std::stod(vwapStr));
        temp_X.push_back(std::stod(lowStr));
        temp_X.push_back(std::stod(highStr));
        temp_X.push_back(std::stod(no_of_tradeStr));
        temp_X.push_back(std::stod(open_t));
        predict_X.push_back(temp_X);
    }
    infile.close();
    predict_Y = model.predict(predict_X);
}

void LinearModel::generate_training_data()
{
    std::string pythonCommand = "python3 get_stock_data_lr.py " + symbol + " " + train_start_date + " " + train_end_date + " " + "linearmodel";
    FILE *pipe = popen(pythonCommand.c_str(), "r");
    if (!pipe)
    {
        std::cerr << "Error executing command!" << std::endl;
    }
    pclose(pipe);
    read_data(symbol + "_linearmodel.csv");
}

void LinearModel::generate_prediction_data()
{
    std::string pythonCommand = "python3 get_stock_data_lr.py " + symbol + " " + start_date + " " + end_date + " " + "linearmodel";
    FILE *pipe = popen(pythonCommand.c_str(), "r");
    if (!pipe)
    {
        std::cerr << "Error executing command!" << std::endl;
    }
    pclose(pipe);
    read_prediction_data(symbol + "_linearmodel.csv");
}

void LinearModel::execute_strategy(const double &x, const double &p)
{
    int i = 0;
    int stocks_inhand = 0;

    double cash_flow = 0;
    const double pB = (100 + p) / 100;
    const double pS = (100 - p) / 100;
    for (; i < predict_Y.size(); i++)
    {
        if (predict_Y[i] > pB * real_Y[i] && stocks_inhand < x)
        {
            stocks_inhand++;
            cash_flow -= real_Y[i];
            order_statics.push_back(predict_Dates[i] + ",BUY,1," + std::to_string(real_Y[i]));
        }
        else if (predict_Y[i] < pS * real_Y[i] && stocks_inhand > -x)
        {
            stocks_inhand--;
            cash_flow += real_Y[i];
            order_statics.push_back(predict_Dates[i] + ",SELL,1," + std::to_string(real_Y[i]));
        }
        daily_cashflow.push_back(predict_Dates[i] + "," + std::to_string(cash_flow));
    }
    cash_flow += (stocks_inhand * real_Y[predict_Y.size() - 1]);
    pnl = cash_flow;
    std::cout << "Generated." << std::endl;
}

LinearModel::~LinearModel()
{
}

void LinearModel::generate_result()
{
    order_stats(order_statics, "order_statistics.csv");
    daily_flow(daily_cashflow, "daily_cashflow.csv");
    final_pnl(pnl, "final_pnl.txt");
}

// int main(int argc, char *argv[])
// {
//     LinearModel lm(argv[1], argv[7], argv[6], argv[5], argv[4]);
//     lm.execute_strategy(std::stod(argv[2]), std::stod(argv[3]));
//     lm.generate_result();
//     return 0;
// }
