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
#include <set>
#include "lexer.h"

class Parser {
    
    std::map<std::string, std::set<std::string>> first = {
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
    
    Lexer lexer;
    Lexer::OutputType *token;
    
    Parser(std::istream &istream) : lexer(istream) {}
    Parser(Lexer &lexer) : lexer(lexer) {}
    
    bool isidentifier(const std::string &s) {
        if (!isalpha(s[0])) return false;   // 1st char must be letter
        for (auto c : s.substr(1))          // all the rest...
            if (!(isalpha(c) || c == '_'))  // ...must be either letter or _
                return false;
        return true;
    }
    
    bool isinteger(const std::string &s) {
        for (auto c : s) {
            if (!isdigit(c))
                return false;
        }
        return true;
    }
    
    void Rat20SU() {
        std::cout << lexer(); // $$
            
        std::cout << "$$ <Opt Declaration List>  <Statement List> $$" << std::endl;
            
        OptDeclList();
        StatementList();

        std::cout << *token; // $$
    }
    
    
    void OptDeclList() {
        *token = lexer();
        if (first["DeclList"].find(token->lexeme) != first["DeclList"].end()) {
            
            std::cout << "<Opt Declaration List>  ->  <Declaration List>" << std::endl;
            
            DeclList();
        } else {
            std::cout << "<Opt Declaration List>  ->  <Empty>" << std::endl;
            
            Empty();
        }
        std::cout << std::endl;
        
        // token is the next token
    }
    
    
    void DeclList() {
        std::cout << "<Declaration List>  ->  <Declaration> ; <Declaration List'>" << std::endl;
        
        Declaration();
        
        *token = lexer();
        
        DeclListP();
        
        // token is the next token
    }
    
    void DeclListP() {
        
        if(first["DeclList"].find(token->lexeme) != first["DeclList"].end()) {
            std::cout << "<Declaration List'>  ->  <Declaration List>" << std::endl;
            DeclList();
        } else {
            std::cout << "<Declaration List'>  ->  <Empty>" << std::endl;
            Empty();
        }
        
        // token is the next token
    }
    
    void Declaration() {
        std::cout << "<Declaration>  ->  <Qualifier> <identifier>" << std::endl << std::endl;
        
        Qualifier();
        
        *token = lexer();
        
        Identifier();
        
        std::cout << lexer() << std::endl; // ;
        
        // token is NOT next token
    }
    
    void Qualifier() {
        // token already has value of qualifier
        std::cout << "<Qualifier>" << std::endl;
        std::cout << *token << std::endl;
    }
    
    void Identifier() {
        std::cout << "<Identifier>  " << std::endl;
        std::cout << *token << std::endl;
    }
    
    void StatementList() {
        std::cout << "<Statement List>  ->  <Statement> ; <Statement List'>" << std::endl;
        
        Statement();
        
        *token = lexer();
        
        StatementListP();
        
        // token is the next token
    }
    
    void StatementListP() {
        
        if(first["StatementList"].find(token->lexeme) != first["StatementList"].end()
           || isidentifier(token->lexeme)) {
            std::cout << "<Statement List'>  ->  <Statement List>" << std::endl;
            StatementList();
        } else {
            std::cout << "<Statement List'>  ->  <Empty>" << std::endl;
            Empty();
        }
    }
    
    void Statement() {
        if (token->lexeme == "{") {
            std::cout << "<Statement>  ->  <Compound>" << std::endl;
            Compound();
        } else if (token->lexeme == "if") {
            std::cout << "<Statement>  ->  <If>" << std::endl;
            If();
        } else if (token->lexeme == "put") {
            std::cout << "<Statement>  ->  <Put>" << std::endl;
            Put();
        } else if (token->lexeme == "get") {
            std::cout << "<Statement>  ->  <Get>" << std::endl;
            Get();
        } else if (token->lexeme == "while") {
            std::cout << "<Statement>  ->  <While>" << std::endl;
            While();
        } else {
            std::cout << "<Statement>  ->  <Assign>" << std::endl;
            Assign();
        }
        
        // token is NOT next token
    }
    
    void Compound() {
        std::cout << "<Compound>  ->  {  <Statement List>  }" << std::endl << std::endl;
        
        std::cout << *token << std::endl; // {
        
        *token = lexer();
        
        StatementList();
        
        std::cout << *token << std::endl; // }
        
        // token is NOT next token
    }
    
    void Assign() {
        std::cout << "<Assign>  ->  <Identifier> = <Expression> ;" << std::endl;
        
        Identifier();
        
        std::cout << lexer() << std::endl; // =
        
        Expression();
        
        std::cout << *token << std::endl; // ;
        
        // token is NOT next token
    }
    void If() {
        std::cout << "<If>  ->  if  ( <Condition>  ) <Statement> <Otherwise> fi" << std::endl;
        
        std::cout << *token << std::endl; // if
        std::cout << lexer() <<std::endl; // (
        
        Condition();
        
        std::cout << *token << std::endl; // )
        
        *token = lexer();
        
        // token is next token
        
        Statement();
        
        *token = lexer();
        
        Otherwise();
        
        std::cout << *token <<std::endl; // fi
    }
    
    void Otherwise() {
        if (token->lexeme == "otherwise") {
            std::cout << "<Otherwise>  ->  otherwise  <Statement>" << std::endl;
            *token = lexer();
            Statement();
            *token = lexer();
        } else {
            std::cout << "<Otherwise>  ->  <Empty>" << std::endl;
            Empty();
        }
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
        std::cout << "<Condition>  ->  <Expression> <Relop> <Expression>" << std::endl;
        
        Expression();
        Relop();
        Expression();
        
        // token is next token
    }
    
    void Relop() {
        std::cout << "<Relop>" << std::endl;
        std::cout << *token << std::endl; // operator
    }
    
    void Expression() {
        std::cout << "<Expression>  ->  <Term><Expression'>" << std::endl;
       
        Term();
        ExpressionP();
        
        // token is next token
    }
    
    void ExpressionP() {
        if (token->lexeme == "+") {
            std::cout << "<Expression'>  ->  +<Term><Expression'>" << std::endl;
            Term();
            ExpressionP();
        } else if (token->lexeme == "-") {
            std::cout << "<Expression'>  ->  -<Term><Expression'>" << std::endl;
            Term();
            ExpressionP();
        } else {
            std::cout << "<Expression'>  ->  <Empty>" << std::endl;
            Empty();
        }
    }
    
    void Term() {
        std::cout << "<Term>  ->  <Factor><Term'>" << std::endl;
        
        Factor();
        TermP();
    }
    
    void TermP() {
        if (token->lexeme == "*") {
            std::cout << "<Term'>  ->  *<Factor><Term'>" << std::endl;
            Factor();
            TermP();
        } else if (token->lexeme == "/") {
            std::cout << "<Term'>  ->  /<Factor><Term'>" << std::endl;
            Factor();
            TermP();
        } else {
            std::cout << "<Term'>  ->  <Empty>" << std::endl;
            Empty();
        }
    }
    
    void Factor() {
        *token = lexer();
        
        if (token->lexeme == "-") {
            std::cout << "<Factor>  ->  -<Primary>" << std::endl;
            *token = lexer();
            Primary();
            
        } else {
            std::cout << "<Factor>  ->  <Primary>" << std::endl;
            Primary();
        }
        
        *token = lexer();
    }
    
    void Primary() {
        if (isidentifier(token->lexeme)) {
            std::cout << "<Primary>  ->  <Identifier>" << std::endl;
            Identifier();
        } else if (isinteger(token->lexeme)) {
            std::cout << "<Primary>  ->  <Integer>" << std::endl;
            Integer();
        } else if (token->lexeme == "true" || token->lexeme == "false") {
            std::cout << "<Primary>" << std::endl;
            std::cout << *token << std::endl;
        } else if (token->lexeme == "(") {
            Expression();
            std::cout << lexer() << std::endl; // )
        } else {
            // ERROR
        }
    }
    
    void Integer() {
        std::cout << "<Integer>  " << std::endl;
        std::cout << *token << std::endl;
    }
    
    void Empty() {
        std::cout << "<Empty>  ->  ε" << std::endl;
    }
    
    void operator()() {
        Rat20SU();
    }
    
};


#endif /* parser_h */
