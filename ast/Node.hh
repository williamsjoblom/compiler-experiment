#pragma once

#include <iostream>
#include <assert.h>

#include "Analyzable.hh"

class Node : public Analyzable {
public:
    virtual ~Node() { }

    bool operator==(const Node& other) const { return equals(other); }
    bool operator!=(const Node& other) const { return !equals(other); }
    virtual bool equals(const Node& other) const { assert(false); }

    virtual void dump(size_t indent) = 0;
    virtual void dump() { dump(0); }
};
