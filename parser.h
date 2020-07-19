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
    
public:
    struct OutputType {};
    
    Lexer lexer;
    Lexer::OutputType token;
    
    Parser(std::istream &istream) : lexer(istream) {}
    Parser(Lexer &lexer) : lexer(lexer) {}
    
    void OptDeclList() {
        token = lexer();
        
        std::cout << token;
        
        auto match = std::find(first["DeclList"].begin(), first["DeclList"].end(), token);
        
        if (match != first["DeclList"].end()) {
            std::cout << "<Opt Declaration List>  ->  <Declaration List>" << std::endl;
            
            DeclList();
        } else {
            std::cout << "<Opt Declaration List>  ->  <Empty>" << std::endl;
        }
        
    }
    
    void StatementList() {
        
    }
    
    void Rat20SU() {
        token = lexer();
        
        std::cout << token;
        
        if (token.lexeme == "$$") {
            std::cout << "$$  <Opt Declaration List>    <Statement List> $$" << std::endl;
            
            OptDeclList();
            StatementList();
        } else {
            std::cout << "Error: Check syntax for missing $$.";
        }
    }
    void DeclList();
    void Decl();
    void Qualifier();
    void Statement();
    void Compound();
    void Assign();
    void If();
    void Put();
    void Get();
    void While();
    void Condition();
    void Relop();
    void Expression();
    void Term();
    void Factor();
    void Primary();
    
    OutputType operator()() {
        OutputType t;
        
        // Find start of Rat20SU code
        do token = lexer(); while (token.lexeme != "$$" && lexer);

        Rat20SU();
        
        return t;
    }
    
};


#endif /* parser_h */
