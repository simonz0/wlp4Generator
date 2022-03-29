/**
 * @file SymbolTable.cc
 * @author Simon Zhang (simon.zhang@uwaterloo.ca)
 * @brief 
 * @version 0.1
 * @date 2022-03-15
 * 
 * @copyright Copyright (c) 2022 belongs to Simon Zhang @ University of Waterloo
 * 
 */

#include <string>
#include <vector>
#include <map>
#include "Variable.h"
#include "SymbolTable.h"
using namespace std;

bool SymbolTable::procedureExist(string procedureName) {
    if (this->table.find(procedureName) == table.end()) {
        return false;
    } else {
        return true;
    }
}

bool SymbolTable::variableExist(string procedureName, string variableName) {
    auto it = this->table.find(procedureName);
    if (it == table.end()) {
        return false;
    }
    for (auto i : it->second) {
        if (i.name == variableName) {
            return true;
        }
    }
    return false;
}

string SymbolTable::variableType(string procedureName, string variableName) {
    auto it = this->table.find(procedureName);
    for (auto i : it->second) {
        if (i.name == variableName) {
            return i.type;
        }
    }
}

int SymbolTable::index(string procedureName, string variableName) {
    auto it = this->table.find(procedureName);
    for (auto i = 0; i < it->second.size(); i++) {
        if (it->second.at(i).name == variableName) {
            return i+1;
        }
    }
}

