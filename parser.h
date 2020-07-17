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
#include <string>
#include <vector>
#include <functional>
#include "lexer.h"

class Parser {
    
    std::map<std::string, std::vector<std::string>> first = {
        { "Rat20SU", { "$$" } },
        { "OptDeclList", { "integer", "boolean" } },
        { "DeclList", { "integer", "boolean" } },
        { "Declaration", { "integer", "boolean" }},
        { "Qualifier", { "integer", "boolean" }},
        { "StatementList", { "{", "if", "put", "get", "while" /* , id */ }},
        { "Statement", { "{", "if", "put", "get", "while" /* , id */ }},
        { "Compound", { "{" }},
        { "Assign", { /* id */ }},
        { "If", { "if" }},
        { "Otherwise", { "otherwise" }},
        { "Put", { "put" }},
        { "Get", { "get" }},
        { "While", { "while" }},
        { "Expression", { /* id, */ /* int, */ "-", "(", "true", "false" } },
        { "Term", { /* id, */ /* int, */ "-", "(", "true", "false" } },
        { "Factor", { /* id, */ /* int, */ "-", "(", "true", "false" } },
        { "Primary", { /* id, */ /* int, */ "(", "true", "false" } }
    };
    
    const std::function<void(void)> Rat20SU = [&]() {
        std::cout << "$$  <Opt Declaration List>    <Statement List> $$";
    };

    const std::function<void(void)> OptDeclList = [&]() {
        
    };

    const std::function<void(void)> DeclList = [&]() {
        
    };

    const std::function<void(void)> Decl = [&]() {
        
    };

    const std::function<void(void)> Qualifier = [&]() {
        
    };

    const std::function<void(void)> StatementList = [&]() {
        
    };

    const std::function<void(void)> Statement = [&]() {
        
    };

    const std::function<void(void)> Compound = [&]() {
        
    };

    const std::function<void(void)> Assign = [&]() {
        
    };

    const std::function<void(void)> If = [&]() {
        
    };

    const std::function<void(void)> Put = [&]() {
        
    };

    const std::function<void(void)> Get = [&]() {
        
    };

    const std::function<void(void)> While = [&]() {
        
    };

    const std::function<void(void)> Condition = [&]() {
        
    };

    const std::function<void(void)> Relop = [&]() {
        
    };

    const std::function<void(void)> Expression = [&]() {
        
    };

    const std::function<void(void)> Term = [&]() {
        
    };

    const std::function<void(void)> Factor = [&]() {
        
    };

    const std::function<void(void)> Primary = [&]() {
        
    };
    
public:
    struct OutputType {};
    
    Lexer lexer;
    
    Parser(std::istream &istream) : lexer(istream) {}
    Parser(Lexer &lexer) : lexer(lexer) {}
    
    OutputType operator()() {
        OutputType t;
        
        auto token = lexer();
        while (token.lexeme != "$$" && lexer) token = lexer();
        
        if (token.lexeme != "$$") {
            std::cout << "Error: Check syntax for missing $$.";
        } else {
            std::cout << token.lexeme;
        }
        
        return t;
    }
    
};


#endif /* parser_h */
