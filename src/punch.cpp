#include "punch.hpp"

// paths
#include <filesystem>

// data structures
#include <unordered_map>

// input/output
#include <iostream>
#include <fstream>
#include <ctime>
#include <stdexcept>

namespace fs = std::filesystem;

int main(int argc, char * argv[])
{
    using namespace std;

    unordered_map<string, Command> argmap = {
        {"in", Command::In}, {"out", Command::Out}, {"log", Command::Log}
    };

    // print usage if not used correctly
    string const usage = "punch <in|out|log|clean>";
    if (argc < 2) {
        cout << usage << endl;
        return 1;
    }
    // clean the punch-card files if specified
    string const arg = argv[1];
    auto const command = map_get(argmap, arg, Command::Invalid);
    if (command == Command::Clean) {
        cleanlogs();
        return 0;
    }

    // read punch logs
    std::string const clean_message = 
        "consider auto-cleaning the punch log files (punch clean)\n"
        "note that this will remove any invalid timelog entries\n"
        "or alternatively, fix the error manually";

    // try to read latest punch in, notify the user if punch is invalid
    std::tm * punch_in;
    try {
        punch_in = read_punch();
    }
    catch (std::runtime_error e) {
        std::cout << "there was an error reading the latest punch-in:"
                  << std::endl
                  << "  " << e.what() << std::endl
                  << clean_message << std::endl;
        return 1;
    }

    // try to read punch-cards, notify the user if logs are invalid
    std::vector<log_entry> logs;
    try {
        logs = readlogs();
    }
    catch (std::runtime_error e) {
        std::cout << "there was an error reading the timelogs:" << std::endl
                  << "  " << e.what() << std::endl
                  << clean_message << std::endl;
        return 1;
    }

    // process arguments
    std::time_t t = std::time(nullptr);
    auto const timestamp = std::asctime(std::localtime(&t));

    switch (command) {
    case Command::In:
        std::cout << timestamp << std::endl;
        break;
    case Command::Out:
        std::cout << timestamp << std::endl;
        break;
    default:
        cout << usage << endl;
        return 1;
    }
    return 0;
}

void cleanlogs()
{
}

std::tm * read_punch()
{
    return nullptr;
}

void writelogs(std::vector<log_entry> const & logs)
{
}

std::vector<log_entry> readlogs()
{
    // if no punch card file exists, then no hours have been punched
    std::vector<log_entry> logs;
    if (!fs::exists(paths::punch_cards)) {
        return logs;
    }
    // otherwise load the punch-card file and parse each line
    std::ifstream punch_cards(paths::punch_cards);
    std::string line;
    std::string error_message;

    // parse_log_entry may throw an error
    // we want to pass that error down, but close the file
    try {
        while (getline(punch_cards, line)) {
            logs.push_back(parse_log_entry(line));
        }
    }
    // if there was an error parsing the file, save it
    catch (std::exception e) {
        error_message = e.what();
    }
    // close the file and pass any errors down
    punch_cards.close();
    if (!error_message.empty()) {
        throw std::runtime_error{error_message};
    }
    return logs;
}

log_entry parse_log_entry(std::string const & line)
{
    std::string const delim = ",";
    std::string parsed{line};
    return log_entry{{0}, {0}};
}
