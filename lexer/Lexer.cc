#include "Lexer.hh"

#include <assert.h>

#include "Token.hh"

Lexer::Lexer(std::string source) {
    this->source = source;
    this->index = 0;
    this->row = 0;
    this->col = 0;
}


TokenQueue Lexer::readAll() {
    int tokenIndex = 0;
    std::vector<Token> tokens = std::vector<Token>();

    readComment();
    while (index < source.size()) {
	Token t = read();
	t.index = tokenIndex++;
	tokens.push_back(t);
    }


    return TokenQueue(tokens);
}


Token Lexer::read() {
    Token t;
    t.row = row;
    t.col = col;
    t.index = 0;
	
    char c = source[index];
    if (isalpha(c))
	readAlpha(t);
    else if(isdigit(c))
	readNum(t);
    else if (c == '"')
	readString(t);
    else
	readSymbol(t);

    readComment();

    return t;
}


void Lexer::readWhitespace() {
    char c = source[index];
    while (isspace(c)) {
	if (c == '\n') {
	    row++;
	    col = 0;
	} else if(isprint(c)) {
	    col++;
	}
	
	c = source[++index];
    }
}

void Lexer::readComment() {
    readWhitespace();
    
    char c = source[index];
    while (c == '#') {
	while (c != '\n' && index < source.length() + 1) {
	    c = source[++index];
	}
	
	readWhitespace();
	c = source[index];
    }
    
    row++;
    col = 0;
}

void Lexer::readAlpha(Token &token) {
    token.type = IDENTIFIER;

    unsigned int startIndex = index;

    char c = source[index];
    while (isalpha(c) || (startIndex != index && (isdigit(c) || c == '_'))) {
	col++;
	c = source[++index];
    }

    token.value = source.substr(startIndex, index - startIndex);
}

void Lexer::readNum(Token &token) {
    token.type = NUMBER;

    unsigned int oldIndex = index;

    char c = source[index];
    while (isdigit(c)) {
	col++;
	c = source[++index];
    }

    token.value = source.substr(oldIndex, index - oldIndex);
}

void Lexer::readString(Token& token) {
    token.type = STRING;

    unsigned int oldIndex = index;

    index++;
    col++;
    
    char c = source[index];
    while (c != '"') {
	col++;
	c = source[++index];
    }

    index++;
    col++;
    
    token.value = source.substr(oldIndex, index - oldIndex);
}

void Lexer::readSymbol(Token &token) {
    unsigned int oldIndex = index;

    std::string longest;
    std::string current = "";

    do {
	if (index > source.size()) break;
	longest = current;
	index++;
	col++;

	current = source.substr(oldIndex, index - oldIndex);
    } while (valueToTokenType.find(current) != valueToTokenType.end());
    index--;

    if (index == oldIndex) { // FIXME will assert if unknown symbol is found. Replace with suitable error-code/exception.
	std::cout << "[" << token.row << ":" << token.col << "] Unknown symbol " << "'" << source[index] << "':" << (int)source[index] << std::endl;
	std::cout << index << " of " << source.size() - 1 << std::endl;
	assert(false);
    }

    token.value = longest;
    token.type = valueToTokenType.at(token.value);
}




