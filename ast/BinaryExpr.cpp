//
// Created by wax on 12/15/16.
//

#include <gen/Gen.h>
#include "util/PrettyPrint.h"
#include "BinaryExpr.h"

BinaryExpr::BinaryExpr(Expr* left, Operator* op, Expr* right) {
    this->left = left;
    this->op = op;
    this->right = right;
}

void BinaryExpr::analyze(Scope *scope) {
    left->analyze(scope);
    right->analyze(scope);
}

void BinaryExpr::dump(size_t indent) {
    std::cout << "(";

    left->dump(indent + 1);
    std::cout << " ";
    op->dump(indent + 1);
    std::cout << " ";
    right->dump(indent + 1);

    std::cout << ")";
}

X86GpVar* BinaryExpr::generate(X86Compiler &c) {
    return Generate::expression(c, this);
}

bool BinaryExpr::equals(const Node &other) const {
    const BinaryExpr* o = dynamic_cast<const BinaryExpr*>(&other);
    if (o == nullptr) return false;
    return *o->left == *left && *o->op == *op && *o->right == *right;
}
