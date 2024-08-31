#include "Database.h"
#include <thread>
#include "Basic.h"
#include "Dma.h"
#include "improved_dma.h"
#include "Rsi.h"
#include "Adx.h"
#include "LinearModel.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>

// Function to get the date one year before the given date
std::tm getOneYearBeforeDate(const std::tm &currentDate)
{
    // Create a time point from the given date
    std::time_t currentTime_t = std::mktime(const_cast<std::tm *>(&currentDate));

    // Create a time point from the time_t
    std::chrono::system_clock::time_point timePoint = std::chrono::system_clock::from_time_t(currentTime_t);
    // Subtract one year
    timePoint -= std::chrono::hours(24 * 365); // Assuming a year has 365 days

    // Convert the time point back to a std::tm structure
    std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
    std::tm result = *std::localtime(&time);

    return result;
}

std::tm getDaysBeforeDate(const std::tm &currentDate, int daysBefore)
{
    // Create a time point from the given date
    std::time_t currentTime_t = std::mktime(const_cast<std::tm *>(&currentDate));
    std::chrono::system_clock::time_point timePoint = std::chrono::system_clock::from_time_t(currentTime_t);
    // Subtract the specified number of days
    timePoint -= std::chrono::hours(24 * daysBefore);

    // Convert the time point back to a std::tm structure
    std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
    std::tm result = *std::localtime(&time);

    return result;
}

std::tm stringToDate(const std::string &dateString)
{
    std::tm dateStruct = {};
    std::istringstream dateStream(dateString);
    dateStream >> std::get_time(&dateStruct, "%d/%m/%Y");
    return dateStruct;
}

// Function to convert std::tm structure to a string date
std::string dateToString(const std::tm &dateStruct)
{
    std::ostringstream dateStream;
    dateStream << std::put_time(&dateStruct, "%d/%m/%Y");
    return dateStream.str();
}

int main(int argc, char *argv[])
{
    std::string symb = argv[1];
    std::string start = argv[2];
    std::string end = argv[3];
    Basic b(symb, start, end, 1, 5, 7);
    Dma dma(symb, start, end, 1, 5, 14, 2);
    Improved_Dma impd(symb, start, end, 1, 5, 14, 5, 2, 0.2, 28);
    Rsi rsi(symb, start, end, 1, 5, 14, 9, 70, 30);
    Adx adx(symb, start, end, 1, 5, 14);
    const std::string start_train = dateToString(getOneYearBeforeDate(stringToDate(start)));
    const std::string end_train = dateToString(getDaysBeforeDate(stringToDate(start), 1));
    LinearModel lm(symb, end, start, end_train, start_train);

    std::thread thread1(&Basic::execute_strategy, &b, 5.0);
    std::thread thread2(&Dma::execute_strategy, &dma, 5.0);
    std::thread thread3(&Improved_Dma::execute_strategy, &impd, 5.0);
    std::thread thread4(&Rsi::execute_strategy, &rsi, 5.0);
    std::thread thread5(&Adx::execute_strategy, &adx, 25.0, 5.0);
    std::thread thread6(&LinearModel::execute_strategy, &lm, 5.0, 2.0);

    // Wait for all threads to finish
    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();
    thread5.join();
    thread6.join();

    std::cout << b.pnl << " " << dma.pnl << " " << impd.pnl << " " << rsi.pnl << " " << adx.pnl << " " << lm.pnl << std::endl;
    std::vector<double> profits_loss;
    profits_loss.push_back(b.pnl);
    profits_loss.push_back(dma.pnl);
    profits_loss.push_back(impd.pnl);
    profits_loss.push_back(rsi.pnl);
    profits_loss.push_back(lm.pnl);
    profits_loss.push_back(adx.pnl);

    double best_ans = profits_loss[0];
    int index = 0;
    for (int i = 0; i < 6; i++)
    {
        if (best_ans < profits_loss[i])
        {
            index = i;
            best_ans = profits_loss[i];
        }
    }

    if (index == 0)
    {
        b.generate_result();
    }
    else if (index == 1)
    {
        dma.generate_result();
    }
    else if (index == 2)
    {
        impd.generate_result();
    }
    else if (index == 3)
    {
        rsi.generate_result();
    }
    else if (index == 4)
    {
        lm.generate_result();
    }
    else if (index == 5)
    {
        adx.generate_result();
    }

    return 0;
}