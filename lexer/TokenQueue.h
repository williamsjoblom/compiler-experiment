//
// Created by wax on 12/13/16.
//

#ifndef DIG_TOKENQUEUE_H
#define DIG_TOKENQUEUE_H


#include <vector>
#include "Token.h"


class TokenQueue {
public:
    TokenQueue(std::vector<Token>);
    ~TokenQueue();
    Token pop();
    Token peek() const;
    Token lookahead(int distance=1) const;
    void rewind(int distance);
    void rewindTo(Token t);
    Token top() const;
    Token at(int i) const;

    Token expect(const TokenType type);
    bool eat(const TokenType type);
    bool eatIdentifier(const std::string value);
    bool empty() const;

    void dump() const;
private:
    int index;
    std::vector<Token> tokens;
};


#endif //DIG_TOKENQUEUE_H
