//
// Created by wax on 12/16/16.
//

#ifndef DIG_PLNSTMT_H
#define DIG_PLNSTMT_H


#include "Stmt.h"
#include "Expr.h"

class PlnStmt : public Stmt {
public:
    Expr* expression;

    PlnStmt(Expr* expression);

    void analyze(Scope* scope);
    void dump(size_t indent);
};


#endif //DIG_PLNSTMT_H