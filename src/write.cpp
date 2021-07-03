#include "punch.hpp"
#include "punch/write.hpp"
#include "punch/read.hpp"

// file manipulation
#include <filesystem>
#include <fstream>
#include <iostream>

// time handling
#include <ctime>
#include <time.h>

// data-structures and algorithms
#include <vector>
#include <algorithm>
#include <iterator>

namespace fs = std::filesystem;

void punch_in()
{
    // don't punch in if already punched in
    if (fs::exists(paths::punch_in)) {
        throw std::runtime_error{"Already punched in"};
    }
    // otherwise read the current time
    std::time_t const now = std::time(nullptr);
    std::tm in_time;
    if (!localtime_r(&now, &in_time)) {
        throw std::runtime_error("Couldn't read local time");
    }

    // and write it to the punch-in file
    std::ofstream punch_file(paths::punch_in);
    punch_file << std::put_time(&in_time, formats::ctime);

    // finally notify the user about the punch-in info
    std::cout << "Punched in at " << std::put_time(&in_time, formats::hour)
              << std::endl;
}

void punch_out()
{
    // can't punch out if not punched in
    if (!fs::exists(paths::punch_in)) {
        throw std::runtime_error{"Not punched in"};
    }
    // otherwise read the punch-in time
    std::tm in_time;
    read_punch(&in_time);

    // read the punch-out time
    std::time_t now = std::time(nullptr);
    std::tm out_time;
    if (!localtime_r(&now, &out_time)) {
        throw std::runtime_error{"Couldn't read local time"};
    }

    // update punch-cards
    auto punch_cards = read_cards();
    punch_cards.emplace_back(in_time, out_time);
    write_cards(punch_cards);

    // notify the user of punch-out info
    std::cout << "Punched out at " << std::put_time(&out_time, formats::hour)
              << std::endl;

    // finally remove the punch-in path
    fs::remove(paths::punch_in);
}

void write_cards(std::vector<card> const & cards)
{
    // load the file for writing
    std::ofstream punch_file(paths::punch_cards);

    // write the cards line-by-line
    std::for_each(cards.begin(), cards.end(),
    [&punch_file](auto const & punch_card) {
        punch_file << punch_card << std::endl;
    });
}

void clean_cards()
{
    // don't do anything if punch-card path doesn't exist
    if (!fs::exists(paths::punch_cards)) {
        return;
    }
    // otherwise read logs and skip invalid lines
    auto logs = read_cards(true);
    write_cards(logs);
    std::cout << "Timecards have been cleaned" << std::endl;
}

void archive()
{
    // determine the amount of files in the archive directory
    fs::directory_iterator const begin(paths::archives);
    fs::directory_iterator const end;
    size_t const n = std::distance(begin, end);

    // generate the archive name
    std::ostringstream filename;
    filename << "punch-cards-" << n << ".csv";

    // move the file
    fs::rename(paths::punch_cards, paths::archives/filename.str());
}
