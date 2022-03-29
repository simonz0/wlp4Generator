/**
 * @file ParseError.h
 * @author Simon Zhang (simon.zhang@uwaterloo.ca)
 * @brief 
 * @version 0.1
 * @date 2022-03-15
 * 
 * @copyright Copyright (c) 2022 belongs to Simon Zhang @ University of Waterloo
 * 
 */

#ifndef _PARSEERROR_H
#define _PARSEERROR_H

#include <iostream>
#include <string>

class ParseError {
    std::string message;

  public:
    ParseError(std::string message);

    // Returns the message associated with the exception.
    const std::string &what() const;
};

#endif