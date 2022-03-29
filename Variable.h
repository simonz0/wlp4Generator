/**
 * @file Variable.h
 * @author Simon Zhang (simon.zhang@uwaterloo.ca)
 * @brief 
 * @version 0.1
 * @date 2022-03-15
 * 
 * @copyright Copyright (c) 2022 belongs to Simon Zhang @ University of Waterloo
 * 
 */
#ifndef _VARIABLE_H
#define _VARIABLE_H

#include <string>
using namespace std;
/**
 * @brief class variable
 * 
 */
class Variable {
    public:
    string name;
    string type;
    int offset;
};

#endif