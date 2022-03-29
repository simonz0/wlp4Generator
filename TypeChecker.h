/**
 * @file TypeChecker.h
 * @author Simon Zhang (simon.zhang@uwaterloo.ca)
 * @brief 
 * @version 0.1
 * @date 2022-03-16
 * 
 * @copyright Copyright (c) 2022 belongs to Simon Zhang @ University of Waterloo
 * 
 */

#ifndef _TYPECHECK_H
#define _TYPECHECK_H

#include <string>
#include "Word.h"
#include "SymbolTable.h"
#include "ProcedureSigniture.h"

class TypeChecker {
    public:
    string currentProcedure;
    const string mainProcedureName = "wain";
    const string mainProcedure = "main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE";
    const string newProcedure = "procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE";
    const string intType = "int";
    const string pointerType = "int*";
    const string expr1 = "expr term";
    const string expr2 = "expr expr PLUS term";
    const string expr3 = "expr expr MINUS term";
    const string term1 = "term factor";
    const string term2 = "term term STAR factor";
    const string term3 = "term term SLASH factor";
    const string term4 = "term term PCT factor";
    const string factor1 = "factor ID";
    const string factor2 = "factor NUM";
    const string factor3 = "factor NULL";
    const string factor4 = "factor LPAREN expr RPAREN";
    const string factor5 = "factor AMP lvalue";
    const string factor6 = "factor STAR factor";
    const string factor7 = "factor NEW INT LBRACK expr RBRACK";
    const string factor8 = "factor ID LPAREN RPAREN";
    const string factor9 = "factor ID LPAREN arglist RPAREN";
    const string arglist1 = "arglist expr";
    const string arglist2 = "arglist → expr COMMA arglist";
    const string lvalue1 = "lvalue ID";
    const string lvalue2 = "lvalue STAR factor";
    const string lvalue3 = "lvalue LPAREN lvalue RPAREN";
    const string statement1 = "statement lvalue BECOMES expr SEMI";
    const string statement2 = "statement PRINTLN LPAREN expr RPAREN SEMI";
    const string statement3 = "statement DELETE LBRACK RBRACK expr SEMI";
    const string test1 = "test expr EQ expr";
    const string test2 = "test expr NE expr";
    const string test3 = "test expr LT expr";
    const string test4 = "test expr LE expr";
    const string test5 = "test expr GE expr";
    const string test6 = "test expr GT expr";
    const string dcl1 = "dcls dcls dcl BECOMES NUM SEMI";
    const string dcl2 = "dcls dcls dcl BECOMES NULL SEMI";
    string check(SymbolNode* node, SymbolTable& table, ProcedureSigniture& signiture);
    void checkParams(SymbolNode* node, SymbolTable& table, ProcedureSigniture& signiture, vector<string>& params);
    
};
#endif