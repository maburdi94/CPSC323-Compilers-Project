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
        { "DecListPrime", { "integer", "boolean", "e" }},
        { "Declaration", { "integer", "boolean" }},
        { "Qualifier", { "integer", "boolean" }},
        { "StatementList", { "{", "if", "put", "get", "while" /* id */ }},
        { "StatementListPrime", { "{", /* id */ "if", "put", "get", "while"} }, 
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
        { "Expression Prime", { ")", "==", "<", ">", ";" } },
        { "Term", { /* id, */ /* int, */ "-", "(", "true", "false" } },
        { "Term Prime", { ")", "==", "<", ">", ";" } },
        { "Factor", { /* id, */ /* int, */ "-", "(", "true", "false" } },
        { "Primary", { /* id, */ /* int, */ "(", "true", "false" } },
        { "Empty", { "e" } }
    };

    std::map<std::string, std::set<std::string> > follow = {
        { "Rat20SU", { "$" } },
        { "OptDeclList", { "{", /* id */ "if", "put", "get", "while" } },
        { "DeclList", { "{", /* id */ "if", "put", "get", "while" } },
        { "DeclListPrime", { "{", /* id */ "if", "put", "get", "while" } },
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
        { "Expression Prime", { ")", "==", "<", ">", ";" } },
        { "Term", { "+", "-", /* id */ /* int */ "true", "false", "(", ")", ";" } },
        { "Term Prime", { "+", "-", "e" } },
        { "Factor", { "*", "/", "+", "-", /* id */ /* int */ "true", "false", "(", ")", ";" } },
        { "Primary", { "*", "/", "+", "-", /* id */ /* int */ "true", "false", "(", ")", ";" } },
        { "Empty", { "{", /* id */ /* int */ "if", "put", "get", "while", "fi", "+", "-", "true", "false",
                "(", ")", ";" } }
    };
    
public:
    struct OutputType {};
    
    Lexer lexer;
    Lexer::OutputType token;
    Lexer::OutputType prev_token;
    // std::stack<std::string> path;

    // enum Token {NONE, IDENTIFIER, KEYWORD, SEPARATOR, OPERATOR, INTEGER, UNKNOWN};

    // struct ExpectedToken {
    //     Token ExpectedToken = NONE;
    //     std::string lexeme;
    //     int lineNum;
    // };
    
    Parser(std::istream &istream) : lexer(istream) {}
    Parser(Lexer &lexer) : lexer(lexer) {}
    
    bool isidentifier(std::string s) {
        for (auto c : s) if (!(isalpha(c) || c == '_')) return false;
        return true;
    }

    Lexer::OutputType next_token() {
        static Lexer::OutputType next_token = lexer();
        return next_token;
    }

    /////////////////////////////
    //      Grammar Rules      //
    /////////////////////////////

    /* 
    R1     <Rat20SU>   ::=  $$  <Opt Declaration List>    <Statement List> $$
        first = {$$}
        follow = {$}

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
        std::cout << "Rat20SU returned false" << std::endl;
        return false;
    }

    /*
    R2     <Opt Declaration List>  ::=  <Declaration List>   |    <Empty>
        first = {integer, boolean, e}
        follow = { {, <identifier>, if, put, get, while}

    */
    bool OptDeclList () {
        if ( DeclList() ) {
            std::cout << "<Opt Declaration List>  ::=  <Declaration List>" << std::endl;
            return true;
        }
        else {
            token = lexer();
            if (std::find(follow["DeclList"].begin(),follow["DeclList"].end(), token.lexeme) != follow["DeclList"].end()) {
                std::cout << "<Opt Declaration List>  ::=  <Empty>" << std::endl;
                lexer.backUp(token);
                return true;
            }
        }
        std::cout << "OptDeclList returned false" << std::endl;
        return false;
    }

    /*
    R3    <Declaration List>     ::=  <Declaration> ; <Declaration List Prime>
        first = {integer, boolean}
        follow = { {, <identifier>, if, put, get, while}

    */
    bool DeclList () {
        if ( Declaration() ) {
            token = lexer();
            if (token.lexeme == ";") {
                if ( DeclListPrime() ) {
                    std::cout << "<Declaration List>     ::=  <Declaration> ; <Declaration List Prime>" << std::endl;
                    return true;
                }
            }
        }
        std::cout << "DeclList returned false" << std::endl;
        return false;
    }

    /*
    R3a    <Declaration List Prime>     ::=  <Declaration List>  |  <Empty>
        first = {integer, boolean, e}
        follow = { {, <identifier>, if, put, get, while}
    */
    bool DeclListPrime () {
        if ( DeclList() ) {
            std::cout << "<Declaration List Prime>     ::=  <Declaration List>" << std::endl;
            return true;
        }
        else {
            if (token.lexeme == "if" || token.lexeme == "put" || token.lexeme == "get" || token.lexeme == "while" || token.lexeme == "{") {
                std::cout << "<Declaration List Prime>     ::=   <Empty>" << std::endl;
                lexer.backUp(token);
                return true;
            }
            else if (token.type == Lexer::IDENTIFIER) {
                std::cout << "<Declaration List Prime>     ::=   <Empty>" << std::endl;
                lexer.backUp(token);
                return true;
            }
        }
        std::cout << "DeclListPrime returned false" << std::endl;
        return false;
    }

    /*
    R4     <Declaration>           ::=  <Qualifier >  <identifier>
        first = {integer, boolean}
        follow = {;}
    */
    bool Declaration () {
        if ( Qualifier() ) {
            token = lexer();
            if (token.type == Lexer::IDENTIFIER) {
                std::cout<< "<Declaration>           ::=  <Qualifier >  <identifier>" << std::endl;
                return true;
            }
        }
        std::cout << "Declaration returned false" << std::endl;
        return false;
    }

    /*
    R5     <Qualifier>             ::=  integer |  boolean
        first = {integer, boolean}
        follow = { <identifier> }

    */
    bool Qualifier () {
        token = lexer();
        if (token.lexeme == "integer" || token.lexeme == "true" || token.lexeme == "false") {
            std::cout << "<Qualifier>    ::=  integer |  boolean" << std::endl;
            return true;
        }
        std::cout << "Qualifier returned false" << std::endl;
        return false;
    }

    /*
    R6     <Statement List>        ::=  <Statement>  <Statement List Prime>
        first = { {, <identifier>, if, put, get, while}
        follow = {$$, } }
    */
    bool StatementList () {
        if ( Statement() ) {
            if ( StatementListPrime() ) {
                std::cout << "<Statement List>   ::=  <Statement>  <Statement List Prime>" << std::endl;
                return true;
            }
        }
        std::cout << "StatementList returned false" << std::endl;
        return false;
    }

    /*
    R6     <Statement List Prime>        ::=  <Statement>  |  <Empty >
        first = { {, <identifier>, if, put, get, while, e}
        follow = {$$, } }
    */
    bool StatementListPrime () {
        if(first["StatementListPrime"].find(token.lexeme) != first["StatementListPrime"].end()) {
            if ( Statement() ) {
                std::cout << "<Statement List Prime>    ::=  <Statement>" << std::endl;
                return true;
            }
        }
        else {
            token = lexer();
            if (token.lexeme == "$$" || token.lexeme == "}") {
                std::cout << "<Statement List Prime>        ::=   <Empty >" << std::endl;
                lexer.backUp(token);
                return true;
            }
        }
        std::cout << "StatementListPrime returned false" << std::endl;
        return false;
    }

    /*
    R7     <Statement>             ::=  <Compound>  |  <Assign>  |  <If>  |  <Put>  |  <Get>  |  <While>
        first = { {, <identifier>, if, put, get, while}
        follow = { {, }, $$, <identifier>, if, put, get, while, otherwise, fi, e}
    */
    bool Statement () {
        if ( Assign() ) {
            std::cout << "<Statement>   ::=  <Assign>" << std::endl;
            return true;
        }
        else if ( If() ) {
            std::cout << "<Statement>   ::=  <If>" << std::endl;
            return true;
        }
        else if ( Put() ) {
            std::cout << "<Statement>   ::=  <Put>" << std::endl;
            return true;
        }
        else if ( Get() ) {
            std::cout << "<Statement>   ::=  <Get>" << std::endl;
            return true;
        }
        else if ( While() ) {
            std::cout << "<Statement>   ::=  <While>" << std::endl;
            return true;
        }
        else if ( Compound() ) {
            std::cout << "<Statement>   ::=  <Compound>" << std::endl;
            return true;
        }
        std::cout << "Statement returned false" << std::endl;
        return false;
    }

    /*
    R8     <Compound>        ::=  {  <Statement List>  }
        first = { { }
        follow = { {, }, $$, <identifier>, if, put, get, while, otherwise, fi, e}
    */
    bool Compound () {
        token = lexer();
        if (token.lexeme == "{") {
            if ( Statement() ) {
                token = lexer();
                if (token.lexeme == "}") {
                    std::cout << "<Compound>    ::=  {  <Statement List>  }" << std::endl;
                    return true;
                }
            }
        }
            
        std::cout << "Compound returned false" << std::endl;
        return false;
    }

    /*
    R9     <Assign>                ::=  <Identifier> = <Expression> ;
        first = { <identifier> }
        follow = { {, }, $$, <identifier>, if, put, get, while, otherwise, fi, e}
    */

    bool Assign () {
        std::cout << token;
        token = lexer();
        std::cout << token;
        if (token.type == Lexer::IDENTIFIER) {
            token = lexer();
            std::cout << token;
            if (token.lexeme == "=") {
                if ( Expression() ) {
                    token = lexer();
                    std::cout << token;
                    if (token.lexeme == ";") {
                        std::cout << "<Assign>       ::=  <Identifier> = <Expression> ;" << std::endl;
                        return true;
                    }
                }
            }
        }
        if (std::find(follow["Assign"].begin(),follow["Assign"].end(), token.lexeme) != follow["Assign"].end()) {
            std::cout << "<Assign>                ::=  <Identifier> = <Expression> ;" << std::endl;
            return true;
        }
        else {
            std::cout << "Assign returned false" << std::endl;
            return false;
        }
    }

    /*
    R10a   <If>                    ::=  if  ( <Condition>  ) <Statement> <Otherwise> fi
        first = {if}
        follow = { {, }, $$, <identifier>, if, put, get, while, otherwise, fi, e}
    */
    bool If () {
        token = lexer();
        if (token.lexeme == "if") {
            token = lexer();
            if (token.lexeme == "(") {
                if ( Condition() ) {
                    token = lexer();
                    if (token.lexeme == ")") {
                        if ( Statement() ) {
                            if ( Otherwise () ) {
                                token = lexer();
                                if (token.lexeme == "fi") {
                                    std::cout << "<If>   ::=  if  ( <Condition>  ) <Statement> <Otherwise> fi" << std::endl;
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }
        std::cout << "If returned false" << std::endl;
        return false;
    }

    /*
    R10b   <Otherwise>             ::=  otherwise  <Statement> | <Empty>
        first = {otherwise, e}
        follow = {fi}
    */
    bool Otherwise () {
        token = lexer();
        if (token.lexeme == "otherwise") {
            if ( Statement() ) {
                std::cout << "<Otherwise>    ::=  otherwise  <Statement>" << std::endl;
                return true;
            }
        }
        else if (token.lexeme == "fi") {                // MIGHT need to add token = lexer(); inside the else
            std::cout << "<Otherwise>       ::=   <Empty>" << std::endl;
            lexer.backUp(token);
            return true;
        }
        std::cout << "Otherwise returned false" << std::endl;
        return false;
    }

    /*
    R11    <Put>                   ::=  put ( <identifier> );
        first = {put}
        follow = { {, }, $$, <identifier>, if, put, get, while, otherwise, fi, e}
    */
    bool Put () {
        token = lexer();
        if (token.lexeme == "put") {
            token = lexer();
            if (token.lexeme == "(") {
                token = lexer();
                if (token.type == Lexer::IDENTIFIER) {
                    token = lexer();
                    if (token.lexeme == ")") {
                        token = lexer();
                        if (token.lexeme == ";") {
                            std::cout << "<Put>     ::=  put ( <identifier> );" << std::endl;
                            return true;
                        }
                    }
                }
            }
        }
        std::cout << "Put returned false" << std::endl;
        return false;
    }

    /*
    R12    <Get>                   ::=  get ( <Identifier> );
        first = {get}
        follow = { {, }, $$, <identifier>, if, put, get, while, otherwise, fi, e}
    */
    bool Get () {
        token = lexer();
        if (token.lexeme == "get") {
            token = lexer();
            if (token.lexeme == "(") {
                token = lexer();
                if (token.type == Lexer::IDENTIFIER) {
                    token = lexer();
                    if (token.lexeme == ")") {
                        token = lexer();
                        if (token.lexeme == ";") {
                            std::cout << "<Get>     ::=  get ( <identifier> );" << std::endl;
                            return true;
                        }
                    }
                }
            }
        }
        std::cout << "Get returned false" << std::endl;
        return false;
    }

    /*
    R13    <While>                 ::=  while ( <Condition>  ) <Statement>
        first = {while}
        follow = { {, }, $$, <identifier>, if, put, get, while, otherwise, fi, e}
    */
    bool While () {
        token = lexer();
        if (token.lexeme == "while") {
            token = lexer();
            if (token.lexeme == "(") {
                if ( Condition() ) {
                    token = lexer();
                    if (token.lexeme == ")") {
                        if ( Statement() ) {
                            std::cout << "<While>    ::=  while ( <Condition>  ) <Statement>" << std::endl;
                            return true;
                        }
                    }
                }
            }
        }
        std::cout << "While returned false" << std::endl;
        return false;
    }

    /*
    R14    <Condition>             ::=  <Expression> <Relop> <Expression>
        first = {-, <identifier>, <integer>, (, true, false}
        follow = { )  }
    */
    bool Condition () {
        if ( Expression() ) {
            if ( Relop() ) {
                if ( Expression() ) {
                    std::cout << "<Condition>  ::=  <Expression> <Relop> <Expression>" << std::endl;
                    return true;
                }
            }
        }
        std::cout << "Condition returned false" << std::endl;
        return false;
    }

    /*
    R15    <Relop>                 ::=  ==  |  >  |  <
        first = {==, >, <}
        follow = {-, <identifier>, <integer>, (, true, false}
    */
    bool Relop () {
        token = lexer();
        if (token.lexeme == "==") {
            std::cout << "<Relop>  ::=  ==" << std::endl;
            return true;
        }
        else if (token.lexeme == "<") {
            std::cout << "<Relop>  ::=  <" << std::endl;
            return true;
        }
        else if (token.lexeme == ">") {
            std::cout << "<Relop>  ::=  >" << std::endl;
            return true;
        }
        std::cout << "Relop returned false" << std::endl;
        return false;
    }

    /*
    R16a   <Expression>            ::=  <Term><Expression'>
        first = {-, <identifier>, <integer>, (, true, false}
        follow = { ), ==, <, <, ; } 
    */
    bool Expression () {
        if ( Term() ) {
            if ( ExpressionPrime() ) {
                std::cout << "<Expression>    ::=  <Term><Expression'>" << std::endl;
                return true;
            }
        }
        std::cout << "Expression returned false" << std::endl;
        return false;
    }

    /*
    R16b   <Expression'>           ::=  +<Term><Expression'>  |  -<Term><Expression'>  |  <Empty>
        first = {+, -, e}
        follow = { ), ==, <, <, ; }
    */
    bool ExpressionPrime () {
        token = lexer();
        if (token.lexeme == "+") {
            if ( Term() ) {
                if ( ExpressionPrime() ) {
                    std::cout << "<Expression'>           ::=  +<Term><Expression'>" << std::endl;
                    return true;
                }
            }
        }
        else if (token.lexeme == "-") {
            if ( Term() ) {
                if ( ExpressionPrime() ) {
                    std::cout << "<Expression'>           ::=  -<Term><Expression'>" << std::endl;
                    return true;
                }
            }
        }
        else {
            if (std::find(follow["Expression Prime"].begin(),follow["Expression Prime"].end(), token.lexeme) != follow["Expression Prime"].end()) {
                std::cout << "<Expression'>           ::=   <Empty>" << std::endl;
                lexer.backUp(token);
                return true;
            }
        }
        if (token.lexeme == ";") {
            std::cout << "<Expression'>           ::=   <Empty>" << std::endl;
            lexer.backUp(token);
            return true;
        }
        std::cout << "ExpressionPrime returned false" << std::endl;
        return false;
    }

    /*
    R17a   <Term>                  ::=  <Factor><Term'>
        first = {-, <identifier>, <integer>, (, true, false}
        follow = {+, -, e}
    */
    bool Term () {
        if ( Factor() ) {
            if ( TermPrime() ) {
                std::cout << "<Term>         ::=  <Factor><Term'>" << std::endl;
                return true;
            }
        }
        std::cout << "Term returned false" << std::endl;
        return false;
    }

    /*
    R17b   <Term'>                 ::=  *<Factor><Term'>  |  /<Factor><Term'>  |  <Empty>
        first = {*, /, e}
        follow = {+, -, e}
    */
    bool TermPrime () {
        token = lexer();
        
        if (token.lexeme == "*") {
            if ( Factor() ) {
                if ( TermPrime() ) {
                    std::cout << "<Term'>      ::=  *<Factor><Term'>" << std::endl;
                    return true;
                }
            }
        }
        else if (token.lexeme == "/") {
            if ( Factor() ) {
                if ( TermPrime() ) {
                    std::cout << "<Term'>      ::=  /<Factor><Term'> " << std::endl;
                    return true;
                }
            }
        }
        else if (token.lexeme == "+" || token.lexeme == "-") {
            std::cout << "<Term'>       ::=  <Empty>" << std::endl;
            lexer.backUp(token);
            return true;
        }
        else {
            Empty();            ////***********************
            return true;
        }
        std::cout << "TermPrime returned false" << std::endl;
        return false;
    }

    /*
    R18    <Factor>                ::=  -<Primary>  |  <Primary>
        first = {-, <identifier>, <integer>, (, true, false}
        follow = {*, /, e}
    */
   bool Factor () {
       if ( Primary() ) {
           std::cout << "<Factor>  ::=   <Primary>" << std::endl;
           return true;
       }
       else {
           token = lexer();
           if (token.lexeme == "-") {
               if ( Primary() ) {
                   std::cout << "<Factor>  ::=   -<Primary>" << std::endl;
                   return true;
               }
           }
       }
       std::cout << "Factor returned false" << std::endl;
       return false;
   }

    /*
    R19    <Primary>               ::=  <Identifier>  |  <Integer>  |  ( <Expression> )  |  true  |  false
        first = { <identifier>, <integer>, (, true, false}
        follow = {*, /, e}
    */
    bool Primary () {
        token = lexer();
        std::cout << "TOKEN inside of Primary : " << token;
        if (token.type == Lexer::IDENTIFIER) {
            std::cout << "<Primary>               ::=  <Identifier>" << std::endl;
            return true;
        }
        else if (token.type == Lexer::INTEGER) {
            std::cout << "INTEGER test passed " << std::endl;
            std::cout << "<Primary>      ::=  <Integer>" << std::endl;
            return true;
        }
        else if (token.lexeme == "(") {
            if ( Expression() ) {
                token = lexer();
                if (token.lexeme == ")") {
                    std::cout << "<Primary>               ::=  ( <Expression> )" << std::endl;
                    return true;
                }
            }
        }
        else if (token.lexeme == "true") {
            std::cout << "<Primary>           ::=   true  " << std::endl;
            return true;
        }
        else if (token.lexeme == "false") {
            std::cout << "<Primary>           ::=   false  " << std::endl;
            return true;
        }
        std::cout << "Primary returned false" << std::endl;
        return false;
    }

    /*
    R20    <Empty>                 ::=  ε
        first = {e}
        follow = {==, <, >, e}
    */
    bool Empty () {
        token = lexer();
        if (token.lexeme == "==" || token.lexeme == "<" || token.lexeme == ">") {
            lexer.backUp(token);
            return true;
        }
        return false;
    }
    
    void operator()() {
        std::cout << "Starting to parse..." << std::endl;
        
        if ( Rat20SU() /* && token.lexeme == "$" */) {
            std::cout << "Success" << std::endl;
            token = lexer();
            if (token.lexeme == "$") {
                std::cout << "Success" << std::endl;
            }
        }
        else {
            std::cout << "Failed" << std::endl;
        }

        // token = lexer();
        // std::cout << token;

        // lexer.backUp();
        // token = lexer();
        // std::cout << token;
    }
    
};


#endif /* parser_h */
