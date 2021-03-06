#pragma once

#include "Literal.hh"

class IntegerLiteral : public Literal {
public:
    int value;

    IntegerLiteral(int value);

    TACOp generate(TACFun* env);
    bool equals(const Node& other) const;
    void dump(size_t indent);
};

