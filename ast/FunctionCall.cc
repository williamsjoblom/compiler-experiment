#include "FunctionCall.hh"

#include "util/PrettyPrint.hh"
#include "ir/TACOp.hh"
#include "genir/FunctionCall.hh"


FunctionCall::FunctionCall(std::string identifier, std::vector<Expr*>* arguments) : identifier{identifier},
                                                                                    arguments{arguments} { }


FunctionCall::~FunctionCall() {
    for (Expr* argument : *arguments) delete argument;
    delete arguments;
}


void FunctionCall::dump(size_t indent) {
    std::cout << identifier << "(";

    for (auto it { arguments->begin() }; it != arguments->end(); it++) {
        if (it != arguments->begin())
            std::cout << ", ";
        (*it)->dump();
    }

    std::cout << ")";
}


void FunctionCall::analyze(Scope* scope) {
    declaration = scope->resolveFunction(identifier);

    type = declaration->returnType;

    if (declaration->parameters->size() != arguments->size()) throw 1;

    for (Expr* argument : *arguments)
        argument->analyze(scope);
}


TACOp FunctionCall::generate(TACFun* fun) {
    return Generate::functionCall(fun, this);
}


bool FunctionCall::equals(const Node &other) const {
    const FunctionCall* o { dynamic_cast<const FunctionCall*>(&other) };
    if (o == nullptr) return false;

    if (o->arguments->size() != arguments->size()) return false;

    for (int i { 0 }; i < arguments->size(); i++) {
        Expr* argument { (*arguments)[i] };
        Expr* otherArgument { (*o->arguments)[i] };

        if (*argument != *otherArgument) return false;
    }

    return o->identifier == identifier;
}
