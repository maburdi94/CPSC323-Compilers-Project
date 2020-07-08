//
//  main.cpp
//  Assignment 1
//
//  Created by Michael Burdi on 7/2/20.
//  Copyright © 2020 Michael Burdi. All rights reserved.
//

#include <iostream>
#include <fstream>

enum Token {NONE, IDENTIFIER, KEYWORD, SEPARATOR, OPERATOR, INTEGER, UNKNOWN};

std::ostream& operator<<(std::ostream& os, Token t)
{
    switch(t)
    {
        case IDENTIFIER: os << "IDENTIFIER";    break;
        case UNKNOWN:    os << "UNKNOWN";       break;
        case KEYWORD:    os << "KEYWORD";       break;
        case SEPARATOR:  os << "SEPARATOR";     break;
        case OPERATOR:   os << "OPERATOR";      break;
        case INTEGER:    os << "INTEGER";       break;
        default:         os.setstate(std::ios_base::failbit);
    }
    return os;
}

typedef struct {
    Token type = NONE;
    std::string lexeme;
} LexerOutputType;


const std::string keywords[] = {
    "while",
    "if",
    "fi",
    "otherwise",
    "get",
    "put",
    "true",
    "false",
    "integer",
    "boolean"
};


bool isoperator(char &c) {
    return c == '=' ||
    c == '+' ||
    c == '-' ||
    c == '*' ||
    c == '/' ||
    c == '<' ||
    c == '>';
}

bool isseparator(char &c) {
    return c == '(' ||
    c == ')' ||
    c == '{' ||
    c == '}' ||
    c == ';';
}

bool isunderscore(char &c) {
    return c == '_';
}

bool iskeyword(std::string &s) {
    size_t len = sizeof(keywords) / sizeof(keywords[0]);
    auto f = std::find(keywords, keywords + len, s);
    return f != (keywords + len);
}

LexerOutputType lexer(std::istream &is) {
    
    LexerOutputType t;
    
    char c;
    while (is.get(c)) {
        
        if (isspace(c)) {
            
            // Discard space. Epsilon. No action taken.
            
            if (t.type != NONE) {
                // Terminating
                break;
            }
            
        }
        else if (isalpha(c) || isunderscore(c)) {
            if (t.type == IDENTIFIER || t.type == UNKNOWN) {
                // Accept
                t.lexeme += c;
            }
            else if (t.type == NONE) {
                // Assume identifier
                t.type = IDENTIFIER;
                
                // Accept
                t.lexeme += c;
            }
            else { // OPERATOR, SEPARATOR, INTEGER
                is.putback(c);
                break;
            }
        }
        else if (isseparator(c)) {
            if (t.type == NONE) {
                t.type = SEPARATOR;
                
                // Accept
                t.lexeme += c;
            }
            else {
                is.putback(c);
                break;
            }
        }
        else if (isoperator(c)) {
            if (t.type == NONE) {
                t.type = OPERATOR;
                
                // Accept
                t.lexeme += c;
            }
            else {
                is.putback(c);
                break;
            }
        }
        else if (isnumber(c)) {
            if (t.type == NONE) {
                t.type = INTEGER;
                
                // Accept
                t.lexeme += c;
            }
            else if (t.type == IDENTIFIER) {
                t.type = UNKNOWN;
                
                // Accept
                t.lexeme += c;
            }
            else if (t.type == INTEGER || t.type == UNKNOWN) {
                // Accept
                t.lexeme += c;
            }
            else {
                is.putback(c);
                break;
            }
        }
    }
    
    
    // We assumed keyword, but now we check.
    // If not keyword, must be identifier.
    if (t.type == IDENTIFIER && iskeyword(t.lexeme)) {
        t.type = KEYWORD;
    }
    
    return t;
}



int main(int argc, const char * argv[]) {
    
    std::ifstream ifile;
    ifile.open("/Users/maburdi/Documents/Education/Cal State Fullerton/Summer 2020/CPSC 323 Compilers and Programming Languages/Project/test2.rat");
    
    while (!ifile.eof()) {
        auto t = lexer(ifile);
        std::cout << t.type << ": " << t.lexeme << std::endl;
    }
    
    ifile.close();
}
