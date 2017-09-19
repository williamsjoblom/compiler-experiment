//
// Created by wax on 2/22/17.
//

#include <ast/type/PrimitiveType.h>
#include <ast/type/DType.h>
#include "TypeUtil.h"
#include "ast/type/TypeVisitor.h"
#include "SemanticError.h"
#include "ast/IntegerLiteral.h"

/**
 * Type result visitor.
 * Used to determine the resulting type of a binary expression.
 */
class TypeResultVisitor : TypeVisitor {
public:
    const Type* resultingType(const Type* type, OperatorSymbol op, const Type* other);

private:
    const Type* result;

    const Type* type;
    OperatorSymbol op;
    const Type* other;

    void visit(const PrimitiveType* type) override;
    void visit(const TupleType* type) override;
};

const Type* TypeResultVisitor::resultingType(const Type* type, OperatorSymbol op, const  Type* other)  {
    this->op = op;
    this->other = other;

    this->result = nullptr;

    type->accept(this);
    return this->result;
}

void TypeResultVisitor::visit(const PrimitiveType* type) {
    const PrimitiveType* other = dynamic_cast<const PrimitiveType*>(this->other);
    if (other == nullptr)
        // TODO: Make this message prettier (ex. "No type conversion for i32->(i32, i32)")
        semanticError("Bad type conversion");

    PrimitiveKind kind;
    int size;

    if (type->kind == PrimitiveKind::NATURAL && other->kind == PrimitiveKind::NATURAL) {
        kind = PrimitiveKind::NATURAL;
    } else {
        kind = PrimitiveKind::INTEGER;
    }

    size = std::max(type->byteSize(), other->byteSize());
    result = new PrimitiveType(kind, size);
}

void TypeResultVisitor::visit(const TupleType *type) {

}

const Type* resultingType(const Type* left, Operator op, const Type* right) {
    // TODO: Maybe check for operator associativity?
    // if left associative:
    TypeResultVisitor resultChecker;
    return resultChecker.resultingType(left, op.symbol, right);
    // if right associative:
    // return resultChecker.resultingType(right, op.symbol, left);
}





const DType resultingPrimitive(const DType& left, Operator op, const DType& right) {
    assert(left.isPrimitive() && right.isPrimitive());

    if (left.type.primitive == DPrimitiveKind::NIL || right.type.primitive == DPrimitiveKind::NIL)
        semanticError("Nil types cannot be directly used directly");

    int sz = std::max(left.byteSize(), right.byteSize());
    DPrimitiveKind kind;
    if (left.type.primitive == DPrimitiveKind::NATURAL && right.type.primitive == DPrimitiveKind::NATURAL) {
        kind = DPrimitiveKind::NATURAL;
    } else {
        kind = DPrimitiveKind::INTEGER;
    }

    return DType(kind, sz);
}


const DType resultingTuplePrimitive(const Expr* left, Operator op, const Expr* right) {
    if (op.symbol == OperatorSymbol::DOT &&
	right->type.type.primitive == DPrimitiveKind::INTEGER) {
	
	const IntegerLiteral* literal = dynamic_cast<const IntegerLiteral*>(right);
	if (literal == nullptr) semanticError("Tuples are only indexable by integer literals");

	int i = literal->value;
	std::vector<DType>* tupleTypes = left->type.type.tuple;

	if (i >= tupleTypes->size()) semanticError("Tuple index out of bounds");

	return tupleTypes->at(i);
    }

    semanticError("Bad type conversion");
}

const DType resultingType(Expr* left, Operator op, Expr* right) {
    if (left->type.isPrimitive() && right->type.isPrimitive()) return resultingPrimitive(left->type, op, right->type);
    if (left->type.isTuple() && right->type.isPrimitive()) return resultingTuplePrimitive(left, op, right);
	
    semanticError("Bad type conversion");
}
