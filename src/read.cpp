#include "punch/read.hpp"

// date writing
#include <ctime>
#include <iomanip>

// file writing
#include <filesystem>
#include <fstream>

// string manipulation
#include <string>
#include <sstream>
#include <ios>

// functional interface
#include <algorithm>

std::tm parse_timestamp(std::string const & timestamp)
{
    // parse the timestamp
    std::tm t;
    std::istringstream token{timestamp};
    token >> std::get_time(&t, formats::ctime);

    // throw error if formatted incorrectly
    if (token.fail()) {
        throw std::invalid_argument{"timestamp formatted incorrectly"};
    }
    return t;
}

card parse_card(std::string const & line)
{
    std::string const delim = ",";

    // throw error if not at least two entries
    size_t const delim_pos = line.find(delim);
    if (delim_pos == std::string::npos) {
        throw std::invalid_argument{"first timestamp not found"};
    }
    // split the entry string
    std::string const punch_in = line.substr(0, delim_pos);
    std::string const punch_out = line.substr(delim_pos+delim.size());

    // throw error if not at most two entries
    if (punch_out.find(delim) != std::string::npos) {
        throw std::invalid_argument{"more than two timestamps found"};
    }
    return card{parse_timestamp(punch_in), parse_timestamp(punch_out)};
}

bool read_punch(std::tm * in_time)
{
    // don't read anything if not punched in
    if (!fs::exists(paths::punch_in)) {
        return false;
    }
    // otherwise open punch-in file and parse the timestamp
    std::ifstream punch_file(paths::punch_in);
    std::string timestamp;
    std::getline(punch_file, timestamp);
    *in_time = parse_timestamp(timestamp);
    return true;
}

std::vector<card> read_cards(bool skip_invalid)
{
    // if no punch card file exists, then no hours have been punched
    std::vector<card> punch_cards;
    if (!fs::exists(paths::punch_cards)) {
        return punch_cards;
    }
    // otherwise load each line of the punch-card file
    std::vector<std::string> lines;
    {
        std::ifstream punch_cards(paths::punch_cards);
        std::string line;
        while (getline(punch_cards, line)) {
            lines.push_back(line);
        }
    }
    // finally parse the timestamps into punch-cards
    punch_cards.reserve(lines.size());
    std::transform(lines.begin(), lines.end(),
                   std::back_inserter(punch_cards), parse_card);
    return punch_cards;
}

void print_card(std::ostream & os, card const & punch_card)
{
    std::tm punch_in = std::get<0>(punch_card);
    std::tm punch_out = std::get<1>(punch_card);

    // print the day
    os << std::put_time(&punch_in, formats::day);
    if (punch_in.tm_mday != punch_out.tm_mday) {
        os << " - " << std::put_time(&punch_out, formats::day);
    }
    os << std::endl;

    // print the time in and time out
    os << "Time in:\t" << std::put_time(&punch_in, formats::hour)
                       << std::endl;

    os << "Time out:\t" << std::put_time(&punch_out, formats::hour)
                        << std::endl;

    // print the total hours
    auto const hours = std::difftime(std::mktime(&punch_out),
                                     std::mktime(&punch_in)) / 3600.0;
    os << std::setprecision(2) << std::fixed;
    os << "Total hours: " << hours << std::endl;
}

void print_cards()
{
    // read the timecards
    auto const punch_cards = read_cards();
    // print each punch-card in a printable way
    std::string sep = "";
    std::for_each(punch_cards.begin(), punch_cards.end(),
                  [&sep](card const & punch_card) {

        std::cout << sep;
        print_card(std::cout, punch_card);
        sep = "\n";
    });

    // print the latest punch in if it exists
    std::tm latest_punch;
    if (read_punch(&latest_punch)) {
        std::cout << sep << std::put_time(&latest_punch, formats::day)
                  << std::endl
                  << "Time in: " << std::put_time(&latest_punch, formats::hour)
                  << std::endl;
    }
}
