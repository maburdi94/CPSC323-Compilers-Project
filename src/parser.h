//
//  parser.h
//  Assignment 1
//
//  Created by Michael Burdi on 7/14/20.
//  Copyright © 2020 Michael Burdi. All rights reserved.
//

#ifndef parser_h
#define parser_h

#include <iostream>
#include <map>
#include <stack>
#include <algorithm>
#include <string>
#include <vector>
#include <functional>
#include <set>
#include "lexer.h"

class Parser {

    // possibly need to add more first and follow sets
    
    std::map<std::string, std::set<std::string> > first = {
        { "Rat20SU", { "$$" } },
        { "OptDeclList", { "integer", "boolean", "e" } },
        { "DeclList", { "integer", "boolean" } },
        { "Declaration", { "integer", "boolean" }},
        { "Qualifier", { "integer", "boolean" }},
        { "StatementList", { "{", "if", "put", "get", "while" /* id */ }},
        { "Statement", { "{", "if", "put", "get", "while" /* id */ }},
        { "Compound", { "{" }},
        { "Assign", { /* id */ }},
        { "If", { "if" }},
        { "Otherwise", { "otherwise" }},
        { "Put", { "put" }},
        { "Get", { "get" }},
        { "While", { "while" }},
        { "Condition", { "-", /* id */ /* int */ "(", "true", "false" } },
        { "Relop", { "==", ">", "<" } },
        { "Expression", { /* id, */ /* int, */ "-", "(", "true", "false" } },
        { "Expression Prime", { "+", "-", "e" } },
        { "Term", { /* id, */ /* int, */ "-", "(", "true", "false" } },
        { "Term Prime", { "*", "/", "e" } },
        { "Factor", { /* id, */ /* int, */ "-", "(", "true", "false" } },
        { "Primary", { /* id, */ /* int, */ "(", "true", "false" } },
        { "Empty", { "e" } }
    };

    std::map<std::string, std::set<std::string> > follow = {
        { "Rat20SU", { "$" } },
        { "OptDeclList", { "{", /* id */ "if", "put", "get", "while" } },
        { "DeclList", { "{", /* id */ "if", "put", "get", "while" } },
        { "Declaration", { ";" } },
        { "Qualifier", { /* id */ } },
        { "StatementList", { "}", "$$" } },
        { "Statement", { "{", "}", "$$", /* id */ "if", "put", "get", "while", "otherwise", "fi" } },
        { "Compound", { "{", "}", "$$", /* id */ "if", "put", "get", "while", "otherwise", "fi" } },
        { "Assign", { "{", "}", "$$", /* id */ "if", "put", "get", "while", "otherwise", "fi" } },
        { "If", { "{", "}", "$$", /* id */ "if", "put", "get", "while", "otherwise", "fi" } },
        { "Otherwise", { "fi" } },
        { "Put", { "{", "}", "$$", /* id */ "if", "put", "get", "while", "otherwise", "fi" } },
        { "Get", { "{", "}", "$$", /* id */ "if", "put", "get", "while", "otherwise", "fi" } },
        { "While", { "{", "}", "$$", /* id */ "if", "put", "get", "while", "otherwise", "fi" } },
        { "Condition", { ")" } },
        { "Relop", { "-", /* id */ /* int */ "(", "true", "false" } },
        { "Expression", { "-", /* id */ /* int */ "true", "false", "(", ")", ";" } },
        { "Expression Prime", { "-", /* id */ /* int */ "true", "false", "(", ")", ";" } },
        { "Term", { "+", "-", /* id */ /* int */ "true", "false", "(", ")", ";" } },
        { "Term Prime", { "+", "-", /* id */ /* int */ "true", "false", "(", ")", ";" } },
        { "Factor", { "*", "/", "+", "-", /* id */ /* int */ "true", "false", "(", ")", ";" } },
        { "Primary", { "*", "/", "+", "-", /* id */ /* int */ "true", "false", "(", ")", ";" } },
        { "Empty", { "{", /* id */ /* int */ "if", "put", "get", "while", "fi", "+", "-", "true", "false",
                "(", ")", ";" } }
    };
    
public:
    struct OutputType {};
    
    Lexer lexer;
    Lexer::OutputType token;
    std::stack<std::string> path;
    
    Parser(std::istream &istream) : lexer(istream) {}
    Parser(Lexer &lexer) : lexer(lexer) {}
    
    bool isidentifier(std::string s) {
        for (auto c : s) if (!(isalpha(c) || c == '_')) return false;
        return true;
    }

    /////////////////////////////
    //      Grammar Rules      //
    /////////////////////////////

    /* 
    R1     <Rat20SU>   ::=  $$  <Opt Declaration List>    <Statement List> $$
        first = {$$}
        follow = {$}		// as described by the youtube video

    */
    bool Rat20SU () {
        token = lexer();

        if (token.lexeme == "$$") {
            if ( OptDeclList() ) {
                if ( StatementList() ) {
                    token = lexer();
                    if (token.lexeme == "$$") {
                        std::cout << "<Rat20SU>   ::=  $$  <Opt Declaration List>    <Statement List> $$" << std::endl;
                        return true;
                    }
                }
            }
        }
        else if (token.lexeme == "$") {
            lexer.backUp(token);
            return true;
        }

        return false;
    }

    /*
    R2     <Opt Declaration List>  ::=  <Declaration List>   |    <Empty>
        first = {integer, boolean, e}
        follow = { {, <identifier>, if, put, get, while}

    */
    bool OptDeclList () {
        // if 

        return false;
    }

    bool DeclList () {
        return false;
    }

    bool Declaration () {
        return false;
    }

    bool Qualifier () {
        return false;
    }

    bool StatementList () {
        return false;
    }

    bool Statement () {
        return false;
    }

    bool Compound () {
        return false;
    }

    bool Assign () {
        return false;
    }

    bool If () {
        return false;
    }

    bool Otherwise () {
        return false;
    }

    bool Put () {
        return false;
    }

    bool Get () {
        return false;
    }

    bool While () {
        return false;
    }

    bool Condition () {
        return false;
    }

    bool Relop () {
        return false;
    }

    bool Expression () {
        return false;
    }

    bool ExpressionPrime () {
        return false;
    }

    bool Term () {
        return false;
    }

    bool TermPrime () {
        return false;
    }

    bool Factor () {
        return false;
    }

    bool Primary () {
        return false;
    }

    bool Empty () {
        return false;
    }
    
    void operator()() {
        Rat20SU();
    }
    
};


#endif /* parser_h */
