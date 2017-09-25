//
// Created by wax on 1/23/17.
//

#include <asmjit/asmjit.h>
#include <ast/VariableExpr.h>
#include <assert.h>
#include <ast/VariableDecl.h>
#include <ast/FunctionCall.h>
#include <ast/FunctionDecl.h>
#include <jit/JitContext.h>
#include <ast/IntegerLiteral.h>

#include "Gen.h"
#include "ast/BinaryExpr.h"
#include "ast/Literal.h"

using namespace asmjit;

namespace Generate {


    Regs expression(X86Compiler &c, IntegerLiteral* expr) {
        Regs regs = Generate::typedRegister(c, expr->type);
        assert(regs.size() == 1);
        c.mov(regs[0], Imm(expr->value));
        return regs;
    }

    Regs expression(X86Compiler &c, VariableExpr* expr) {
	return expr->declaration->bVar;
    }

    std::vector<X86Gp> expression(X86Compiler &c, FunctionCall* expr) {
        FunctionDecl* decl = expr->declaration;

        X86Gp handle = c.newGpd("handle");
        assert(decl->bHandleIndex != -1); // Handle index has not been assigned!
        c.mov(handle, imm_ptr(JitContext::handles + decl->bHandleIndex));

        std::vector<X86Gp> args;
        for (unsigned int i = 0; i < expr->arguments->size(); i++) {
            Regs a = expr->arguments->at(i)->generate(c);
            //assert(a.size() == 1);
	    for (X86Gp arg : a)
		args.push_back(arg);
        }

        CCFuncCall* call = c.call(x86::ptr(handle), decl->bCreatePrototype());

        Regs ret = Generate::typedRegister(c, decl->returnType);
        if (expr->type.isPrimitive() && !expr->type.isNilType()) {
            // Primitives uses calling convention...
            call->setRet(0, ret[0]);
        } else if (expr->type.isTuple()) {
            // ...while tuples are passed on the stack.

            // Compensate for newly popped base pointer.
            int stackOffset = x86::rbp.getSize() + sizeof(int);
	    
            for (int i = (int) ret.size() - 1; i >= 0; i--) {
                c.mov(ret[i], x86::ptr(x86::rsp, -(ret[i].getSize() + stackOffset)));
                stackOffset += ret[i].getSize();
            }
        }

        for (unsigned int i = 0; i < args.size(); i++) {
            X86Gp a = args.at(i);
            call->setArg(i, a);
        }

        return ret;
    }

    Regs expression(X86Compiler& c, UnaryExpr* expr) {
        assert(expr->type.isPrimitive());

        Regs regs = expr->expr->generate(c);
        assert(regs.size() == 1);
        X86Gp value = regs[0];
        X86Gp result = c.newInt32("unaryexp");

        c.mov(result, value);
        if (expr->op->symbol == OperatorSymbol::MINUS)
            c.neg(result);

        return { result };
    }

    Regs expression(X86Compiler& c, TupleExpr* expr) {
        Regs regs = Generate::typedRegister(c, expr->type);

	int actualIndex = 0;
	for (Expr* value : expr->values) {
	    Regs valueRegs = value->generate(c);
	    
	    for (X86Gp reg : valueRegs) {
		c.mov(regs[actualIndex++], reg);
	    }
	}
	
        return regs;
    }

}
