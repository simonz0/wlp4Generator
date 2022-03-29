/**
 * @file Word.h
 * @author Simon Zhang (simon.zhang@uwaterloo.ca)
 * @brief 
 * @version 0.1
 * @date 2022-03-15
 * 
 * @copyright Copyright (c) 2022 belongs to Simon Zhang @ University of Waterloo
 * 
 */

#ifndef _WORD_H
#define _WORD_H

#include <string>
#include <vector>

using namespace std;

class SymbolNode {
    public:
    string rule;
    string symbol;
    string lex;
    string type;
    bool isTerminal;
    vector<SymbolNode*> children;
};

#endif