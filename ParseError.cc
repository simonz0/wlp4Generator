/**
 * @file ParseError.cc
 * @author Simon Zhang (simon.zhang@uwaterloo.ca)
 * @brief 
 * @version 0.1
 * @date 2022-03-15
 * 
 * @copyright Copyright (c) 2022 belongs to Simon Zhang @ University of Waterloo
 * 
 */

#include "ParseError.h"
#include <iostream>
#include <string>

ParseError::ParseError(std::string message) : message(std::move(message)) {}

const std::string &ParseError::what() const { return message; }