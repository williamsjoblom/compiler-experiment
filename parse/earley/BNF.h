#pragma once

#include <string>
#include <vector>

#include "lexer/Token.h"

/**
 * Forward declarations.
 */
struct BNFT;
struct BNFNT;


/**
 * Symbol.
 */
struct BNFSymbol {

    /**
     * True if this is a terminal symbol.
     */
    virtual bool terminal() { return false; }

    
    /**
     * True if this is a non terminal symbol.
     */
    virtual bool nonTerminal() { return false; }

    
    /**
     * Cast to terminal symbol.
     */
    BNFT* asTerminal();

    
    /**
     * Cast to non terminal symbol.
     */
    BNFNT* asNonTerminal();

    /**
     * To string.
     */
    virtual std::string toS() const = 0;
};


/**
 * Terminal symbol.
 */
struct BNFT : public BNFSymbol {
    /**
     * Type of terminal token.
     */
    TokenType type;
    
    /**
     * Value of terminal token, can be left empty to match
     * token regardless of value.
     */
    std::string value;

    
    /**
     * Constructor.
     */
    BNFT(TokenType type, std::string value="") : type(type), value(value)  { }

    
    /**
     * To string.
     */
    std::string toS() const override;
};


/**
 * Non terminal symbol.
 */
struct BNFNT : public BNFSymbol {
    /**
     * Non terminal symbol.
     */
    std::string symbol;

    
    /**
     * Constructor.
     */
    BNFNT(std::string symbol) : symbol(symbol) { }

    
    /**
     * To string.
     */
    std::string toS() const override;
};


/**
 * Production.
 */
struct BNFProduction {
    /**
     * Symbols.
     */
    std::vector<BNFSymbol*> symbols;

    
    /**
     * To string.
     */
    std::string toS() const;
};

    
/**
 * Production rule.
 */
struct BNFRule {
    std::string symbol;

    std::vector<BNFProduction> productions;
    
    std::string toS() const;
};


/**
 * Grammar.
 */
struct BNFGrammar {
    std::vector<BNFRule> rules;
};
