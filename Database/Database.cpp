#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Database.h"
#include <cstdlib>
#include <cstdio>

Database::Database()
{
    data = std::vector<StockData>();
    start_idx = 0;
}

Database::~Database()
{
}

void Database::read_data(const std::string &filepath)
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
        std::string dateStr, closeStr;
        std::getline(ss, dateStr, ',');
        std::getline(ss, closeStr, ',');
        StockData current;
        current.date = dateStr;
        current.close = std::stod(closeStr);
        data.push_back(current);
    }
    infile.close();
}

void Database::show_data()
{
    for (const auto &current : data)
    {
        std::cout << "Date: " << current.date << ", Close : " << current.close << std::endl;
    }
}

std::vector<StockData> &Database::get_data()
{
    return data;
}

void Database::generate_data(const std::string &symbol, const std::string &start_date, const std::string &end_date, const int &n, const std::string &strategy)
{
    std::string pythonCommand = "python3 get_stock_data.py " + symbol + " " + start_date + " " + end_date + " " + std::to_string(n) + " " + strategy;
    FILE *pipe = popen(pythonCommand.c_str(), "r");
    if (!pipe)
    {
        std::cerr << "Error executing command!" << std::endl;
    }
    int start_date_index;
    fscanf(pipe, "%d", &start_date_index);
    pclose(pipe);
    start_idx = start_date_index;
    /// std::cout<<start_idx<<std::endl;
    read_data(symbol + "_" + strategy + ".csv");
};

void order_stats(const std::vector<std::string> order_statics, const std::string filename)
{
    std::ofstream outputFile(filename);
    outputFile << "Date,Order_dir,Quantity,Price" << std::endl;
    for (std::string s : order_statics)
    {
        outputFile << s << std::endl;
    }
}

void daily_flow(const std::vector<std::string> daily_cashflow, const std::string filename)
{
    std::ofstream outputFile(filename);
    outputFile << "Date,Cashflow" << std::endl;
    for (std::string s : daily_cashflow)
    {
        outputFile << s << std::endl;
    }
}
void final_pnl(const double &pnl, const std::string filename)
{
    std::ofstream fin_pnl(filename);
    fin_pnl << pnl << std::endl;
}