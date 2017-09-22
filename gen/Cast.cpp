//
// Created by wax on 6/27/17.
//

#include <asmjit/asmjit.h>
#include "ast/type/PrimitiveType.h"
#include "ast/Expr.h"
#include "Gen.h"

using namespace asmjit;

/**
 * "Cast" a register to type.
 */
X86Gp registerCast(X86Compiler& c, X86Gp reg, DType& regType, DType& targetType) {
    if (regType.byteSize() >= targetType.byteSize()) {
        // Target type smaller than expression type, no extend required.
        switch (targetType.byteSize()) {
            case 1: return reg.r8();
            case 2: return reg.r16();
            case 4: return reg.r32();
            case 8: return reg.r64();
            default:
                targetType.dump();
                assert(false);
        }
    } else {
        // TODO: verify that these widening moves fully works as expected.
        X86Gp result = Generate::typedRegister(c, targetType)[0];

        if (targetType.type.primitive == DPrimitiveKind::INTEGER) {
            c.movsx(result, reg);
        } else if (targetType.type.primitive == DPrimitiveKind::NATURAL) {
            c.movzx(result, reg);
        }

        return result;
    }
}

/**
 * Cast expression to primitive.
 * Tuples can be casted to primitives, returning a primitive containing the tuples first value.
 */
X86Gp primitiveCast(X86Compiler& c, Expr* expr, DType& type) {
    std::cout << "Generating primitive cast" << std::endl;
    assert(expr->type.isPrimitive() || expr->type.isTuple());
    X86Gp e = expr->generate(c)[0];
    return registerCast(c, e, expr->type, type);
}

/**
 * Cast expression to tuple.
 */
Regs tupleCast(X86Compiler& c, Expr* expr, DType& type) {
    assert(expr->type.isTuple());
    assert(expr->type.type.tuple->size() >= type.type.tuple->size());

    int valueCount = (int) type.type.tuple->size();

    expr->dump(); std::cout << std::endl;
    Regs e = expr->generate(c);
    std::cout << "e.size: " << e.size() << " valueCount: " << valueCount << std::endl;
    Regs result = Regs();
    for (int i = 0; i < valueCount; i++) {
        DType regType = (*expr->type.type.tuple)[i];
        DType targetType = (*type.type.tuple)[i];

        X86Gp castedReg = registerCast(c, e[i], regType, targetType);
        result.push_back(castedReg);
    }

    std::cout << "Tuple cast completed: " << result.size() << std::endl;

    return result;
}


namespace Generate {
    Regs cast(X86Compiler& c, Expr* expr, DType& type) {
        Regs result;
        if (type == expr->type)
            result = expr->generate(c);
        else if (type.isPrimitive())
            result = { primitiveCast(c, expr, type) };
        else if (type.isTuple())
            result = tupleCast(c, expr, type);
        else
            assert(false);

        return result;
    }
}
