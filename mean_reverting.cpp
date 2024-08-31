#include <iostream>
#include <string>
#include <cstdlib>

int main(int argc, char *argv[])
{
    if (argc == 9)
    {
        std::string fullCommand = "g++ Database/Database.cpp Mean_reverting_stop_loss/mean_reverting_stoploss.cpp -o mean";
        int res = system(fullCommand.c_str());
        if (res == 0)
        {
            std::string arguments = " " + std::string(argv[1]) + " " + std::string(argv[2]) + " " + std::string(argv[3]) + " " + std::string(argv[4]) + " " + std::string(argv[5]) + " " + std::string(argv[6]) + " " + std::string(argv[7]) + " " + std::string(argv[8]);
            fullCommand = "./mean" + arguments;
            system(fullCommand.c_str());
        }
    }
    else
    {
        std::string fullCommand = "g++ Database/Database.cpp Mean_reverting/mean_reverting.cpp -o mean";
        std::cout << fullCommand << std::endl;
        int res = system(fullCommand.c_str());
        if (res == 0)
        {
            std::string arguments = " " + std::string(argv[1]) + " " + std::string(argv[2]) + " " + std::string(argv[3]) + " " + std::string(argv[4]) + " " + std::string(argv[5]) + " " + std::string(argv[6]) + " " + std::string(argv[7]) + " ";
            fullCommand = "./mean" + arguments;

            system(fullCommand.c_str());
        }
    }
    return 0;
}
