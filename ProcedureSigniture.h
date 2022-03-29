/**
 * @file ProcedureSigniture.h
 * @author Simon Zhang (simon.zhang@uwaterloo.ca)
 * @brief 
 * @version 0.1
 * @date 2022-03-15
 * 
 * @copyright Copyright (c) 2022 belongs to Simon Zhang @ University of Waterloo
 * 
 */

#ifndef _PROCEDURESIG_H
#define _PROCEDURESIG_H

#include <string>
#include <map>
#include <vector>

using namespace std;


class ProcedureSigniture {
    public:
    // key = variable name and value is parameter types
    map<string, vector<string>> signiture;
    bool procedureExist(string procedureName);
    vector<string> paramTypes(string procedureName);
    int paramNum(string procedureName);
};

#endif