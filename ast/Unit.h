//
// Created by wax on 1/24/17.
//

#ifndef DIG_UNIT_H
#define DIG_UNIT_H

#include "BlockStmt.h"
#include "FunctionDecl.h"

class Unit : public Node {
public:
    BlockStmt* statements;
    std::vector<FunctionDecl*> functions;

    Unit(std::vector<Stmt*> statements, std::vector<FunctionDecl*> functions);
    ~Unit();

    void analyze(Scope* scope);
    virtual bool equals(const Node& other) const;
    void dump(size_t indent);

    unsigned int functionCount();
    std::string symbolFromPtr(void* ptr);

private:
};


#endif //DIG_UNIT_H
