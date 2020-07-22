
#ifndef parser_h
#define parser_h

#include <iostream>
#include <map>
#include <string>
#include <unordered_set>
#include <exception>

#include "lexer.h"


class Parser {
    
public:
    
    struct SyntaxError : std::exception {
        std::string error_msg;
        std::string line;
        unsigned long lineNumber;

        SyntaxError(const std::string &err_msg, const std::string &line, Lexer::OutputType *token) : error_msg(err_msg), lineNumber(token->line), line(line) {}
        
        friend std::ostream &operator<<(std::ostream &os, SyntaxError &e);
    };
    
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
        for (auto c : s)        // all chars...
            if (!isdigit(c))    // ...must be 0-9
                return false;
        return true;
    }
    
    
    void Rat20SU() {
        lexer(token);
        
        if (token->lexeme != "$$") {
            throw SyntaxError("Program must start with $$", lexer.getline(), token);
        }
        
        std::cout << *token << std::endl;  // $$
        
        std::cout << "$$ <Opt Declaration List>  <Statement List> $$" << std::endl;
            
        OptDeclList();
        StatementList();
        
        if (token->lexeme != "$$") {
            throw SyntaxError("Program must start with $$", lexer.getline(), token);
        }

        std::cout << *token << std::endl; // $$
        
    }
    
    
    void OptDeclList() {
        
        static const std::unordered_set<std::string> firsts = {"boolean", "integer"};
        
        lexer(token);

        if (firsts.find(token->lexeme) != firsts.end()) {
            std::cout << "<Opt Declaration List>  ->  <Declaration List>" << std::endl;
            DeclList();
        }
        else {
            std::cout << "<Opt Declaration List>  ->  <Empty>" << std::endl;
            Empty();
        }
        
    }
    
    
    void DeclList() {
        std::cout << "<Declaration List>  ->  <Declaration> ; <Declaration List'>" << std::endl;
        
        Declaration();
        DeclListP();
        
    }
    
    
    void DeclListP() {
        lexer(token);
        
        if(token->lexeme == "boolean" || token->lexeme == "integer") {
            std::cout << "<Declaration List'>  ->  <Declaration List>" << std::endl;
            DeclList();
        }
        else {
             std::cout << "<Declaration List'>  ->  <Empty>" << std::endl;
            Empty();
        }
        
    }
    
    
    void Declaration() {
        std::cout << "<Declaration>  ->  <Qualifier> <identifier>" << std::endl;
        
        Qualifier();
        
        lexer(token);
        
        Identifier();
        
        std::cout << lexer(token) << std::endl; // ;
        
    }
    
    
    void Qualifier() {
         std::cout << "<Qualifier>  -> " << *token << std::endl;
    }
    
    
    void Identifier() {
        
        if (!isidentifier(token->lexeme)) {
            throw SyntaxError("Invalid identifier " + token->lexeme, lexer.getline(), token);
        }
        
         std::cout << "<Identifier>  -> " << *token << std::endl;
        
    }
    
    
    void StatementList() {
        std::cout << "<Statement List>  ->  <Statement> ; <Statement List'>" << std::endl;
        
        Statement();
        
        lexer(token);
        
        StatementListP();
        
    }
    
    
    void StatementListP() {
        
        static const std::unordered_set<std::string> firsts = {"{", "if", "put", "get", "while"};
        
        if(firsts.find(token->lexeme) != firsts.end() || isidentifier(token->lexeme)) {
            std::cout << "<Statement List'>  ->  <Statement List>" << std::endl;
            StatementList();
        }
        else {
            std::cout << "<Statement List'>  ->  <Empty>" << std::endl;
            Empty();
        }
        
    }
    
    
    void Statement() {
        if (token->lexeme == "{") {
            std::cout << "<Statement>  ->  <Compound>" << std::endl;
            Compound();
        }
        else if (token->lexeme == "if") {
            std::cout << "<Statement>  ->  <If>" << std::endl;
            If();
        }
        else if (token->lexeme == "put") {
            std::cout << "<Statement>  ->  <Put>" << std::endl;
            Put();
        }
        else if (token->lexeme == "get") {
            std::cout << "<Statement>  ->  <Get>" << std::endl;
            Get();
        }
        else if (token->lexeme == "while") {
            std::cout << "<Statement>  ->  <While>" << std::endl;
            While();
        }
        else if (isidentifier(token->lexeme)) {
            std::cout << "<Statement>  ->  <Assign>" << std::endl;
            Assign();
        }
        else {
            throw SyntaxError("Expected statement", lexer.getline(), token);
        }
        
    }
    
    
    void Compound() {
        std::cout << "<Compound>  ->  {  <Statement List>  }" << std::endl << std::endl;
        
        std::cout << *token << std::endl; // {
        
        lexer(token);
        
        StatementList();
        
        if (token->lexeme != "}") {
            throw SyntaxError("Expected }", lexer.getline(), token);
        }
        
        std::cout << *token << std::endl; // }

    }
    
    
    void If() {
        std::cout << "<If>  ->  if  ( <Condition>  ) <Statement> <Otherwise> fi" << std::endl;
        
        std::cout << *token << std::endl; // if
        
        if (token->lexeme != "if") {
            throw SyntaxError("Assignment expression missing =", lexer.getline(), token);
        }
        
        lexer(token);
        
        std::cout << *token << std::endl; // (
        
        Condition();
        
        std::cout << *token << std::endl; // )
        
        lexer(token);
        
        Statement();
        
        lexer(token);
        
        Otherwise();
        
        std::cout << *token <<std::endl; // fi
        
    }
    
    
    void Assign() {

        std::cout << "<Assign>  ->  <Identifier> = <Expression> ;" << std::endl;
        
        Identifier();
        
        lexer(token);
        
        if (token->lexeme != "=") {
            throw SyntaxError("Assignment expression missing =", lexer.getline(), token);
        }
        
        std::cout << *token << std::endl; // =
        
        Expression();
        
        if (token->lexeme != ";") {
            throw SyntaxError("Missing semicolon at the end of line", lexer.getline(), token);
        }
        
        std::cout << *token << std::endl; // ;
        
    }
    
    
    void Otherwise() {
        if (token->lexeme == "otherwise") {
            std::cout << "<Otherwise>  ->  otherwise  <Statement>" << std::endl;
            
            lexer(token);
            
            Statement();
            
            lexer(token); // fi
            
            if (token->lexeme != "fi") {
                throw SyntaxError("Expected closing fi", lexer.getline(), token);
            }
            
        }
        else { // token is fi
            std::cout << "<Otherwise>  ->  <Empty>" << std::endl;
            Empty();
        }
    }
    
    
    void Put() {
        std::cout << "<Put>  ->  put ( <identifier> );" << std::endl;
        
        std::cout << *token << std::endl; // put
        
        lexer(token);
        
        if (token->lexeme != "(") {
            throw SyntaxError("Expected (", lexer.getline(), token);
        }
        
        std::cout << *token << std::endl; // (
        
        lexer(token);
        
        Identifier();
        
        lexer(token);
        
        if (token->lexeme != ")") {
            throw SyntaxError("Expected )", lexer.getline(), token);
        }
        
        std::cout << *token << std::endl; // )
        
        lexer(token);
        
        if (token->lexeme != ";") {
            throw SyntaxError("Missing semicolon at the end of line", lexer.getline(), token);
        }
        
        std::cout << *token << std::endl; // ;
        
    }
    
    
    void Get() {
        std::cout << "get ( <Identifier> );" << std::endl;
        
        std::cout << *token; // get
        
        lexer(token);
        
        if (token->lexeme != "(") {
            throw SyntaxError("Expected (", lexer.getline(), token);
        }
        
        std::cout << *token << std::endl; // (
        
        lexer(token);
        
        Identifier();
        
        lexer(token);
        
        if (token->lexeme != ")") {
            throw SyntaxError("Expected )", lexer.getline(), token);
        }
        
        std::cout << *token << std::endl; // )
        
        if (token->lexeme != ";") {
            throw SyntaxError("Missing semicolon at the end of line", lexer.getline(), token);
        }
        
        std::cout << *token << std::endl; // ;
        
    }
    
    
    void While() {
        std::cout << "<While>  ->  while ( <Condition>  ) <Statement>" << std::endl;
        
        std::cout << *token << std::endl; // while
       
        lexer(token);
        
        if (token->lexeme != "(") {
            throw SyntaxError("Expected (", lexer.getline(), token);
        }
        
        std::cout << *token << std::endl; // (
        
        Condition();
        
        if (token->lexeme != ")") {
            throw SyntaxError("Expected )", lexer.getline(), token);
        }
        
        std::cout << *token << std::endl; // )
        
        lexer(token);
        
        Statement();
        
    }
    
    
    void Condition() {
        std::cout << "<Condition>  ->  <Expression> <Relop> <Expression>" << std::endl;

        Expression();
        Relop();
        Expression();
        
    }
    
    
    void Relop() {
        if (token->lexeme == "==" || token->lexeme == ">" || token->lexeme == "<") {
            std::cout << "<Relop>  -> " << *token << std::endl;
        }
        else {
            throw SyntaxError("Expected relational operator (==, >, <)", lexer.getline(), token);
        }
    }
    
    
    void Expression() {
         std::cout << "<Expression>  ->  <Term><Expression'>" << std::endl;
        
         Term();
         ExpressionP();
        
    }
    
    
    void ExpressionP() {
        if (token->lexeme == "+") {
            std::cout << "<Expression'>  ->  +<Term><Expression'>" << std::endl;
            
            Term();
            ExpressionP();
        }
        else if (token->lexeme == "-") {
            std::cout << "<Expression'>  ->  -<Term><Expression'>" << std::endl;
           
            Term();
            ExpressionP();
        }
        else {
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
        }
        else if (token->lexeme == "/") {
            std::cout << "<Term'>  ->  /<Factor><Term'>" << std::endl;
            
            Factor();
            TermP();
        }
        else {
            std::cout << "<Term'>  ->  <Empty>" << std::endl;
            
            Empty();
        }
    }
    
    
    void Factor() {
        lexer(token);
        
        if (token->lexeme == "-") {
            std::cout << "<Factor>  ->  -<Primary>" << std::endl;
            
            lexer(token);
            Primary();
        }
        else {
            std::cout << "<Factor>  ->  <Primary>" << std::endl;
            
            Primary();
        }
        
        lexer(token);
        
    }
    
    
    void Primary() {
        
        if (isidentifier(token->lexeme)) {
            std::cout << "<Primary>  ->  <Identifier>" << std::endl;
            
            Identifier();
        }
        else if (isinteger(token->lexeme)) {
            std::cout << "<Primary>  ->  <Integer>" << std::endl;
            
            Integer();
        }
        else if (token->lexeme == "true" || token->lexeme == "false") {
            std::cout << "<Primary>" << std::endl;
            
            std::cout << *token << std::endl;
        }
        else if (token->lexeme == "(") {
            Expression();
            
            if (token->lexeme != ")") {
                throw SyntaxError("Expected ) at end of expression", lexer.getline(), token);
            }
            
            std::cout << lexer(token) << std::endl; // )
            
        }
        else {
            throw SyntaxError("Expected id, integer, true|false, or '('", lexer.getline(), token);
        }
    }
    
    
    void Integer() {
         std::cout << "<Integer>  -> " << *token << std::endl;
    }
    
    
    void Empty() {
         std::cout << "<Empty>  ->  ε" << std::endl;
    }
    
    
    void operator()() {
        try {
            Rat20SU();
        } catch (SyntaxError &e) {
            std::cout << e << std::endl;
        }
    }
    
    
};

std::ostream &operator<<(std::ostream &os, Parser::SyntaxError &e) {
    os << "\nsyntax error in line " << e.lineNumber << ":\t" << e.line << "\n" << e.error_msg << std::endl;
    return os;
}


#endif /* parser_h */
