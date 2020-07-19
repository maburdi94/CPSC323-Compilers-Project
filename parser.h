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
    std::stack<std::string> path;
    
    Parser(std::istream &istream) : lexer(istream) {}
    Parser(Lexer &lexer) : lexer(lexer) {}
    
    bool isidentifier(std::string s) {
        for (auto c : s) if (!(isalpha(c) || c == '_')) return false;
        return true;
    }
    
    void Rat20SU() {
        token = lexer();
        std::cout << token; // $$
            
        std::cout << "$$ <Opt Declaration List>  <Statement List> $$" << std::endl;
            
        OptDeclList();
        StatementList();
        
        token = lexer();
        std::cout << token; // $$
    }
    
    
    void OptDeclList() {
        if (std::find(first["DeclList"].begin(),first["DeclList"].end(), token.lexeme) != first["DeclList"].end()) {
            
            std::cout << "<Opt Declaration List>  ->  <Declaration List>" << std::endl;
            
            DeclList();
        } else {
            std::cout << "<Opt Declaration List>  ->  <Empty>" << std::endl;
            
            Empty();
        }
        
    }
    
    
    void DeclList() {
        /*
            Push the routes on a stack because we don't know
            which route until we get the next token.
         */
        
        Declaration();
        
        std::cout << "<Declaration List>  ->  <Declaration> ;";
        
        token = lexer();
        std::cout << token; // ;
        
        auto match = std::find(first["Declaration"].begin(), first["Declaration"].end(), token.lexeme);
        
        if (match != first["Declaration"].end()) {
            std::cout << " <Declaration List>" << std::endl;
            
            DeclList();
        }
        
    }
    
    void Declaration() {
        token = lexer();
        std::cout << token; // qualifier
        
        token = lexer();
        std::cout << token; // identifier
        
        std::cout << "<Declaration>  ->  <Qualifier> <Identifier>" << std::endl;
    }
    
    void Qualifier();
    
    void StatementList() {
        while (lexer && Statement());
    }
    
    bool Statement() {
        token = lexer();
        std::cout << token;
        
        if (token.lexeme == "{") Compound();
        else if (token.lexeme == "if") If();
        else if (token.lexeme == "put") Put();
        else if (token.lexeme == "get") Get();
        else if (token.lexeme == "while") While();
        else if (isidentifier(token.lexeme)) Assign();
        else if (token.lexeme == "$$") return false;
        
        return true;
    }
    
    void Compound() {
        std::cout << "{  <Statement List>  }" << std::endl;
    }
    void Assign() {
        std::cout << "<Identifier> = <Expression> ;" << std::endl;
    }
    void If() {
        std::cout << "if  ( <Condition>  ) <Statement> <Otherwise> fi" << std::endl;
    }
    void Put() {
        std::cout << "put ( <identifier> );" << std::endl;
    }
    void Get() {
        std::cout << "get ( <Identifier> );" << std::endl;
    }
    void While() {
        std::cout << "while ( <Condition>  ) <Statement>" << std::endl;
    }
    void Condition() {
        
    }
    void Relop() {
        
    }
    void Expression() {
        
    }
    void Term() {}
    void Factor() {}
    void Primary() {}
    void Empty() {}
    
    void operator()() {
        Rat20SU();
    }
    
};


#endif /* parser_h */
