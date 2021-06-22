#pragma once

#include "punch.hpp"
#include <vector>

/**
 * Delete any invalid punch-card from punch-cards file
 */
void cleanlogs();

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
 * Write punch-cards to file
 *
 * :param logs: the timelogs to write
 */
void writelogs(std::vector<card> const & logs);

/**
 * Archive the current punch-cards file
 */
void archive();
