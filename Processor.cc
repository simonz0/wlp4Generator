/**
 * @file Processor.cc
 * @author Simon Zhang (simon.zhang@uwaterloo.ca)
 * @brief 
 * @version 0.1
 * @date 2022-03-15
 * 
 * @copyright Copyright (c) 2022 belongs to Simon Zhang @ University of Waterloo
 * 
 */

#include <string>
#include <iostream>
#include <sstream>
#include "Processor.h"
#include "ParseError.h"
#include "Variable.h"
using namespace std;

vector<string> splitRule(string rule);
string declarationType(SymbolNode* node);

void Processor::initializeParseTree() {
    this->parseTree = new SymbolNode();
    this->parseTree->symbol = "start";
    this->parseTree->isTerminal = false;  
}

void Processor::buildParseTree(SymbolNode* node) {
    string line;
    if (node->isTerminal) {
        getline(cin, line);
        vector<string> elements = splitRule(line);
        node->lex = elements.back();
        return;
    } else {
        getline(cin, line);
        node->rule = line;
        vector<string> elements = splitRule(line);
        vector<SymbolNode*> children;
        for (int i = 1; i < elements.size(); i++) {
            SymbolNode* temp = new SymbolNode();
            if (this->terminals.find(elements.at(i)) != this->terminals.end()) {
                temp->symbol = elements.at(i);
                temp->isTerminal = true;
                children.emplace_back(temp);
            } else {
                temp->symbol = elements.at(i);
                temp->isTerminal = false;
                children.emplace_back(temp);
            }
        }
        node->children = children;
    }
    for (auto i : node->children) {
        this->buildParseTree(i);
    }
}

void Processor::buildSymbolTableAndSigniture(SymbolNode* node) {
    if (node->rule == this->mainProcedure || node->rule == this->newProcedure) {
        string procedureName;
        // we need to change the current procedure and check duplicate procedure name
        if (node->rule == this->mainProcedure) {
            this->currentProcedure = this->mainProcedureName;
            procedureName = this->mainProcedureName;
        } else {
            for (auto i : node->children) {
                if (i->symbol == "ID") {
                    this->currentProcedure = i->lex;
                    procedureName = i->lex;
                    break;
                }
            }
        }
        // here we need to check if there is another procedure with the same name here
        bool procedureDuplicate = this->table.procedureExist(procedureName);
        if (procedureDuplicate) {
            throw ParseError("ERROR: Duplicate Procedure Name!");
        }

        // if there is not then we need to create a procedure signiture and procedure symbol table here
        vector<string> paramTypes;
        this->signiture.signiture[procedureName] = paramTypes;
        vector<Variable> variables;
        this->table.table[procedureName] = variables;

        // here we dont recursive on third params and fifth params of wain
        if (node->rule == this->mainProcedure) {
            for(int i = 0; i < node->children.size(); i++) {
                if (i == 3 || i == 5) {
                    this->buildParams(node->children.at(i));
                } else {
                    this->buildSymbolTableAndSigniture(node->children.at(i));
                }
            }
            return;
        }

        // we need to check declaration of variables
    } else if (node->rule == this->declarationsRule) {
        string type = declarationType(node);
        string name = node->children.back()->lex;
        bool variableDuplicate = this->table.variableExist(this->currentProcedure, name);
        if (variableDuplicate) {
            throw ParseError("ERROR: Duplicate Variable Name in Same Procedure!");
        }
        // if not duplicate, append it to current procedure symbol table
        Variable temp;
        temp.type = type;
        temp.name = name;
        this->table.table.find(this->currentProcedure)->second.emplace_back(temp);

        // check if variable used before declaration
    } else if (node->rule == this->variableUse1 || node->rule == this->variableUse2) {
        string name = node->children.front()->lex;
        bool variableExist = this->table.variableExist(this->currentProcedure, name);
        if (!variableExist) {
            throw ParseError("ERROR: Variable Used NOT Declared!");
        }

        // check if procedure used before declaration
    } else if (node->rule == this->functionCall1 || node->rule == this->functionCall2) {
        string name = node->children.front()->lex;
        bool procedureExist = this->signiture.procedureExist(name);
        if (!procedureExist) {
            throw ParseError("ERROR: Procedure Called NOT Declared");
        }
        // if we have param tree, then we collect param and return
    } else if (node->symbol == this->params) {
        this->buildParams(node);
        return;
    }

    for (auto i : node->children) {
        this->buildSymbolTableAndSigniture(i);
    }
}


void Processor::buildParams(SymbolNode* node) {
    if (node->rule == this->declarationsRule) {
        string type = declarationType(node);
        string name = node->children.back()->lex;
        bool variableDuplicate = this->table.variableExist(this->currentProcedure, name);
        if (variableDuplicate) {
            throw ParseError("ERROR: Duplicate Variable Name in Same Procedure!");
        }
        // if not duplicate, append it to current procedure symbol table
        Variable temp;
        temp.type = type;
        temp.name = name;
        this->table.table.find(this->currentProcedure)->second.emplace_back(temp);

        // append to function signiture
        auto it = this->signiture.signiture.find(this->currentProcedure);
        it->second.emplace_back(type);
    }
    for (auto i : node->children) {
        buildParams(i);
    }
}

void Processor::printSymbolTableAndSigniture() {
    // for each function
    for (auto it : this->signiture.signiture) {

        // print function signiture
        cerr << it.first << ":";
        for (auto type : it.second) {
            cerr << " " << type;
        }
        cerr << endl;
        auto itt = this->table.table.find(it.first);
        for (auto v : itt->second)
        {
            cerr << v.name << " " << v.type << endl;
        }
    }
}

void Processor::destroyParseTree(SymbolNode* node) {
    for (auto i : node->children) {
        destroyParseTree(i);
    }
    delete(node);
}

string declarationType(SymbolNode* node) {
    if (node->children.front()->children.size() == 1) {
        return "int";
    } else {
        return "int*";
    }
}

vector<string> splitRule(string rule) {
    vector<string> result;
    istringstream ss(rule, istringstream::in);
    string word;
    while(ss >> word) {
        result.emplace_back(word);
    }
    return result;
}