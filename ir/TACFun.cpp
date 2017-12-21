#include "TACFun.h"

#include <iostream>
#include <sstream>

#include "TACFunOp.h"
#include "ast/FunctionDecl.h"
#include "util/Colors.h"
#include "TACProgram.h"


TACFun::TACFun(TACProgram* parent, int id) {
    this->parent = parent;
    
    identifier = "#entry";
    this->id = id;

    parameters = std::vector<TACVar*>();
    instr = std::vector<TAC*>();
    labels = std::vector<TACLabel*>();
    variables = std::vector<TACVar*>();
}


TACFun::TACFun(TACProgram* parent, int id, FunctionDecl* decl) : TACFun(parent, id) {
    identifier = decl->identifier;

    for (VariableDecl* param : *decl->parameters) {
	TACType t = TACType(param->type);
	TACVar* var = newParam(t);
	param->irVar = var;
    }
}


TACFun::~TACFun() {
    for (int i = 0; i < instr.size(); i++)
	delete instr[i];
    for (int i = 0; i < labels.size(); i++)
	delete labels[i];
    for (int i = 0; i < variables.size(); i++)
	delete variables[i];
}


TAC* TACFun::add(TACC opcode, TACOp s0, TACOp s1, TACOp d) {
    TAC* tac = new TAC(opcode, s0, s1, d);
    instr.push_back(tac);

    bindWaitingLabels(tac);
    
    return tac;
}


TACLabel* TACFun::newLabel(std::string name) {
    TACLabel* label = new TACLabel();
    labels.push_back(label);

    label->id = labels.size() - 1;
    label->tac = nullptr;
    
    if (name.empty()) {
	std::stringstream ss;
	ss << "L#" << label->id;
	label->name = ss.str();
    } else {
	label->name = name;
    }
    
    return label;
}


TACLabel* TACFun::label(int id) const {
    return labels[id];
}


void TACFun::bindLabel(TACLabel* label) {
    waitingLabels.push_back(label);
}


void TACFun::bindLabel(TACLabel* label, TAC* tac) {
    if (tac->label != nullptr) {
	*label = *tac->label;
	label = tac->label;
    } else {
	tac->label = label;
    }
    
    label->tac = tac;
}


TACVar* TACFun::newVar(TACType& type, std::string name) {
    TACVar* var = new TACVar();
    variables.push_back(var);

    var->id = variables.size() - 1;
    var->type = type;
    
    if (name.empty()) {
	std::stringstream ss;
	ss << "V#" << var->id;
	var->name = ss.str();
    } else {
	var->name = name;
    }

    return var;
}


TACVar* TACFun::newParam(TACType& type, std::string name) {
    TACVar* var = newVar(type, name);
    parameters.push_back(var);
    return var;
}


TACVar* TACFun::var(int id) {
    return variables[id];
}


TACOp TACFun::newImm(TACType& type, unsigned long value) {
    TACOp op(true);
    op.type = type;
    op.kind = TACOpKind::IMMEDIATE;
    op.data.immValue = value;

    return op;
}


void TACFun::dump() {
    std::cout << UNDL(<< BOLD(<< identifier <<) <<)
	      << ":" << std::endl;
    
    for (TAC* tac : instr) {
	tac->dump(this, 1);
	std::cout << std::endl;
    }

    for (TACLabel* label : waitingLabels)
	label->dump(1);
}


void TACFun::bindWaitingLabels(TAC* tac) {
    for (TACLabel* label : waitingLabels)
	bindLabel(label, tac);
    waitingLabels.clear();
}