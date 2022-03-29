/**
 * @file wlp4gen.cc
 * @author Simon Zhang (simon.zhang@uwaterloo.ca)
 * @brief 
 * @version 0.1
 * @date 2022-03-15
 * 
 * @copyright Copyright (c) 2022 belongs to Simon Zhang @ University of Waterloo
 * 
 */

#include <iostream>
#include <vector>
#include "Processor.h"
#include "ParseError.h"
#include "TypeChecker.h"
#include "Generator.h"
using namespace std;



int main() {
    Processor p = Processor();
    TypeChecker t = TypeChecker();
    Generator g = Generator();
    try {
        p.initializeParseTree();
        p.buildParseTree(p.parseTree);
        p.buildSymbolTableAndSigniture(p.parseTree);
        // p.printSymbolTableAndSigniture();
        t.check(p.parseTree, p.table, p.signiture);
        g.generateCode(p.parseTree, p.table, p.signiture);
    } catch (ParseError& perr) {
        cerr << perr.what() << endl;
    }
    p.destroyParseTree(p.parseTree);
    
}