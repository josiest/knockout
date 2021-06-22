#pragma once

#include "punch.hpp"
#include <iostream>
#include <vector>
#include <ctime>
#include <iomanip>

namespace std {
    inline ostream & operator<<(ostream & os, card const & entry)
    {
        std::tm const & in_time = get<0>(entry);
        std::tm const & out_time = get<1>(entry);

        os << std::put_time(&in_time, formats::ctime) << ","
           << std::put_time(&out_time, formats::ctime);
        return os;
    }
}

/**
 * Parse a single timestamp
 *
 * :param timestamp: the timestamp to parse
 * :return: the std::tm struct that represents it
 * :throws: std::invalid_argument if timestamp is ill-formatted
 */
std::tm parse_timestamp(std::string const & timestamp);

/**
 * Parse a punch-card
 *
 * :param line: the entry string to parse
 *
 * :return: the timestamps associated with the punch-in and punch-out
 *
 * :throws: invalid_argument if the string is not valid format
 */
card parse_card(std::string const & line);

/**
 * Read the latest punch-in
 *
 * :param in_time: the pointer to write the time to
 * :returns: true if in_time was written to
 * :throws: runtime_error if punch is invalid format
 */
bool read_punch(std::tm * in_time);

/**
 * Read punch-cards from file
 *
 * :param skip_invalid: specify to skip invalid lines. defaults to false
 *
 * :return: a vector of all the punch-cards
 */
std::vector<card> read_cards(bool skip_invalid = false);

/**
 * Print a single timecard
 */
void print_card(std::ostream & os, card const & punch_card);

/**
 * Print the timecards
 */
void print_cards();
