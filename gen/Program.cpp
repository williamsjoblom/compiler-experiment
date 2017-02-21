//
// Created by wax on 1/23/17.
//

#include <iostream>

#include "jit/JitContext.h"
#include "ast/Unit.h"
#include "config.h"
#include "Gen.h"

namespace Generate {

    void entry(X86Compiler& c, Unit* unit);
    void functions(JitRuntime* runtime, Unit* unit);

    // Rename function to Generate::unit.
    ProgramType program(JitRuntime* runtime, Unit* unit) {
        X86Assembler a(runtime);
        X86Compiler c(&a);

        StringLogger logger;
        a.setLogger(&logger);

        JitContext::handles = new void*[unit->functions.size()];
        Generate::functions(runtime, unit);

        Generate::entry(c, unit);

        c.finalize();

        //std::cout << "Generated program:" << std::endl << logger.getString();

        ProgramType func = asmjit_cast<ProgramType>(a.make());
        return func;
    }

    void entry(X86Compiler& c, Unit* unit) {
        c.addFunc(FuncBuilder0<int>(kCallConvHost));

        unit->statements->generate(c);

        c.endFunc();
    }

    void functions(JitRuntime* runtime, Unit* unit) {
        int index = 0;

        for (FunctionDecl* func : unit->functions) {
            func->bHandleIndex = index;
            void* ptr = Generate::function(runtime, func);
            JitContext::handles[index] = ptr;
            index++;
        }
    }
}