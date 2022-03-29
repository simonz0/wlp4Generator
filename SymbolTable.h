/**
 * @file SymbolTable.h
 * @author Simon Zhang (simon.zhang@uwaterloo.ca)
 * @brief 
 * @version 0.1
 * @date 2022-03-15
 * 
 * @copyright Copyright (c) 2022 belongs to Simon Zhang @ University of Waterloo
 * 
 */

#ifndef _SYMBOLTABLE_H
#define _SYMBOLTABLE_H

#include <string>
#include <vector>
#include <map>
#include "Variable.h"
using namespace std;

class SymbolTable {
    public:
    // key = procedure name and vector of variables is variables declared at this stage
    map<string, vector<Variable>> table;
    bool procedureExist(string procedureName);
    bool variableExist(string procedureName, string variableName);
    string variableType(string procedureName, string variableName);
    int index(string procedureName, string variableName);
};


#endif