/**
 * @file Generator.cc
 * @author Simon Zhang (simon.zhang@uwaterloo.ca)
 * @brief 
 * @version 0.1
 * @date 2022-03-19
 * 
 * @copyright Copyright (c) 2022 belongs to Simon Zhang @ University of Waterloo
 * 
 */
#include "Generator.h"

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include "SymbolTable.h"
#include "Word.h"
#include "ParseError.h"
#include "ProcedureSigniture.h"

int offset(int paramNum, int index);
void push(string registerName);
void pop(string registerName);
int getOffset(int paramNum, int index, string curProcedure, string mainProcedureName);
string randomSuffix();

void Generator::generateCode(SymbolNode* node, SymbolTable& table, ProcedureSigniture& signiture) {
    // import libs
    cout << ".import new" << endl;
    cout << ".import delete" << endl;
    cout << ".import print" << endl;
    cout << "lis $4 ; $4 will always hold 4" << endl;
    cout << ".word 4"<< endl;
    cout << "lis $10 ; $10 will always hold address for print" << endl;
    cout << ".word print" << endl;
    cout << "lis $11 ; $11 will always hold 1" << endl;
    cout << ".word 1" << endl;
    // call init before any call
    vector<string> paramTypes = signiture.paramTypes(this->mainProcedureName);
        if (paramTypes.at(0) == this->pointerType) {
            cout << ".import init" << endl;
            push("$31");
            cout << "lis $5\n.word init\njalr $5" << endl;
            pop("$31");
        } else {
            push("$2");
            push("$31");
            cout << "lis $2\n.word 0" << endl;
            cout << ".import init" << endl;
            cout << "lis $5\n.word init\njalr $5" << endl;
            pop("$31");
            pop("$2");
        }
    cout << "beq $0, $0, " << this->procedurePrefix + this->mainProcedureName << endl;
    this->code(node, table, signiture);
}

void Generator::code(SymbolNode* node, SymbolTable& table, ProcedureSigniture& signiture) {

    // track current procedure name
    if (node->rule == this->mainProcedure || node->rule == this->newProcedure) {
        if (node->rule == this->mainProcedure) {
            this->currentProcedure = this->mainProcedureName;             
        } else {
            for (auto i : node->children) {
                if (i->symbol == "ID") {
                    this->currentProcedure = i->lex;
                    break;
                }
            }
        }
    }

    if (node->rule == this->mainProcedure) {
        cout << this->procedurePrefix << this->mainProcedureName << ":" << endl;
        cout << this->wainPrologue << endl;
        push("$1");
        push("$2");
        this->code(node->children.at(8), table, signiture);
        this->code(node->children.at(9), table, signiture);
        this->code(node->children.at(11), table, signiture);
        cout << this->wainEpilogue << endl;
        
    } 
    
    else if (node->rule == this->start) {
        code(node->children.at(1), table, signiture);
    }
    
    else if (node->rule == this->introMain) {
        code(node->children.front(), table, signiture);
    }

    else if (node->rule == this->expr1) {
        code(node->children.front(), table, signiture);
    }

    else if (node->rule == this->term1) {
        code(node->children.front(), table, signiture);
    }

    else if (node->rule == this->factor1) {
        code(node->children.front(), table, signiture);
    }

    else if (node->isTerminal && node->symbol == "ID") {
        // find offset
        int paramNum = signiture.paramNum(this->currentProcedure);
        int index = table.index(this->currentProcedure, node->lex);
        //TODO: cerr << index << endl;
        int off = offset(paramNum, index);
        if (this->currentProcedure == this->mainProcedureName) {
            // main procedure, params are local variables
            cout << "lw $3 , " << (-4) * (index - 1) << "( $29 )" << endl;
        }
        else {
            cout << "lw $3 , " << off << "( $29 )" << endl;
        }
        
    }

    else if (node->rule == this->factor4) {
        code(node->children.at(1), table, signiture);
    }

    // +
    else if (node->rule == this->expr2) {
        if (node->children.front()->type == this->intType && node->children.back()->type == this->intType) {
            code(node->children.front(), table, signiture);
            push("$3");
            code(node->children.back(), table, signiture);
            pop("$5");
            cout << "add $3, $5, $3" << endl;
        }
        else if (node->children.front()->type == this->pointerType && node->children.back()->type == this->intType) {
            code(node->children.front(), table, signiture);
            push("$3");
            code(node->children.back(), table, signiture);
            cout << "mult $3 , $4 ; $4 always has the value 4" << endl;
            cout << "mflo $3" << endl;
            pop("$5");
            cout << "add $3 , $5 , $3" << endl;
        }
        else if (node->children.front()->type == this->intType && node->children.back()->type == this->pointerType) {
            code(node->children.front(), table, signiture);
            cout << "mult $3 , $4 ; $4 always has the value 4" << endl;
            cout << "mflo $3" << endl;
            push("$3");
            code(node->children.back(), table, signiture);
            pop("$5");
            cout << "add $3 , $5 , $3" << endl;
        }
        
    }

    // -
    else if (node->rule == this->expr3) {
        if (node->children.front()->type == this->intType && node->children.back()->type == this->intType) {
            code(node->children.front(), table, signiture);
            push("$3");
            code(node->children.back(), table, signiture);
            pop("$5");
            cout << "sub $3, $5, $3" << endl;
        }
        else if (node->children.front()->type == this->pointerType && node->children.back()->type == this->intType) {
            code(node->children.front(), table, signiture);
            push("$3");
            code(node->children.back(), table, signiture);
            cout << "mult $3 , $4 ; $4 always has the value 4" << endl;
            cout << "mflo $3" << endl;
            pop("$5");
            cout << "sub $3 , $5 , $3" << endl;
        }
        else if (node->children.front()->type == this->pointerType && node->children.back()->type == this->pointerType) {
            code(node->children.front(), table, signiture);
            push("$3");
            code(node->children.back(), table, signiture);
            pop("$5");
            cout << "sub $3 , $5 , $3" << endl;
            cout << "div $3 , $4" << endl;
            cout << "mflo $3" << endl;
        }
        
    }

    // *
    else if (node->rule == this->term2) {
        code(node->children.front(), table, signiture);
        push("$3");
        code(node->children.back(), table, signiture);
        pop("$5");
        cout << "mult $5, $3" << endl;
        cout << "mflo $3" << endl;
    }

    // /
    else if(node->rule == this->term3) {
        code(node->children.front(), table, signiture);
        push("$3");
        code(node->children.back(), table, signiture);
        pop("$5");
        cout << "div $5, $3" << endl;
        cout << "mflo $3" << endl;
    }

    // %
    else if (node->rule == this->term4) {
        code(node->children.front(), table, signiture);
        push("$3");
        code(node->children.back(), table, signiture);
        pop("$5");
        cout << "div $5, $3" << endl;
        cout << "mfhi $3" << endl;
    }

    // factor -> num
    else if (node->rule == this->factor2) {
        string num = node->children.front()->lex;
        cout << "lis $3" << endl;
        cout << ".word " << num << endl;
    }

    // print statements
    else if (node->rule == this->statement4) {
        code(node->children.front(), table, signiture);
        code(node->children.back(), table, signiture);
    }

    else if (node->rule == this->statement2) {
        push("$1");
        code(node->children.at(2), table, signiture);
        cout << "add $1 , $3 , $0" << endl;
        push("$31");
        cout << "lis $5" << endl;
        cout << ".word print" << endl;
        cout << "jalr $5" << endl;
        pop("$31");
        pop("$1");
    }

    // variable declarations and assignment statements
    else if (node->rule == this->dcl1) {
        code(node->children.front(), table, signiture);
        string num = node->children.at(3)->lex;
        cout << "lis $3" << endl;
        cout << ".word " << num << endl;
        push("$3");
    }

    else if (node->rule == this->statement1) {
        // lvalue -> id; variable assignment
        if (node->children.front()->rule == this->lvalue1) {
            code(node->children.at(2), table, signiture);
            int paramNum = signiture.paramNum(this->currentProcedure);
            int index = table.index(this->currentProcedure, node->children.front()->children.front()->lex);
            int off = offset(paramNum, index);
            if (this->currentProcedure == this->mainProcedureName)
            {
                // main procedure, params are local variables
                cout << "sw $3 , " << (-4) * (index - 1) << "( $29 )" << endl;
            }
            else
            {
                cout << "sw $3 , " << off << "( $29 )" << endl;
            }
        } else {
            // pointer assignment
            // case : lvalue → STAR factor
            this->code(node->children.at(2), table, signiture);
            push("$3");
            this->code(node->children.front()->children.back(), table, signiture);
            pop("$5");
            cout << "sw $5 , 0( $3 )" << endl;
        }
    }

    else if (node->rule == this->lvalue3) {
        code(node->children.at(1), table, signiture);
    }

    // while loops whose continuation condition is expressed with "<"

    // < 
    else if (node->rule == this->test3) {
        code(node->children.front(), table, signiture);
        push("$3");
        code(node->children.back(), table, signiture);
        pop("$5");
        if (node->children.front()->type == this->intType) {
            cout << "slt $3 ,$5 ,$3" << endl;
        }
        else {
            cout << "sltu $3 ,$5 ,$3" << endl;
        }
    }

    else if (node->rule == this->loop) {
        // generate random label name
        int randomNum;
        randomNum = rand() % 1000000;
        string beginLoop = this->loopLabel + to_string(randomNum);
        string endWhileLabel = this->endWhile + to_string(randomNum);
        cout << beginLoop << ":" << endl;
        code(node->children.at(2), table, signiture);
        cout << "beq $3 , $0 ," << endWhileLabel << endl;
        code(node->children.at(5), table, signiture);
        cout << "beq $0 , $0 , " << beginLoop << endl;
        cout << endWhileLabel << ":" << endl;
    }

    // other test cases
    // == 
    else if (node->rule == this->test1) {
        code(node->children.front(), table, signiture);
        push("$3");
        code(node->children.back(), table, signiture);
        pop("$5");
        if (node->children.front()->type == this->intType) {
            cout << "slt $6 ,$3 ,$5 ; $6 = $3 < $5" << endl;
            cout << "slt $7 ,$5 ,$3 ; $7 = $5 < $3" << endl;
        }
        else {
            cout << "sltu $6 ,$3 ,$5 ; $6 = $3 < $5" << endl;
            cout << "sltu $7 ,$5 ,$3 ; $7 = $5 < $3" << endl;
        }
        cout << "add $3 , $6 , $7" << endl;
        cout << "sub $3 , $11 , $3" << endl;
    }
    
    // != 
    else if (node->rule == this->test2) {
        code(node->children.front(), table, signiture);
        push("$3");
        code(node->children.back(), table, signiture);
        pop("$5");
        if (node->children.front()->type == this->intType) {
            cout << "slt $6 ,$3 ,$5 ; $6 = $3 < $5" << endl;
            cout << "slt $7 ,$5 ,$3 ; $7 = $5 < $3" << endl;
        }
        else {
            cout << "sltu $6 ,$3 ,$5 ; $6 = $3 < $5" << endl;
            cout << "sltu $7 ,$5 ,$3 ; $7 = $5 < $3" << endl;
        }
        cout << "add $3 , $6 , $7" << endl;
    }

    // <=
    else if (node->rule == this->test4) {
        code(node->children.back(), table, signiture);
        push("$3");
        code(node->children.front(), table, signiture);
        pop("$5");
        if (node->children.front()->type == this->intType) {
            cout << "slt $3 ,$5 ,$3" << endl;
        }
        else {
            cout << "sltu $3 ,$5 ,$3" << endl;
        }
        cout << "sub $3 , $11 , $3" << endl;
    }

    // >=
    else if (node->rule == this->test5) {
        code(node->children.front(), table, signiture);
        push("$3");
        code(node->children.back(), table, signiture);
        pop("$5");
        if (node->children.front()->type == this->intType) {
            cout << "slt $3 ,$5 ,$3" << endl;
        }
        else {
            cout << "sltu $3 ,$5 ,$3" << endl;
        }
        cout << "sub $3 , $11 , $3" << endl;
    }

    // >
    else if (node->rule == this->test6) {
        code(node->children.back(), table, signiture);
        push("$3");
        code(node->children.front(), table, signiture);
        pop("$5");
        if (node->children.front()->type == this->intType) {
            cout << "slt $3 ,$5 ,$3" << endl;
        }
        else {
            cout << "sltu $3 ,$5 ,$3" << endl;
        }
    }

    // if statements
    else if (node->rule == this->ifStatement) {
        int randomNum;
        randomNum = rand() % 1000000;
        string uqElseLabel = this->elseLabel + to_string(randomNum);
        string uqEndif = this->endifLabel + to_string(randomNum);
        code(node->children.at(2), table, signiture);
        cout << "beq $3 , $0 , " << uqElseLabel << endl;
        code(node->children.at(5), table, signiture);
        cout << "beq$0 ,$0 , " << uqEndif << endl;
        cout << uqElseLabel << ":" << endl;
        code(node->children.at(9), table, signiture);
        cout << uqEndif << ":" << endl;
    }

    /*************************************************************************************************/
    // pointers, but do not do dynamic memory allocation, 
    // pointer arithmetic, or pointer comparisons other than equality or inequality

    // initialize a null ptr
    else if (node->rule == this->dcl2) {
        code(node->children.front(), table, signiture);
        cout << "add $3 , $0 , $11" << endl;
        push("$3");
    }

    // initialize a null ptr
    else if (node->rule == this->factor3) {
        cout << "add $3 , $0 , $11 ; recall $11 is always 1" << endl;
    }

    else if (node->rule == this->factor5) {
        // case 1: factor→ AMP lvalue when lvalue → ID
        if (node->children.back()->rule == this->lvalue1) {
            string id = node->children.back()->children.front()->lex;
            int paramNum = signiture.paramNum(this->currentProcedure);
            int index = table.index(this->currentProcedure, id);
            int off = getOffset(paramNum, index, this->currentProcedure, this->mainProcedureName);
            cout << "lis $3" << endl;
            cout << ".word " << off << endl;
            cout << "add $3 , $3 , $29" << endl;
        }
        
        // case 2: factor1 → AMP lvalue and lvalue → STAR factor2
        else if (node->children.back()->rule == this->lvalue2) {
            this->code(node->children.back()->children.back(), table, signiture);
        }

    }

    else if (node->rule == this->factor6) {
        this->code(node->children.back(), table, signiture);
        cout << "lw $3 , 0( $3 ) ; $3 contains the loaded value" << endl;
    }

    else if (node->rule == this->lvalue2) {

    }

    // dynamic memory allocation, 
    // but do not do pointer arithmetic or pointer comparisons other than equality or inequality

    // new
    else if (node->rule == this->factor7) {
        this->code(node->children.at(3), table, signiture);
        cout << "add $1 , $3 , $0 ; new procedure expects value in $1" << endl;
        push("$31");
        cout << "lis $5" << endl;
        cout << ".word new" << endl;
        cout << "jalr $5" << endl;
        pop("$31");
        cout << "bne $3 , $0 , 1 ; if call succeeded skip next instruction" << endl;
        cout << "add $3 , $11 , $0 ; set $3 to NULL if allocation fails" << endl;
    }

    // delete
    else if (node->rule == this->statement3) {
        string suffix = randomSuffix();
        this->code(node->children.at(3), table, signiture);
        cout << "beq $3 , $11 ," << this->skipDelete + suffix << endl;
        cout << "add $1 , $3 , $0 ; delete expects the address in $1" << endl;
        push("$31");
        cout << "lis $5" << endl;
        cout << ".word delete" << endl;
        cout << "jalr $5" << endl;
        pop("$31");
        cout << this->skipDelete + suffix << ":" << endl;
    }

    /********************** Procedure Calls **************************************/
    else if (node->rule == this->procedures1) {
        this->code(node->children.front(), table, signiture);
        this->code(node->children.back(), table, signiture);
    }

    else if (node->rule == this->newProcedure) {
        cout << this->procedurePrefix + node->children.at(1)->lex << ":" << endl;
        cout << "sub $29 , $30 , $4 ; assume caller saved old frame procedure" << endl;
        // push("$29");
        // push("$31");
        this->code(node->children.at(6), table, signiture);
        this->code(node->children.at(7), table, signiture);
        this->code(node->children.at(9), table, signiture);
        // pop("$31");
        // pop("$29");
        cout << "add $30 ,$29 ,$4 ;reset stackframe" << endl;
        cout << "jr $31" << endl;
    }

    else if (node->rule == this->factor8) {
        push("$29");
        push("$31");
        cout << "lis $5" << endl;
        cout << ".word " << this->procedurePrefix + node->children.front()->lex << endl;
        cout << "jalr $5" << endl;
        pop("$31");
        pop("$29");
    }

    else if (node->rule == this->noParam) {

    }

    // procedure calls that have parameters
    else if (node->rule == this->factor9) {
        int paramNum = signiture.paramNum(node->children.front()->lex);
        push("$29");
        push("$31");
        this->code(node->children.at(2), table, signiture);
        cout << "lis $5" << endl;
        cout << ".word " << this->procedurePrefix + node->children.front()->lex << endl;
        cout << "jalr $5" << endl;
        // pop
        for (int i = 0; i < paramNum; i++) {
            pop("$7");
        }
        pop("$31");
        pop("$29");
    }

    else if (node->rule == this->arglist1) {
        this->code(node->children.front(), table, signiture);
        push("$3");
    }

    else if (node->rule == this->arglist2) {
        this->code(node->children.front(), table, signiture);
        push("$3");
        this->code(node->children.back(), table, signiture);
    }

}


/********************************* Helper functions **************************************************/

void push(string registerName) {
    cout << "sw " << registerName << ", -4($30)" << endl;
    cout << "sub $30, $30, $4" << endl;
}

void pop(string registerName) {
    cout << "add $30 , $30 , $4" << endl;
    cout << "lw " << registerName << ", -4( $30 )" << endl;
}

int offset(int paramNum, int index) {
    if (index <= paramNum) {
        return 4*(paramNum - index + 1);
    } else {
        index = index  - paramNum;
        return (-4) * (index - 1);
    }
}

int getOffset(int paramNum, int index, string curProcedure, string mainProcedureName) {
    if (curProcedure == mainProcedureName) {
       return (-4) * (index - 1);
    }
    else {
        return offset(paramNum, index);
    }
}

string randomSuffix() {
    int randomNum = rand() % 1000000;
    string result = to_string(randomNum);
    return result;
}