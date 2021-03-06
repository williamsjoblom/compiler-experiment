#include "DType.hh"

#include <assert.h>
#include <iostream>


DType::DType() { }


DType::DType(const DType &o) {
   copy(o);
}


DType::DType(DKind kind, int sz, bool ref) : kind{kind},
                                             sz{sz},
                                             label{""},
                                             ref{ref} { }


DType::DType(DPrimitiveKind primitiveKind, int sz, bool ref) : kind{DKind::PRIMITIVE},
                                                               sz{sz},
                                                               label{""},
                                                               ref{ref} {
    this->type.primitive = primitiveKind;
}


DType::DType(std::vector<DType>* tupleTypes, bool ref) : kind{DKind::TUPLE},
                                                         ref{ref},
                                                         label{""} {
    this->type.tuple = tupleTypes;

    sz = 0;
    assert(tupleTypes != nullptr);
    for (DType t : *tupleTypes) sz += t.byteSize();
}


DType::~DType() {
    if (isSeq()) delete type.seq;
}


int DType::byteSize() const {
    return sz;
}


int DType::stackSize() const {
    if (isTuple())
	return sizeof(void*);
    else
	return sz;
}


void DType::copy(const DType &o) {
    kind = o.kind;
    switch (kind) {
    case DKind::PRIMITIVE: type.primitive = o.type.primitive; break;
    case DKind::TUPLE: type.tuple = new std::vector<DType>(*o.type.tuple); break;
    case DKind::ARRAY: type.seq = new DType(*o.type.seq); break;
    }
    sz = o.sz;
    label = o.label;
    ref = o.ref;
}


bool DType::operator==(const DType &o) {
    if (kind != o.kind) return false;
    if (sz != o.sz) return false;
    if (ref != o.ref) return false;

    switch (kind) {
        case DKind::PRIMITIVE: return type.primitive == o.type.primitive;
        case DKind::TUPLE:
            for (int i { 0 }; i < type.tuple->size(); i++)
                if ((*type.tuple)[i] != (*o.type.tuple)[i])
                    return false;
            return true;
        case DKind::ARRAY:  return type.seq == o.type.seq;
    }

    assert(false);
}


void DType::dump() const {
    switch(kind) {
        case DKind::PRIMITIVE:
            std::cout << "";
            if (type.primitive == DPrimitiveKind::NATURAL)
                std::cout << "NAT";
            else if (type.primitive == DPrimitiveKind::INTEGER)
                std::cout << "INT";
            else if (type.primitive == DPrimitiveKind::NIL)
                std::cout << "NIL";

	    std::cout << sz;
            break;
        case DKind::TUPLE:
            std::cout << "(";
            for (DType t : *type.tuple) {
                t.dump();
                std::cout << " ";
            }
            std::cout << ")";
            break;
        default:
	    std::cout << "Dumping unknown DKind " << (int) kind << std::endl;
            assert(false);
    }
}
