//
//  parser.h
//  Assignment 1
//
//  Created by Michael Burdi on 7/14/20.
//  Copyright © 2020 Michael Burdi. All rights reserved.
//

#ifndef parser_h
#define parser_h

#include <map>
#include <string>
#include <vector>

void _Rat20SU() {
    
}

void _OptDeclList() {
    
}

void _DeclList() {
    
}

void _Decl() {
    
}

void _Qualifier() {
    
}

void _StatementList() {
    
}

void _Statement() {
    
}

void _Compound() {
    
}

void _Assign() {
    
}

void _If() {
    
}

void _Put() {
    
}

void _Get() {
    
}

void _While() {
    
}

void _Condition() {
    
}

void _Relop() {
    
}

void _Expression() {
    
}

void _Term() {
    
}

void _Factor() {
    
}

void _Primary() {
    
}


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


#endif /* parser_h */
