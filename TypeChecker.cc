/**
 * @file TypeChecker.cc
 * @author Simon Zhang (simon.zhang@uwaterloo.ca)
 * @brief 
 * @version 0.1
 * @date 2022-03-16
 * 
 * @copyright Copyright (c) 2022 belongs to Simon Zhang @ University of Waterloo
 * 
 */

#include <string>
#include "TypeChecker.h"
#include "Word.h"
#include "ParseError.h"
#include "SymbolTable.h"
#include "ProcedureSigniture.h"


string TypeChecker::check(SymbolNode* node, SymbolTable& table, ProcedureSigniture& signiture) {
    
    // track current procedure name
    if (node->rule == this->mainProcedure || node->rule == this->newProcedure) {
        // we need to change the current procedure and check duplicate procedure name
        if (node->rule == this->mainProcedure) {
            this->currentProcedure = this->mainProcedureName;
            // second dcl (5th) must derive int
            int only1 = node->children.at(5)->children.front()->children.size();
            if (only1 != 1) {
                throw ParseError("ERROR: second param of wain must be int");
            }
            // wain must return int
            string returnType = check(node->children.at(11), table, signiture);
            if (returnType != this->intType) {
                throw ParseError("ERROR: procedure must return int!");
            }             
        } else {
            for (auto i : node->children) {
                if (i->symbol == "ID") {
                    this->currentProcedure = i->lex;
                    break;
                }
            }

            // expr in this must have type int;
            string returnType = check(node->children.at(9), table, signiture);
            if (returnType != this->intType) {
                throw ParseError("ERROR: procedure must return int!");
            }
        }
    }

    // apply rules
    if (node->symbol == "NUM") {
        node->type = this->intType;
        return this->intType;
    }
    else if (node->symbol == "NULL") {
        node->type = this->pointerType;
        return this->pointerType;
    }
    else if (node->symbol == "ID") {
        bool variableExist = table.variableExist(this->currentProcedure, node->lex);
        if (!variableExist) {
            return "";
        }
        node->type = table.variableType(this->currentProcedure, node->lex);
        return node->type;
    }
    else if (node->rule == this->factor1) {
        node->type = check(node->children.front(), table, signiture);
        return node->type;
    }
    else if (node->rule == this->factor2) {
        node->type = check(node->children.front(), table, signiture);
        return node->type;
    }
    else if (node->rule == this->factor3) {
        node->type = check(node->children.front(), table, signiture);
        return node->type;
    }
    else if (node ->rule == this->factor4) {
        node->type = check(node->children.at(1), table, signiture);
        return node->type;
    }
    else if (node->rule == this->factor5) {
        string lvalueType = check(node->children.at(1), table, signiture);
        if (lvalueType != this->intType) {
            string errorMsg = "ERROR: " + node->rule;
            throw ParseError(errorMsg);           
        }
        node->type = this->pointerType;
        return this->pointerType;
    }
    else if (node->rule == this->factor6) {
        string childFactor = check(node->children.back(), table, signiture);
        if (childFactor != this->pointerType) {
            string errorMsg = "ERROR: " + node->rule;
            throw ParseError(errorMsg);
        }
        node->type = this->intType;
        return this->intType;
    }
    else if (node->rule == this->factor7) {
        string childExpr = check(node->children.at(3), table, signiture);
        if (childExpr != this->intType) {
            string errorMsg = "ERROR: " + node->rule;
            throw ParseError(errorMsg);
        }
        node->type = this->pointerType;
        return this->pointerType;
    }
    else if (node->rule == this->factor8) {
        string procedureName = node->children.front()->lex;
        vector<string> types = signiture.paramTypes(procedureName);
        if (types.size() != 0) {
            string errorMsg = "ERROR: Incorrect # of Params @ " + node->rule;
            throw ParseError(errorMsg);
        }
        node->type = this->intType;
        return this->intType;
    }
    else if (node->rule == this->factor9) {
        string procedureName = node->children.front()->lex;
        vector<string> types = signiture.paramTypes(procedureName);
        vector<string> params;
        checkParams(node, table, signiture, params);
        if (types.size() != params.size()) {
            string errorMsg = "ERROR: Incorrect # of Params @ " + node->rule;
            throw ParseError(errorMsg);
        }
        for (int i = 0; i < types.size(); i++) {
            if (types.at(i) != params.at(i)) {
                string errorMsg = "ERROR: Params Types are NOT Matching @ " + node->rule;
                throw ParseError(errorMsg);
            }
        }
        // procedure wain cannot call its self
        if (node->children.front()->lex == this->mainProcedureName && this->currentProcedure == this->mainProcedureName) {
            throw ParseError("ERROR: wain cannot call its self!");
        }
        node->type = this->intType;
        return this->intType;
    }
    else if (node->rule == this->lvalue1) {
        string name = node->children.front()->lex;
        bool variableExist = table.variableExist(this->currentProcedure, name);
        if (!variableExist) {
            string errorMsg = "ERROR: Variable Used NOT Declared @ " + node->rule;
            throw ParseError(errorMsg);
        }
        string type = table.variableType(this->currentProcedure, name);
        node->type = type;
        return type;
    }
    else if (node->rule == this->lvalue2) {
        string type = check(node->children.back(), table, signiture);
        if (type != this->pointerType) {
            string errorMsg = "ERROR: " + node->rule;
            throw ParseError(errorMsg);
        }
        node->type = this->intType;
        return this->intType;
    }
    else if (node->rule == this->lvalue3) {
        string type = check(node->children.at(1), table, signiture);
        node->type = type;
        return type;
    }
    else if (node->rule == this->term1) {
        string type = check(node->children.front(), table, signiture);
        node->type = type;
        return type;
    }
    else if (node->rule == this->term2 || node->rule == this->term3 || node->rule == this->term4) {
        string termChild = check(node->children.front(), table, signiture);
        string factorChild = check(node->children.back(), table, signiture);
        if (termChild != this->intType || factorChild != this->intType) {
            string errorMsg = "ERROR: " + node->rule;
            throw ParseError(errorMsg);
        }
        node->type = this->intType;
        return this->intType;
    }
    else if (node->rule == this->expr1) {
        string type = check(node->children.front(), table, signiture);
        node->type = type;
        return type;
    }
    else if (node->rule == this->expr2) {
        string exprType = check(node->children.front(), table, signiture);
        string termType = check(node->children.back(), table, signiture);
        if (exprType == this->intType && termType == this->intType) {
            node->type = this->intType;
            return this->intType;
        }
        else if (exprType == this->pointerType && termType == this->intType) {
            node->type = this->pointerType;
            return this->pointerType;
        }
        else if (exprType == this->intType && termType == this->pointerType) {
            node->type = this->pointerType;
            return this->pointerType;
        }
        else {
            string errorMsg = "ERROR: " + node->rule;
            throw ParseError(errorMsg);
        }
    }
    else if (node->rule == this->expr3) {
        string exprType = check(node->children.front(), table, signiture);
        string termType = check(node->children.back(), table, signiture);
        if (exprType == this->intType && termType == this->intType) {
            node->type = this->intType;
            return this->intType;
        }
        else if (exprType == this->pointerType && termType == this->intType) {
            node->type = this->pointerType;
            return this->pointerType;
        }
        else if (exprType == this->pointerType && termType == this->pointerType) {
            node->type = this->intType;
            return this->intType;
        }
        else {
            string errorMsg = "ERROR: " + node->rule;
            throw ParseError(errorMsg);
        }
    }
    else if (node->rule == this->statement1) {
        string lvalueType = check(node->children.front(), table, signiture);
        string exprType = check(node->children.at(2), table, signiture);
        if (lvalueType != exprType) {
            string errorMsg = "ERROR: " + node->rule;
            throw ParseError(errorMsg);
        }
    }
    else if (node->rule == this->statement2) {
        string exprType = check(node->children.at(2), table, signiture);
        if (exprType != this->intType) {
            string errorMsg = "ERROR: " + node->rule;
            throw ParseError(errorMsg);
        }
    }
    else if (node->rule == this->statement3) {
        string exprType = check(node->children.at(3), table, signiture);
        if (exprType != this->pointerType) {
            string errorMsg = "ERROR: " + node->rule;
            throw ParseError(errorMsg);
        }
    }
    else if (node ->rule == this->test1 || node ->rule == this->test2 || node ->rule == this->test3 || node ->rule == this->test4 || node ->rule == this->test5 || node ->rule == this->test6) {
        string firstExpr = check(node->children.front(), table, signiture);
        string secondExpr = check(node->children.back(), table, signiture);
        if (firstExpr != secondExpr) {
            string errorMsg = "ERROR: " + node->rule;
            throw ParseError(errorMsg);
        }
    }
    else if (node->rule == this->dcl1) {
        int only1 = node->children.at(1)->children.at(0)->children.size();
        if (only1 != 1) {
            string errorMsg = "ERROR: " + node->rule;
            throw ParseError(errorMsg);
        }
    }
    else if (node->rule == this->dcl2) {
        int only2 = node->children.at(1)->children.at(0)->children.size();
        if (only2 != 2) {
            string errorMsg = "ERROR: " + node->rule;
            throw ParseError(errorMsg);
        }
    } else {

    }
    for (auto i : node->children) {
        check(i, table, signiture);
    }
    return "";
}

void TypeChecker::checkParams(SymbolNode* node, SymbolTable& table, ProcedureSigniture& signiture, vector<string>& params) {
    if (node->symbol == "expr") {
        string type = check(node, table, signiture);
        params.emplace_back(type);
        return;
    } else {
        for (auto i : node->children) {
            checkParams(i, table, signiture, params);
        }
    }
}