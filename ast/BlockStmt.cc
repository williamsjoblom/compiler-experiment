#include <vector>

#include "BlockStmt.hh"
#include "util/PrettyPrint.hh"


BlockStmt::BlockStmt(std::vector<Stmt*> statements) : statements{statements} { }


BlockStmt::~BlockStmt() {
    for (Stmt* statement : statements) delete statement;
}


void BlockStmt::analyze(Scope* scope) {
    Scope* innerScope = new Scope(scope);
    for (Stmt* stmt : statements) {
        stmt->analyze(innerScope);
    }
}


void BlockStmt::dump(size_t indent) {
    printIndent(indent);
    std::cout << "{" << std::endl;

    for (Stmt* stmt : statements) {
        stmt->dump(indent + 1);
    }

    printIndent(indent);
    std::cout << "}" << std::endl;
}


void BlockStmt::generate(TACFun* env) {
    for (Stmt* stmt : statements)
	stmt->generate(env);
}


bool BlockStmt::equals(const Node &other) const {
    const BlockStmt* o { dynamic_cast<const BlockStmt*>(&other) };
    if (o == nullptr) return false;

    if (o->statements.size() != statements.size()) return false;

    for (unsigned int i { 0 }; i < statements.size(); i++) {
	Stmt* statement { statements[i] };
	Stmt* otherStatement { o->statements[i] };

	if (*statement != *otherStatement) return false;
    }

    return true;
}
