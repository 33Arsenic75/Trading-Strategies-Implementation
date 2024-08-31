#ifndef DATABASE_H
#define DATABASE_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

struct StockData {
    std::string date;
    double close;
};

struct Trade{
    std::string date;
    std::string direction;
};

class Database {
public:
    Database();
    ~Database();
    void read_data(const std::string& filepath);
    void show_data();
    std::vector<StockData>& get_data();
    void generate_data(const std::string&symbol,const std::string& start_date, const std::string& end_date, const int&n,const std::string&strategy);
    std::vector<StockData> data;
    int start_idx;
};

void order_stats(const std::vector<std::string>order_statics,const std::string filename);
void daily_flow(const std::vector<std::string>daily_cashflow,const std::string filename);
void final_pnl(const double& pnl,const std::string filename);

#endif  // DATABASE_H
