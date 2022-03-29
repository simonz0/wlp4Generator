/**
 * @file ProcedureSigniture.cc
 * @author Simon Zhang (simon.zhang@uwaterloo.ca)
 * @brief 
 * @version 0.1
 * @date 2022-03-15
 * 
 * @copyright Copyright (c) 2022 belongs to Simon Zhang @ University of Waterloo
 * 
 */

#include "ProcedureSigniture.h"
#include <map>
#include <string>
#include <vector>
using namespace std;

bool ProcedureSigniture::procedureExist(string procedureName) {
    auto it = this->signiture.find(procedureName);
    if (it == this->signiture.end()) {
        return false;
    } else {
        return true;
    }
}

vector<string> ProcedureSigniture::paramTypes(string procedureName) {
    return this->signiture.find(procedureName)->second;
}

int ProcedureSigniture::paramNum(string procedureName) {
    vector<string> result = paramTypes(procedureName);
    return result.size();
}