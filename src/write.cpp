#include "punch.hpp"
#include "punch/write.hpp"
#include "punch/read.hpp"

// file manipulation
#include <filesystem>
#include <fstream>

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
        throw std::runtime_error{"already punched in"};
    }
    // otherwise write the current time to the punch-in file
    std::ofstream punch_file(paths::punch_in);
    std::time_t const timestamp = std::time(nullptr);
    punch_file << std::asctime(std::localtime(&timestamp));
}

void punch_out()
{
    // can't punch out if not punched in
    if (!fs::exists(paths::punch_in)) {
        throw std::runtime_error{"not punched in"};
    }
    // otherwise read the punch-in time
    std::tm in_time;
    read_punch(&in_time);

    // read the punch-out time
    std::time_t current_time = std::time(nullptr);
    std::tm out_time;
    if (!localtime_r(&current_time, &out_time)) {
        throw std::runtime_error{"couldn't read local time"};
    }

    // update punch-cards
    auto punch_cards = readlogs();
    punch_cards.emplace_back(in_time, out_time);
    writelogs(punch_cards);

    // finally remove the punch-in path
    fs::remove(paths::punch_in);
}

void writelogs(std::vector<card> const & logs)
{
    // load the file for writing
    std::ofstream punch_file(paths::punch_cards);

    // write the cards line-by-line
    std::for_each(logs.begin(), logs.end(),
    [&punch_file](auto const & punch_card) {
        punch_file << punch_card << std::endl;
    });
}

void cleanlogs()
{
    // don't do anything if punch-card path doesn't exist
    if (!fs::exists(paths::punch_cards)) {
        return;
    }
    // otherwise read logs and skip invalid lines
    auto logs = readlogs(true);
    writelogs(logs);
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
