#ifndef DIG_TACOP_H
#define DIG_TACOP_H

#include <string>

//#include "TACFun.h"
#include "TACFunOp.h"
#include "TACType.h"
#include "ast/FunctionDecl.h"

class TACFun;

/**
 * Three address operand kind.
 */
enum TACOpKind {
    IMMEDIATE,
    FUNCTION,
    VARIABLE,
    LABEL,
};


union TACOpData {
    unsigned long immValue;
    int functionId;
    int labelId;
    int varId;
};

/**
 * Three address operand.
 */
class TACOp {
public:
    /**
     * Kind.
     */
    TACOpKind kind;


    /**
     * Type.
     */
    TACType type;

    /**
     * Data.
     * Constains immedate value, label id etc. depending on kind.
     */
    TACOpData data;

    /**
     * Ignore.
     * True if this operand is unused. Automatically set when using
     * default constructor.
     */
    bool ignore;

    
    /**
     * Default constructor.
     */
    TACOp(bool used=false);


    /**
     * Label constructor.
     */
    TACOp(TACLabel* label);


    /**
     * Variable constructor.
     */
    TACOp(TACVar* var);


    /**
     * Function constructor.
     */
    TACOp(FunctionDecl* fun);

    
    /**
     * To string.
     */
    std::string toS(TACFun* fun) const;
};

#endif
