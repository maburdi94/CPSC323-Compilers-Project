//
//  lexer.h
//  Assignment 1
//
//  Created by Michael Burdi on 7/16/20.
//  Copyright © 2020 Michael Burdi. All rights reserved.
//

#ifndef lexer_h
#define lexer_h

#include <iostream>

class Lexer {
    std::istream *istream;
    
    const std::string keywords[10] = {
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
    
public:
    
    enum Token {NONE, IDENTIFIER, KEYWORD, SEPARATOR, OPERATOR, INTEGER, UNKNOWN};
    
    struct OutputType {
        Token type = NONE;
        std::string lexeme;
        unsigned long line = 0;
        int posNum;
        OutputType* next;
        
        friend std::ostream & operator<<(std::ostream &os, Lexer::OutputType t);
    };

    
    
    Lexer(std::istream &is) : istream(&is) {}
    
    bool isoperator(char &c) {
        return c == '+' ||
        c == '-' ||
        c == '*' ||
        c == '/' ||
        c == '<' ||
        c == '>' ||
        c == '=';
    }

    bool isseparator(char &c) {
        return c == '(' ||
        c == ')' ||
        c == '{' ||
        c == '}' ||
        c == ';';
    }

    bool iskeyword(std::string &s) {
        size_t len = sizeof(keywords) / sizeof(keywords[0]);
        auto f = std::find(keywords, keywords + len, s);
        return f != (keywords + len);
    }
    
    friend std::ostream& operator<<(std::ostream& os, Token &t);
    
    /* Allows the use of Lexer object in a conditional statement
     to test if it has finished analyzing the input. */
    operator bool() const {
        return istream->good();
    }


    void backUp() {

    }
    
    Lexer::OutputType operator()() {
        Lexer::OutputType t;
        
        static unsigned long line = 1;
        
        bool skip = false;
        for (char c = 0, b = c; istream->get(c); b = c) {
            
            skip = (!skip && (c == '*' && b == '['))
            || (skip && !(c == ']' && b == '*'));
            
            if (skip) continue;
            
            if (isspace(c)) {
                
                if (c == '\n') line++;
                
                if (t.type != NONE)
                    break;
            } else if (isalpha(c)) {
                if (t.type == NONE) {
                    t.type = IDENTIFIER;
                    t.lexeme = c;
                } else if (t.type == IDENTIFIER) {
                    t.lexeme += c;
                } else if (t.type == INTEGER) {
                    t.type = UNKNOWN;
                    t.lexeme += c;
                } else if (t.type == UNKNOWN) {
                    t.lexeme += c;
                } else {
                    istream->putback(c);
                    break;
                }
            } else if (c == '_') {
                if (t.type == NONE) {
                    t.type = UNKNOWN;
                    t.lexeme = c;
                } else if (t.type == IDENTIFIER) {
                    t.lexeme += c;
                } else {
                    t.type = UNKNOWN;
                    t.lexeme += c;
                }
            } else if (isseparator(c)) {
                if (t.type == NONE) {
                    t.type = SEPARATOR;
                    t.lexeme = c;
                } else {
                    istream->putback(c);
                    break;
                }
            } else if (isoperator(c)) {
                if (t.type == NONE) {
                    t.type = OPERATOR;
                    t.lexeme = c;
                } else if (c == '=' && t.lexeme == "=") {
                    t.lexeme += c;
                } else {
                    istream->putback(c);
                    break;
                }
            } else if (isnumber(c)) {
                if (t.type == NONE) {
                    t.type = INTEGER;
                    t.lexeme = c;
                } else if (t.type == IDENTIFIER) {
                    t.type = UNKNOWN;
                    t.lexeme += c;
                } else if (t.type == INTEGER) {
                    t.lexeme += c;
                } else if (t.type == UNKNOWN) {
                    t.lexeme += c;
                } else {
                    istream->putback(c);
                    break;
                }
            } else {
                t.type = UNKNOWN;
                t.lexeme += c;
                
                if (t.lexeme == "$$") {
                    t.type = KEYWORD;
                    break;
                }
                
                // Comment
                if (t.lexeme == "[]") {
                    t.type = NONE;
                    t.lexeme = "";
                }
            }
        }
        
        
        // We assumed keyword, but now we check.
        // If not keyword, must be identifier.
        if (t.type == IDENTIFIER && iskeyword(t.lexeme)) {
            t.type = KEYWORD;
        }
        
        t.line = line;
        
        return t;
    }
};

std::ostream & operator<<(std::ostream &os, Lexer::OutputType t) {
    os << t.line << ":" << t.type << " " << t.lexeme << std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, Lexer::Token &t)
{
    switch(t)
    {
        case Lexer::Token::IDENTIFIER: os << "IDENTIFIER";    break;
        case Lexer::Token::UNKNOWN:    os << "UNKNOWN";       break;
        case Lexer::Token::KEYWORD:    os << "KEYWORD";       break;
        case Lexer::Token::SEPARATOR:  os << "SEPARATOR";     break;
        case Lexer::Token::OPERATOR:   os << "OPERATOR";      break;
        case Lexer::Token::INTEGER:    os << "INTEGER";       break;
            
        default:         os.setstate(std::ios_base::failbit);
    }
    return os;
}

#endif /* lexer_h */
