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
    
    bool printProductionsUsed = false;
    
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
        for (auto c : s)
            if (!isdigit(c))
                return false;
        return true;
    }
    
    void Rat20SU() {
        std::cout << lexer(); // $$
         
        if (printProductionsUsed) std::cout << "$$ <Opt Declaration List>  <Statement List> $$" << std::endl;
            
        OptDeclList();
        StatementList();

        std::cout << *token; // $$
    }
    
    
    void OptDeclList() {
        *token = lexer();
        if (first["DeclList"].find(token->lexeme) != first["DeclList"].end()) {
            
            if (printProductionsUsed) std::cout << "<Opt Declaration List>  ->  <Declaration List>" << std::endl;
            
            DeclList();
        } else {
            if (printProductionsUsed) std::cout << "<Opt Declaration List>  ->  <Empty>" << std::endl;
            
            Empty();
        }
        
        // token is the next token
    }
    
    
    void DeclList() {
        if (printProductionsUsed) std::cout << "<Declaration List>  ->  <Declaration> ; <Declaration List'>" << std::endl;
        
        Declaration();
        
        *token = lexer();
        
        DeclListP();
        
        // token is the next token
    }
    
    void DeclListP() {
        
        if(first["DeclList"].find(token->lexeme) != first["DeclList"].end()) {
            if (printProductionsUsed) std::cout << "<Declaration List'>  ->  <Declaration List>" << std::endl;
            DeclList();
        } else {
            if (printProductionsUsed) std::cout << "<Declaration List'>  ->  <Empty>" << std::endl;
            Empty();
        }
        
        // token is the next token
    }
    
    void Declaration() {
        if (printProductionsUsed) std::cout << "<Declaration>  ->  <Qualifier> <identifier>" << std::endl;
        
        Qualifier();
        
        *token = lexer();
        
        Identifier();
        
        std::cout << lexer() << std::endl; // ;
        
        // token is NOT next token
    }
    
    void Qualifier() {
        // token already has value of qualifier
        if (printProductionsUsed) std::cout << "<Qualifier>" << std::endl;
        std::cout << *token << std::endl;
    }
    
    void Identifier() {
        if (printProductionsUsed) std::cout << "<Identifier>  " << std::endl;
        std::cout << *token << std::endl;
    }
    
    void StatementList() {
        if (printProductionsUsed) std::cout << "<Statement List>  ->  <Statement> ; <Statement List'>" << std::endl;
        
        Statement();
        
        *token = lexer();
        
        StatementListP();
        
        // token is the next token
    }
    
    void StatementListP() {
        
        if(first["StatementList"].find(token->lexeme) != first["StatementList"].end()
           || isidentifier(token->lexeme)) {
            if (printProductionsUsed) std::cout << "<Statement List'>  ->  <Statement List>" << std::endl;
            StatementList();
        } else {
            if (printProductionsUsed) std::cout << "<Statement List'>  ->  <Empty>" << std::endl;
            Empty();
        }
    }
    
    void Statement() {
        if (token->lexeme == "{") {
            if (printProductionsUsed) std::cout << "<Statement>  ->  <Compound>" << std::endl;
            Compound();
        } else if (token->lexeme == "if") {
            if (printProductionsUsed) std::cout << "<Statement>  ->  <If>" << std::endl;
            If();
        } else if (token->lexeme == "put") {
            if (printProductionsUsed) std::cout << "<Statement>  ->  <Put>" << std::endl;
            Put();
        } else if (token->lexeme == "get") {
            if (printProductionsUsed) std::cout << "<Statement>  ->  <Get>" << std::endl;
            Get();
        } else if (token->lexeme == "while") {
            if (printProductionsUsed) std::cout << "<Statement>  ->  <While>" << std::endl;
            While();
        } else {
            if (printProductionsUsed) std::cout << "<Statement>  ->  <Assign>" << std::endl;
            Assign();
        }
        
        // token is NOT next token
    }
    
    void Compound() {
        if (printProductionsUsed) std::cout << "<Compound>  ->  {  <Statement List>  }" << std::endl << std::endl;
        
        std::cout << *token << std::endl; // {
        
        *token = lexer();
        
        StatementList();
        
        std::cout << *token << std::endl; // }
        
        // token is NOT next token
    }
    
    void If() {
        if (printProductionsUsed) std::cout << "<If>  ->  if  ( <Condition>  ) <Statement> <Otherwise> fi" << std::endl;
        
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
    
    void Assign() {
       if (printProductionsUsed)  std::cout << "<Assign>  ->  <Identifier> = <Expression> ;" << std::endl;
        
        *token = lexer();
        
        DeclListP();
        
        // token is the next token
    }
    
    void Otherwise() {
        if (token->lexeme == "otherwise") {
            if (printProductionsUsed) std::cout << "<Otherwise>  ->  otherwise  <Statement>" << std::endl;
            
            std::cout << *token << std::endl;
            
            *token = lexer(); // {
            
            Statement();
            
            *token = lexer(); // }
            
        } else {
            if (printProductionsUsed) std::cout << "<Otherwise>  ->  <Empty>" << std::endl;
            Empty();
        }
    }
    
    void Put() {
        if (printProductionsUsed) std::cout << "put ( <identifier> );" << std::endl;
    }
    
    void Get() {
        if (printProductionsUsed) std::cout << "get ( <Identifier> );" << std::endl;
    }
    
    void While() {
        if (printProductionsUsed) std::cout << "while ( <Condition>  ) <Statement>" << std::endl;
    }
    
    void Condition() {
        if (printProductionsUsed) std::cout << "<Condition>  ->  <Expression> <Relop> <Expression>" << std::endl;

        Expression();
        Relop();
        Expression();
        
        // token is next token
    }
    
    void Relop() {
        if (printProductionsUsed) std::cout << "<Relop>" << std::endl;
        std::cout << *token << std::endl; // operator
    }
    
    void Expression() {
        if (printProductionsUsed) std::cout << "<Expression>  ->  <Term><Expression'>" << std::endl;
       
        Term();
        ExpressionP();
        
        // token is next token
    }
    
    void ExpressionP() {
        if (token->lexeme == "+") {
            if (printProductionsUsed) std::cout << "<Expression'>  ->  +<Term><Expression'>" << std::endl;
            Term();
            ExpressionP();
        } else if (token->lexeme == "-") {
            if (printProductionsUsed) std::cout << "<Expression'>  ->  -<Term><Expression'>" << std::endl;
            Term();
            ExpressionP();
        } else {
            if (printProductionsUsed) std::cout << "<Expression'>  ->  <Empty>" << std::endl;
            Empty();
        }
    }
    
    void Term() {
        if (printProductionsUsed) std::cout << "<Term>  ->  <Factor><Term'>" << std::endl;
        
        Factor();
        TermP();
    }
    
    void TermP() {
        if (token->lexeme == "*") {
            if (printProductionsUsed) std::cout << "<Term'>  ->  *<Factor><Term'>" << std::endl;
            Factor();
            TermP();
        } else if (token->lexeme == "/") {
            if (printProductionsUsed) std::cout << "<Term'>  ->  /<Factor><Term'>" << std::endl;
            Factor();
            TermP();
        } else {
            if (printProductionsUsed) std::cout << "<Term'>  ->  <Empty>" << std::endl;
            Empty();
        }
    }
    
    void Factor() {
        *token = lexer();
        
        if (token->lexeme == "-") {
            if (printProductionsUsed) std::cout << "<Factor>  ->  -<Primary>" << std::endl;
            *token = lexer();
            Primary();
            
        } else {
            if (printProductionsUsed) std::cout << "<Factor>  ->  <Primary>" << std::endl;
            Primary();
        }
        
        *token = lexer();
    }
    
    void Primary() {
        if (isidentifier(token->lexeme)) {
            if (printProductionsUsed) std::cout << "<Primary>  ->  <Identifier>" << std::endl;
            Identifier();
        } else if (isinteger(token->lexeme)) {
            if (printProductionsUsed) std::cout << "<Primary>  ->  <Integer>" << std::endl;
            Integer();
        } else if (token->lexeme == "true" || token->lexeme == "false") {
            if (printProductionsUsed) std::cout << "<Primary>" << std::endl;
            std::cout << *token << std::endl;
        } else if (token->lexeme == "(") {
            Expression();
            std::cout << lexer() << std::endl; // )
        } else {
            // ERROR
        }
    }
    
    void Integer() {
        if (printProductionsUsed) std::cout << "<Integer>  " << std::endl;
        std::cout << *token << std::endl;
    }
    
    void Empty() {
        if (printProductionsUsed) std::cout << "<Empty>  ->  ε" << std::endl;
    }
    
    void operator()() {
        Rat20SU();
    }
    
};


#endif /* parser_h */
