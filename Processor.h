/**
 * @file Processor.h
 * @author Simon Zhang (simon.zhang@uwaterloo.ca)
 * @brief 
 * @version 0.1
 * @date 2022-03-15
 * 
 * @copyright Copyright (c) 2022 belongs to Simon Zhang @ University of Waterloo
 * 
 */

#ifndef _PROCESSOR_H
#define _PROCESSOR_H

#include <set>
#include <string>
#include "Word.h"
#include "SymbolTable.h"
#include "ProcedureSigniture.h"

using namespace std;

class Processor {
    public:
    string currentProcedure;
    SymbolNode* parseTree;
    SymbolTable table;
    ProcedureSigniture signiture;
    const set<string> terminals = {
        "BOF", "BECOMES", "COMMA", "ELSE", "EOF", "EQ", "GE", "GT", "ID", "IF", "INT", "LBRACE", "LE", "LPAREN", "LT", "MINUS", "NE", "NUM", "PCT", "PLUS", "PRINTLN", "RBRACE", "RETURN", "RPAREN", "SEMI", "SLASH", "STAR", "WAIN", "WHILE", "AMP", "LBRACK", "RBRACK", "NEW", "DELETE", "NULL"
        };
    const string mainProcedure = "main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE";
    const string newProcedure = "procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE";
    const string declarationsRule = "dcl type ID";
    const string mainProcedureName = "wain";
    const string variableUse1 = "factor ID";
    const string variableUse2 = "lvalue ID";
    const string functionCall1 = "factor ID LPAREN RPAREN";
    const string functionCall2 = "factor ID LPAREN arglist RPAREN";
    const string params = "params";
    void buildParseTree(SymbolNode* node);
    void buildSymbolTableAndSigniture(SymbolNode* node);
    void initializeParseTree();
    void buildParams(SymbolNode* node);
    void printSymbolTableAndSigniture();
    void destroyParseTree(SymbolNode* node);
};

#endif