#pragma once

#include "punch.hpp"
#include <vector>

/**
 * Delete any invalid timecards from punch-cards file
 */
void clean_cards();

/**
 * Write a punch-in with the current time
 *
 * :throws: std::runtime_error if already punched in
 */
void punch_in();

/**
 * Write a punch-out with the current time
 *
 * :throws: std::runtime_error if not punched in or if punch-in is ill-formatted
 */
void punch_out();

/**
 * Write timecards to file
 *
 * :param cards: the timecards to write
 */
void write_cards(std::vector<card> const & cards);

/**
 * Archive the current punch-cards file
 */
void archive();
