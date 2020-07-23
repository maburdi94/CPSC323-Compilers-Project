
#ifndef lexer_h
#define lexer_h

#include <iostream>
#include <unordered_set>

class Lexer {
    std::istream *istream;
    
    const std::unordered_set<std::string> keywords = {
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
    
    // Keep track of the line number.
    unsigned long current_line = 1;
    
public:
    
    enum Token {NONE, IDENTIFIER, KEYWORD, SEPARATOR, OPERATOR, INTEGER, UNKNOWN};
    
    struct OutputType {
        Token type = NONE;
        std::string lexeme;
        unsigned long line = 0;

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
        return keywords.find(s) != keywords.end();
    }
    
    friend std::ostream& operator<<(std::ostream& os, Token &t);
    
    
    std::string getline() {
        std::streampos pos = istream->tellg();
        istream->seekg (0, istream->beg);
        
        unsigned long i = current_line;
        std::string line;
        
        while (i--) std::getline(*istream, line);
        
        istream->seekg(pos, istream->beg);
        
        return line;
    }
    
    
    void operator()(OutputType *token) {
        *token = this->operator()();
    }
    
    OutputType &&operator()() {
        OutputType t;
        
        // We need this to skip things like comments.
        bool skip = false;
        
        
        // Iterate char-by-char and remember last char.
        for (char c = 0, b = c; istream->get(c); b = c) {
            
            
            // Skip if this char and last char [* (comment)
            // Don't skip if this char and last char *]. (end of comment)
            skip = (!skip && (c == '*' && b == '['))
            || (skip && !(c == ']' && b == '*'));
            
            
            // If skip, loop (or EOF kills loop).
            // Only encounter *] will reset skip.
            if (skip) continue;
            
            // Check if c is space char
            if (isspace(c)) {
                
                // Increment line on \n char
                if (c == '\n') current_line++;
                
                // Terminate all other states on space
                if (t.type != NONE)
                    break;
            }
            // Check if c is letter
            else if (isalpha(c)) {
                // State is NONE. Go to IDENTIFIER
                if (t.type == NONE) {
                    t.type = IDENTIFIER;
                    t.lexeme = c;
                }
                // State is IDENTIFIER. Go to IDENTIFIER
                else if (t.type == IDENTIFIER) {
                    t.lexeme += c;
                }
                // State is INTEGER. Go to UNKNOWN
                else if (t.type == INTEGER) {
                    t.type = UNKNOWN;
                    t.lexeme += c;
                }
                // State is UNKNOWN. Go to UNKNOWN
                else if (t.type == UNKNOWN) {
                    t.lexeme += c;
                }
                // Terminate for all other states.
                else {
                    istream->putback(c);
                    break;
                }
            }
            // Check if c is a underscore
            else if (c == '_') {
                // State is NONE. Go to UNKNOWN
                if (t.type == NONE) {
                    t.type = UNKNOWN;
                    t.lexeme = c;
                }
                // State is IDENTIFIER. Go to IDENTIFIER
                else if (t.type == IDENTIFIER) {
                    t.lexeme += c;
                }
                // All other states go to UNKNOWN
                else {
                    t.type = UNKNOWN;
                    t.lexeme += c;
                }
            }
            // Check if c is a separator
            else if (isseparator(c)) {
                // State is NONE. Go to SEPARATOR
                if (t.type == NONE) {
                    t.type = SEPARATOR;
                    t.lexeme = c;
                }
                // Terminate for all other states.
                else {
                    istream->putback(c);
                    break;
                }
            }
            // Check if c is a operator
            else if (isoperator(c)) {
                // State is NONE. Go to OPERATOR
                if (t.type == NONE) {
                    t.type = OPERATOR;
                    t.lexeme = c;
                }
                // "Special" operator ==
                else if (c == '=' && t.lexeme == "=") {
                    t.lexeme += c;
                }
                // Terminate for all other states.
                else {
                    istream->putback(c);
                    break;
                }
            }
            // Check if c is a digit
            else if (isnumber(c)) {
                
                // State is NONE. Go to INTEGER
                if (t.type == NONE) {
                    t.type = INTEGER;
                    t.lexeme = c;
                }
                // State is IDENTIFIER. Go to UNKNOWN
                else if (t.type == IDENTIFIER) {
                    t.type = UNKNOWN;
                    t.lexeme += c;
                }
                // State is INTEGER. Go to INTEGER
                else if (t.type == INTEGER) {
                    t.lexeme += c;
                }
                // State is UNKNOWN. Go to UNKNOWN
                else if (t.type == UNKNOWN) {
                    t.lexeme += c;
                }
                // Terminate for all other states.
                else {
                    istream->putback(c);
                    break;
                }
            }
            // c is none of the above
            else {
                
                // UNKNOWN token
                t.type = UNKNOWN;
                t.lexeme += c;
                
                // "Special" UNKNOWN token is KEYWORD
                if (t.lexeme == "$$") {
                    t.type = KEYWORD;
                    break;
                }
                
                // Don't return comments. Reset.
                if (t.lexeme == "[]") {
                    t.type = NONE;
                    t.lexeme = "";
                }
            }
        }
        
        
        // We assumed IDENTIFIER. Check if it is KEYWORD
        if (t.type == IDENTIFIER && iskeyword(t.lexeme)) {
            t.type = KEYWORD;
        }
        
        // Line number where token found.
        t.line = current_line;
        
        return std::move(t);
    }
};

std::ostream & operator<<(std::ostream &os, Lexer::OutputType t) {
    os << t.type << " " << t.lexeme << std::endl;
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
