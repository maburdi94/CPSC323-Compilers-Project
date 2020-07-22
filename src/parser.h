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
                std::cout << "*******OPT DECL LIST PASS*******" << std::endl;
                if ( StatementList() ) {
                    std::cout << "*******STATEMENT LIST PASS*******" << std::endl;
                    // token = lexer();
                    std::cout << token;
                    if (token.lexeme == "$$") {
                        std::cout << "*******RAT20SU*******" << std::endl;
                        std::cout << "<Rat20SU>   ::=  $$  <Opt Declaration List>    <Statement List> $$" << std::endl;
                        return true;
                    }
                }
            }
        }

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
            if (std::find(first["DeclList"].begin(),first["DeclList"].end(), token.lexeme) != first["DeclList"].end()) {
                std::cout << "<Opt Declaration List>  ::=  <Empty>" << std::endl;
                return true;
            }
        }

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
            // token = lexer();
            // if (std::find(follow["DeclListPrime"].begin(),follow["DeclListPrime"].end(), token.lexeme) != follow["DeclListPrime"].end()) {
            //     std::cout << "DeclListPrime success" << std::endl;
            //     std::cout << "<Declaration List Prime>     ::=   <Empty>" << std::endl;
            //     return true;
            // }
            

            if (token.lexeme == "if" || token.lexeme == "put" || token.lexeme == "get" || token.lexeme == "while") {
                std::cout << "<Declaration List Prime>     ::=   <Empty>" << std::endl;
                return true;
            }
            else if (token.type == Lexer::IDENTIFIER) {
                std::cout << "DeclListPrime success" << std::endl;
                std::cout << "<Declaration List Prime>     ::=   <Empty>" << std::endl;
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
        return false;
    }

    /*
    R6     <Statement List Prime>        ::=  <Statement>  |  <Empty >
        first = { {, <identifier>, if, put, get, while, e}
        follow = {$$, } }
    */
    bool StatementListPrime () {
        if ( Statement() ) {
            std::cout << "<Statement List Prime>    ::=  <Statement>" << std::endl;
            return true;
        }
        else {
            // token = lexer();
            if (token.lexeme == "$$" || token.lexeme == "}") {
                std::cout << "<Statement List Prime>        ::=   <Empty >" << std::endl;
                return true;
            }
        }
        return false;
    }

    /*
    R7     <Statement>             ::=  <Compound>  |  <Assign>  |  <If>  |  <Put>  |  <Get>  |  <While>
        first = { {, <identifier>, if, put, get, while}
        follow = { {, }, $$, <identifier>, if, put, get, while, otherwise, fi, e}
    */
    bool Statement () {
        std::cout << "------Inside statement with token : " << token;
        if ( Assign() ) {
            std::cout << "Assign was true" << std::endl;
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
                if (token.lexeme == "{") {
                    std::cout << "<Compound>    ::=  {  <Statement List>  }" << std::endl;
                    return true;
                }
            }
        }
        return false;
    }

    /*
    R9     <Assign>                ::=  <Identifier> = <Expression> ;
        first = { <identifier> }
        follow = { {, }, $$, <identifier>, if, put, get, while, otherwise, fi, e}
    */

    bool Assign () {
        std::cout << "------Inside assign with token : " << token;
        if (token.type == Lexer::IDENTIFIER) {
            std::cout << "IDENTIFIER test passed" << std::endl;
            token = lexer();
            std::cout << "current token : " << token;
            if (token.lexeme == "=") {
                if ( Expression() ) {
                    std::cout << "*******Expression passed********* with token : " << token << std::endl;
                    // token = lexer();
                    if (token.lexeme == ";") {
                        // std::cout << 
                        std::cout << "<Assign>    ::=  <Identifier> = <Expression> ;" << std::endl;
                        return true;
                    }
                }
            }
        }
        else {
            if (std::find(follow["Assign"].begin(),follow["Assign"].end(), token.lexeme) != follow["Assign"].end()) {
                std::cout << "<Assign>                ::=  <Identifier> = <Expression> ;" << std::endl;
                return true;
            }
        }
        return false;
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
            return true;
        }
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
        return false;
    }

    /*
    R12    <Get>                   ::=  get ( <Identifier> );
        first = {get}
        follow = { {, }, $$, <identifier>, if, put, get, while, otherwise, fi, e}
    */
    bool Get () {
        std::cout << "GET TOKEN: " << token;
        token = lexer();
        std::cout << "GET TOKEN after lexer(): " << token;
        if (token.lexeme == "get") {
            token = lexer();
            if (token.lexeme == "(") {
                token = lexer();
                if (token.type == Lexer::IDENTIFIER) {
                    token = lexer();
                    if (token.lexeme == ")") {
                        token = lexer();
                        if (token.lexeme == ";") {
                            std::cout << "*********GET SUCCESS**********" << std::endl;
                            std::cout << "<Get>     ::=  get ( <identifier> );" << std::endl;
                            return true;
                        }
                    }
                }
            }
        }

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
        return false;
    }

    /*
    R16a   <Expression>            ::=  <Term><Expression'>
        first = {-, <identifier>, <integer>, (, true, false}
        follow = { ), ==, <, <, ; } 
    */
    bool Expression () {
        if ( Term() ) {
            std::cout << "Term success " << std::endl;
            if ( ExpressionPrime() ) {
                std::cout << "<Expression>    ::=  <Term><Expression'>" << std::endl;
                return true;
            }
        }
        return false;
    }

    /*
    R16b   <Expression'>           ::=  +<Term><Expression'>  |  -<Term><Expression'>  |  <Empty>
        first = {+, -, e}
        follow = { ), ==, <, <, ; }
    */
    bool ExpressionPrime () {
        // token = lexer();
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
                std::cout << "***********EXPRESSION PRIME PASS*************" << std::endl;
                std::cout << "<Expression'>           ::=   <Empty>" << std::endl;
                return true;
            }
        }
        // if (token.lexeme == ";") {
        //     std::cout << "***********EXPRESSION PRIME PASS*************" << std::endl;
        //     return true;
        // }
        return false;
    }

    /*
    R17a   <Term>                  ::=  <Factor><Term'>
        first = {-, <identifier>, <integer>, (, true, false}
        follow = {+, -, e}
    */
    bool Term () {
        if ( Factor() ) {
            std::cout << "Factor success" << std::endl;
            if ( TermPrime() ) {
                std::cout << "*******Term passed*********" << std::endl;
                std::cout << "<Term>         ::=  <Factor><Term'>" << std::endl;
                return true;
            }
        }
        return false;
    }

    /*
    R17b   <Term'>                 ::=  *<Factor><Term'>  |  /<Factor><Term'>  |  <Empty>
        first = {*, /, e}
        follow = {+, -, e}
    */
    bool TermPrime () {
        std::cout << "Token before inside of TermPrime : " << token;
        token = lexer();
        std::cout << "Token inside of TermPrime : " << token;
        
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
            return true;
        }
        else {
            Empty();
            return true;
        }
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
       return false;
   }

    /*
    R19    <Primary>               ::=  <Identifier>  |  <Integer>  |  ( <Expression> )  |  true  |  false
        first = { <identifier>, <integer>, (, true, false}
        follow = {*, /, e}
    */
    bool Primary () {
        std::cout << "PRIMARY token : " << token;
        token = lexer();
        std::cout << "PRIMARY token : " << token;
        if (token.type == Lexer::IDENTIFIER) {
            std::cout << "<Primary>               ::=  <Identifier>" << std::endl;
            return true;
        }
        else if (token.type == Lexer::INTEGER) {
            std::cout << "PRIMARY test SUCCEEDED--------" << std::endl;
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
        return false;
    }

    /*
    R20    <Empty>                 ::=  ε
        first = {e}
        follow = {==, <, >, e}
    */
    bool Empty () {
        std::cout << "*****TOKEN INSIDE OF EMPTY" << token;
        // if (std::find(follow["Empty"].begin(),follow["Empty"].end(), token.lexeme) != follow["Empty"].end()) {
        //     // std::cout << 
        // }
        return true;
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
