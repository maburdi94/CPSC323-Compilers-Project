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

    // possibly need to add more first and follow sets
    
    std::map<std::string, std::vector<std::string>> first = {
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

    std::map<std::string, std::vector<std::string>> follow = {
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
    
    // bool Rat20SU() {
    //     token = lexer();
    //     std::cout << token; // $$
            
    //     std::cout << "$$ <Opt Declaration List>  <Statement List> $$" << std::endl;
            
    //     OptDeclList();
    //     StatementList();
        
    //     token = lexer();
    //     std::cout << token; // $$
    // }
    
    
    // bool OptDeclList() {
    //     if (std::find(first["DeclList"].begin(),first["DeclList"].end(), token.lexeme) != first["DeclList"].end()) {
            
    //         std::cout << "<Opt Declaration List>  ->  <Declaration List>" << std::endl;
            
    //         DeclList();
    //     } else {
    //         std::cout << "<Opt Declaration List>  ->  <Empty>" << std::endl;
            
    //         Empty();
    //     }
        
    // }
    
    
    // bool DeclList() {
    //     /*
    //         Push the routes on a stack because we don't know
    //         which route until we get the next token.
    //      */
        
    //     Declaration();
        
    //     std::cout << "<Declaration List>  ->  <Declaration> ;";
        
    //     token = lexer();
    //     std::cout << token; // ;
        
    //     auto match = std::find(first["Declaration"].begin(), first["Declaration"].end(), token.lexeme);
        
    //     if (match != first["Declaration"].end()) {
    //         std::cout << " <Declaration List>" << std::endl;
            
    //         DeclList();
    //     }
        
    // }
    
    // void Declaration() {
    //     token = lexer();
    //     std::cout << token; // qualifier
        
    //     token = lexer();
    //     std::cout << token; // identifier
        
    //     std::cout << "<Declaration>  ->  <Qualifier> <Identifier>" << std::endl;
    // }
    
    // void Qualifier() {
    //     std::cout << "<Qualifier>  ->  integer |  boolean" << std::endl;
    // }
    
    // void StatementList() {
    //     std::cout << "<StatementList>  ->  <Statement>   | <Statement> <Statement List>" << std::endl;
    //     while (lexer && Statement());
    // }
    
    // bool Statement() {
    //     token = lexer();
    //     std::cout << token;
        
    //     if (token.lexeme == "{") {
    //         std::cout<< "<Statement>  ->  <Compound>" << std::endl;
    //         Compound();
    //     }
    //     else if (token.lexeme == "if") If() {
    //         std::cout<< "<Statement>  ->  <If>" << std::endl;
    //         If();
    //     }
    //     else if (token.lexeme == "put") Put() {
    //         std::cout<< "<Statement>  ->  <Put>" << std::endl;
    //         Put();
    //     }
    //     else if (token.lexeme == "get") Get() {
    //         std::cout<< "<Statement>  ->  <Get>" << std::endl;
    //         Get();
    //     }
    //     else if (token.lexeme == "while") While() {
    //         std::cout<< "<Statement>  ->  <While>" << std::endl;
    //         While();
    //     }
    //     else if (isidentifier(token.lexeme)) Assign() {
    //         std::cout<< "<Statement>  ->  <Assign>" << std::endl;
    //         Assign();
    //     }
    //     else if (token.lexeme == "$$") return false;
        
    //     return true;
    // }
    
    // void Compound() {
    //     std::cout << "<Compound>  ->  {  <Statement List>  }" << std::endl;
    //     Compound();
    // }
    // void Assign() {
    //     std::cout << "<Assign>  ->  <Identifier> = <Expression> ;" << std::endl;
    //     Assign();
    // }
    // void If() {
    //     std::cout << "<If>  ->  if  ( <Condition>  ) <Statement> <Otherwise> fi" << std::endl;
    //     If();
    // }
    // void Put() {
    //     std::cout << "put ( <identifier> );" << std::endl;
    // }
    // void Get() {
    //     std::cout << "get ( <Identifier> );" << std::endl;
    // }
    // void While() {
    //     std::cout << "while ( <Condition>  ) <Statement>" << std::endl;
    // }
    // void Condition() {
        
    // }
    // void Relop() {
        
    // }
    // void Expression() {
        
    // }
    // void Term() {}
    // void Factor() {}
    // void Primary() {}
    // void Empty() {}
    
    void operator()() {
        Rat20SU();
    }
    
};


#endif /* parser_h */
