//
//  main.cpp
//  Assignment 1
//
//  Created by Michael Burdi on 7/2/20.
//  Copyright © 2020 Michael Burdi. All rights reserved.
//

#include <iostream>
#include <sstream>

enum Token {IDENTIFIER, KEYWORD, SEPARATOR, OPERATOR, INTEGER};

typedef struct {
    Token type;
    std::string lexeme;
} LexerOutputType;



bool isoperator(char &c) {
    return c == '=' ||
    c == '+' ||
    c == '-' ||
    c == '*' ||
    c == '/';
}

bool isseparator(char &c) {
    return c == '(' ||
    c == ')' ||
    c == ';';
}

LexerOutputType &&lexer(std::stringstream &s) {
    
    LexerOutputType t;
    
    char c;
    while (s.get(c)) {
        
        // Consume input
        t.lexeme += c;
        
        if (isspace(c)) {
            
        /*  Space (non-accepting state)
            Transition => { epsilon } */
            
            // Discard space
            t.lexeme.pop_back();
        }
        
        if (isalpha(c)) {
            t.type = IDENTIFIER;
        }
        else if (isseparator(c)) {
            t.type = SEPARATOR;
        }
        else if (isoperator(c)) {
            t.type = OPERATOR;
        }
        else if (isnumber(c)) {
            t.type = INTEGER;
        }
    }
    
    return std::move(t);
}



int main(int argc, const char * argv[]) {
    
    std::string s = "INT    ax12    =   45;";
    std::stringstream is(s);
    
    lexer(is);
}
