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
#include <string>

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
        int size = 0;
        
        friend std::ostream & operator<<(std::ostream &os, Lexer::OutputType &t);
    };

    int lineNum;
    
    Lexer(std::istream &is) : istream(&is) {
        lineNum = 0;
    }
    
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

    bool isunderscore(char &c) {
        return c == '_';
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

    // when parser needs to backup a token because it's testing for the follow set,
    // it will ask this function to backup the pointer to the previous token
    void backUp(Lexer::OutputType token) {
        for (int i = 0; i < token.size + 1; i++) {
            istream->unget();
        }
    }
    
    Lexer::OutputType operator()() {
        Lexer::OutputType t;
        
        char c;
        while (istream->get(c)) {
            
            if (isspace(c)) {
                if (c == '\n') {
                    lineNum++;
                }

                // Discard space. Epsilon. No action taken.
                
                if (t.type != NONE) {
                    // Terminating
                    break;
                }
                
            }
            else if (c == '$') {
                t.type = UNKNOWN;
                t.lexeme += c;
                t.size += 1;
                
                if (istream->get(c) && c == '$') {
                    t.type = KEYWORD;
                    t.lexeme += c;
                    t.size += 1;
                } else {
                    istream->putback(c);
                }
                
                break;
            }
            else if (c == '[') {
                if (istream->get(c) && c == '*') {
                    // This is a comment
                    for (char b=c; istream->get(c) && istream; b=c) {
                        if (b=='*' && c==']') break;
                    }
                } else {
                    t.type = UNKNOWN;
                    
                    t.lexeme += c;
                    t.size += 1;
                }
            }
            else if (isalpha(c) || isunderscore(c)) {
                if (t.type == IDENTIFIER || t.type == UNKNOWN) {
                    // Accept
                    t.lexeme += c;
                    t.size += 1;
                }
                else if (t.type == NONE) {
                    // Assume identifier
                    t.type = IDENTIFIER;
                    
                    // Accept
                    t.lexeme += c;
                    t.size += 1;
                }
                else { // OPERATOR, SEPARATOR, INTEGER
                    istream->putback(c);
                    break;
                }
            }
            else if (isseparator(c)) {
                if (t.type == NONE) {
                    t.type = SEPARATOR;
                    
                    // Accept
                    t.lexeme += c;
                    t.size += 1;
                }
                else {
                    istream->putback(c);
                    break;
                }
            }
            else if (isoperator(c)) {
                if (t.type == NONE) {
                    t.type = OPERATOR;
                    
                    // Accept
                    t.lexeme += c;
                    t.size += 1;
                    
                    if (c == '=') {
                        if (istream->get(c) && c == '=') {
                            // Equality operator ==
                            t.lexeme += c;
                            t.size += 1;
                        } else {
                            istream->putback(c);
                        }
                    }
                }
                else {
                    istream->putback(c);
                    break;
                }
            }
            else if (isnumber(c)) {
                if (t.type == NONE) {
                    t.type = INTEGER;
                    
                    // Accept
                    t.lexeme += c;
                    t.size += 1;
                }
                else if (t.type == IDENTIFIER) {
                    t.type = UNKNOWN;
                    
                    // Accept
                    t.lexeme += c;
                    t.size += 1;
                }
                else if (t.type == INTEGER || t.type == UNKNOWN) {
                    // Accept
                    t.lexeme += c;
                    t.size += 1;
                }
                else {
                    istream->putback(c);
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
};

std::ostream & operator<<(std::ostream &os, Lexer::OutputType &t) {
    os << t.type << ": " << t.lexeme << std::endl;
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
