//
// Created by wax on 12/16/16.
//

#include "gen/Gen.h"
#include "util/PrettyPrint.h"
#include "PlnStmt.h"
#include "semantic/SemanticError.h"
#include "genir/BuiltIn.h"


PlnStmt::PlnStmt(Expr* expression) {
    this->expression = expression;
}


PlnStmt::~PlnStmt() {
    delete expression;
}


void PlnStmt::analyze(Scope* scope) {
    expression->analyze(scope);
    if (!expression->type.isPrimitive())
	semanticError("pln function only takes primitive type arguments");
}


void PlnStmt::dump(size_t indent) {
    printIndent(indent);
    std::cout << "pln ";
    expression->dump();
    std::cout << std::endl;
}


bool PlnStmt::equals(const Node &other) const {
    const PlnStmt* o = dynamic_cast<const PlnStmt*>(&other);
    if (o == nullptr) return false;

    return *o->expression == *expression;
}


void PlnStmt::generate(X86Compiler& c) {
    Generate::pln(c, this);
}


void PlnStmt::generate(TACFun* fun) {
    Generate::pln(fun, this);
}
