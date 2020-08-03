
#ifndef parser_h
#define parser_h

#include <iostream>
#include <map>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <exception>
#include <vector>
#include <utility>

#include "lexer.h"


size_t MEMORY_ADDRESS = 5000;


class Parser {
    
public:
    
    struct SyntaxError : std::exception {
        std::string error_msg;
        std::string line;
        unsigned long lineNumber;

        SyntaxError(const std::string &err_msg, const std::string &line, Lexer::OutputType &token) : error_msg(err_msg), lineNumber(token.line), line(line) {}
        
        friend std::ostream &operator<<(std::ostream &os, SyntaxError &e);
    };
    
    
    struct Symbol {
        size_t address;
        enum Type {Boolean, Integer} type;
    };
    
    struct Instruction {
        enum Op {
            PUSHI, PUSHM, POPM, STDOUT, STDIN, ADD, SUB, MUL, DIV, GRT, LES, EQU, JUMPZ, JUMP, LABEL
        } op;
        size_t operand;
        
        friend std::ostream & operator<<(std::ostream &, Parser::Instruction &);
    };
    
    // Symbol Table
    std::unordered_map<std::string, Symbol> symbols;
    
    // Assembly instructions
    std::vector<Instruction> instructions;
    
    
    Lexer lexer;
    Lexer::OutputType token;
    
    Parser(std::istream &istream) : lexer(istream) {}
    Parser(Lexer &lexer) : lexer(lexer) {}
    
    
    void operator()() {
        try {
            Rat20SU();
            
            for (int i = 0; i < instructions.size(); i++) {
                std::cout.width(3);
                std::cout << std::right << (i + 1);
                
                std::cout.width(2);
                std::cout << std::left << ".";
                
                std::cout.width(6);
                std::cout << std::left << instructions[i];
            }
            
        } catch (SyntaxError &e) {
            std::cout << e << std::endl;
        }
    }
    
    
private:
    
    void expect(Lexer::OutputType &token, const std::string &value, const std::string &msg) {
        expect(token, token.lexeme == value, msg);
    }
    
    void expect(Lexer::OutputType &token, const std::unordered_set<std::string> &&symbols, const std::string &msg) {
        expect(token, symbols.find(token.lexeme) != symbols.end(), msg);
    }
    
    void expect(Lexer::OutputType &token, bool expression, const std::string &msg) {
        if (!expression) {
            throw SyntaxError(msg, lexer.getline(), token);
        }
    }
    
    
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
        
        lexer(&token);
        
        expect(token, "$$", "Program must start with $$");
        
        std::cout << token << std::endl;  // $$
        
        std::cout << "$$ <Opt Declaration List>  <Statement List> $$" << std::endl;
            
        OptDeclList();
        StatementList();
        
        expect(token, "$$", "Program must end with $$");

        std::cout << token << std::endl; // $$
        
    }
    
    
    void OptDeclList() {
        
        static const std::unordered_set<std::string> firsts = {"boolean", "integer"};
        
        lexer(&token);

        if (firsts.find(token.lexeme) != firsts.end()) {
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
        
        lexer(&token);
        
        expect(token, ";", "Missing semicolon at the end of declaration");
        
        DeclListP();
        
    }
    
    
    void DeclListP() {
        lexer(&token);
        
        if(token.lexeme == "boolean" || token.lexeme == "integer") {
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
        
        // Type of declaration
        Symbol::Type type = token.lexeme == "boolean" ?
            Symbol::Type::Boolean :
            Symbol::Type::Integer;
        
        lexer(&token);
        Identifier();
        
        // Add symbol to table
        symbols[token.lexeme] = {MEMORY_ADDRESS++, type};
        
    }
    
    
    void Qualifier() {
         std::cout << "<Qualifier>  -> " << token << std::endl;
    }
    
    
    void Identifier() {
        
        expect(token, isidentifier(token.lexeme), "Invalid identifier");
        
        std::cout << "<Identifier>  -> " << token << std::endl;
        
    }
    
    
    void StatementList() {
        std::cout << "<Statement List>  ->  <Statement> ; <Statement List'>" << std::endl;
        
        Statement();
        
        lexer(&token);
        
        StatementListP();
        
    }
    
    
    void StatementListP() {
        
        static const std::unordered_set<std::string> firsts = {"{", "if", "put", "get", "while"};
        
        if(firsts.find(token.lexeme) != firsts.end() || isidentifier(token.lexeme)) {
            std::cout << "<Statement List'>  ->  <Statement List>" << std::endl;
            StatementList();
        }
        else {
            std::cout << "<Statement List'>  ->  <Empty>" << std::endl;
            Empty();
        }
        
    }
    
    
    void Statement() {
        if (token.lexeme == "{") {
            std::cout << "<Statement>  ->  <Compound>" << std::endl;
            Compound();
        }
        else if (token.lexeme == "if") {
            std::cout << "<Statement>  ->  <If>" << std::endl;
            If();
        }
        else if (token.lexeme == "put") {
            std::cout << "<Statement>  ->  <Put>" << std::endl;
            Put();
        }
        else if (token.lexeme == "get") {
            std::cout << "<Statement>  ->  <Get>" << std::endl;
            Get();
        }
        else if (token.lexeme == "while") {
            std::cout << "<Statement>  ->  <While>" << std::endl;
            While();
        }
        else if (isidentifier(token.lexeme)) {
            std::cout << "<Statement>  ->  <Assign>" << std::endl;
            Assign();
        }
        else {
            throw SyntaxError("Expected statement", lexer.getline(), token);
        }
        
    }
    
    
    void Compound() {
        std::cout << "<Compound>  ->  {  <Statement List>  }" << std::endl << std::endl;
        
        std::cout << token << std::endl; // {
        
        lexer(&token);
        
        StatementList();
        
        expect(token, "}", "Expected }");
        
        std::cout << token << std::endl; // }

    }
    
    
    void If() {
        std::cout << "<If>  ->  if  ( <Condition>  ) <Statement> <Otherwise> fi" << std::endl;
        
        std::cout << token << std::endl; // if
        
        lexer(&token);
        
        expect(token, "(", "Expected (");
        
        std::cout << token << std::endl; // (
        
        Condition();
        
        expect(token, ")", "Expected )");
        
        std::cout << token << std::endl; // )
        
        lexer(&token);
        
        Statement();
        
        lexer(&token);
        
        Otherwise();
        
        expect(token, "fi", "Expected fi");
        
        std::cout << token <<std::endl; // fi
        
    }
    
    
    void Assign() {

        std::cout << "<Assign>  ->  <Identifier> = <Expression> ;" << std::endl;
        
        Identifier();
        
        std::string iden = token.lexeme;
        
        lexer(&token);
        
        expect(token, "=", "Assignment expression missing =");
        
        std::cout << token << std::endl; // =
        
        Expression();
        
        expect(token, ";", "Missing semicolon at the end of line");
        
        std::cout << token << std::endl; // ;
        
        instructions.push_back({Instruction::POPM, symbols[iden].address});
        
    }
    
    
    void Otherwise() {
        if (token.lexeme == "otherwise") {
            std::cout << "<Otherwise>  ->  otherwise  <Statement>" << std::endl;
            
            lexer(&token);
            
            Statement();
            
            lexer(&token); // fi
            
        }
        else { // token is fi
            std::cout << "<Otherwise>  ->  <Empty>" << std::endl;
            Empty();
        }
    }
    
    
    void Put() {
        std::cout << "<Put>  ->  put ( <identifier> );" << std::endl;
        
        std::cout << token << std::endl; // put
        
        lexer(&token);
        
        expect(token, "(", "Expected (");
        
        std::cout << token << std::endl; // (
        
        lexer(&token);
        
        Identifier();
        
        lexer(&token);
        
        expect(token, ")", "Expected )");
        
        std::cout << token << std::endl; // )
        
        lexer(&token);
        
        expect(token, ";", "Missing semicolon at the end of line");
        
        std::cout << token << std::endl; // ;
        
    }
    
    
    void Get() {
        std::cout << "get ( <Identifier> );" << std::endl;
        
        std::cout << token; // get
        
        lexer(&token);
        
        expect(token, "(", "Expected (");
        
        std::cout << token << std::endl; // (
        
        lexer(&token);
        
        Identifier();
        
        lexer(&token);
        
        expect(token, ")", "Expected )");
        
        std::cout << token << std::endl; // )
        
        expect(token, ";", "Missing semicolon at the end of line");
        
        std::cout << token << std::endl; // ;
        
    }
    
    
    void While() {
        std::cout << "<While>  ->  while ( <Condition>  ) <Statement>" << std::endl;
        std::cout << token << std::endl; // while
        
        instructions.push_back({Instruction::LABEL, NULL});
        size_t addr = instructions.size();
       
        lexer(&token);
        expect(token, "(", "Expected (");
        std::cout << token << std::endl; // (
        
        Condition();
        
        Instruction &back_patch = instructions.back();
        
        expect(token, ")", "Expected )");
        std::cout << token << std::endl; // )
        
        lexer(&token);
        Statement();
        
        instructions.push_back({Instruction::JUMP, addr});
        
        back_patch.operand = instructions.size() + 1;
    }
    
    
    void Condition() {
        std::cout << "<Condition>  ->  <Expression> <Relop> <Expression>" << std::endl;

        Expression(); // PUSHM
        
        Lexer::OutputType op = token; // (copy) operator
        
        Relop();      // no change to instructions
        Expression(); // PUSHM
        
        if (op.lexeme == "==") {
            instructions.push_back({Instruction::EQU, NULL});
            instructions.push_back({Instruction::JUMPZ, NULL});
        } else if (op.lexeme == "<") {
            instructions.push_back({Instruction::LES, NULL});
            instructions.push_back({Instruction::JUMPZ, NULL});
        } else if (op.lexeme == ">") {
            instructions.push_back({Instruction::GRT, NULL});
            instructions.push_back({Instruction::JUMPZ, NULL});
        }
        
        // JUMPZ is always the last instruction
    }
    
    
    void Relop() {
        
        expect(token, {"==", ">", "<"}, "Expected relational operator (==, >, <)");
        
        std::cout << "<Relop>  -> " << token << std::endl;
    }
    
    
    void Expression() {
         std::cout << "<Expression>  ->  <Term><Expression'>" << std::endl;
        
         Term();
         ExpressionP();
        
    }
    
    
    void ExpressionP() {
        if (token.lexeme == "+") {
            std::cout << "<Expression'>  ->  +<Term><Expression'>" << std::endl;
            
            Term();
            instructions.push_back({Instruction::ADD, NULL});
            ExpressionP();
        }
        else if (token.lexeme == "-") {
            std::cout << "<Expression'>  ->  -<Term><Expression'>" << std::endl;
           
            Term();
            instructions.push_back({Instruction::SUB, NULL});
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
        if (token.lexeme == "*") {
            std::cout << "<Term'>  ->  *<Factor><Term'>" << std::endl;
            
            Factor();
            instructions.push_back({Instruction::MUL, NULL});
            TermP();
        }
        else if (token.lexeme == "/") {
            std::cout << "<Term'>  ->  /<Factor><Term'>" << std::endl;
            
            Factor();
            instructions.push_back({Instruction::DIV, NULL});
            TermP();
        }
        else {
            std::cout << "<Term'>  ->  <Empty>" << std::endl;
            
            Empty();
        }
    }
    
    
    void Factor() {
        lexer(&token);
        
        if (token.lexeme == "-") {
            std::cout << "<Factor>  ->  -<Primary>" << std::endl;
            
            lexer(&token);
            Primary();
        }
        else {
            std::cout << "<Factor>  ->  <Primary>" << std::endl;
            
            Primary();
        }
        
        lexer(&token);
        
    }
    
    
    void Primary() {
        
        if (isidentifier(token.lexeme)) {
            std::cout << "<Primary>  ->  <Identifier>" << std::endl;
            
            Identifier();
            
            instructions.push_back({Instruction::PUSHM, symbols[token.lexeme].address});
        }
        else if (isinteger(token.lexeme)) {
            std::cout << "<Primary>  ->  <Integer>" << std::endl;
            
            Integer();
            
            instructions.push_back({Instruction::PUSHI, stoul(token.lexeme)});
        }
        else if (token.lexeme == "true" || token.lexeme == "false") {
            std::cout << "<Primary>" << std::endl;
            
            std::cout << token << std::endl;
        }
        else if (token.lexeme == "(") {
            Expression();

            expect(token, ")", "Expected )");
            
            std::cout << token << std::endl; // )
            
        }
        else {
            
            expect(token, false, "Expected id, integer, true|false, or '('");
            
        }
    }
    
    
    void Integer() {
         std::cout << "<Integer>  -> " << token << std::endl;
    }
    
    
    void Empty() {
         std::cout << "<Empty>  ->  ε" << std::endl;
    }
    
};

std::ostream & operator<<(std::ostream &os, Parser::Instruction &t) {

    switch(t.op)
    {
        case Parser::Instruction::PUSHI:     os << "PUSHI";  break;
        case Parser::Instruction::PUSHM:     os << "PUSHM";  break;
        case Parser::Instruction::POPM:      os << "POPM";   break;
        case Parser::Instruction::STDOUT:    os << "STDOUT"; break;
        case Parser::Instruction::STDIN:     os << "STDIN";  break;
        case Parser::Instruction::ADD:       os << "ADD";    break;
        case Parser::Instruction::SUB:       os << "SUB";    break;
        case Parser::Instruction::MUL:       os << "MUL";    break;
        case Parser::Instruction::DIV:       os << "DIV";    break;
        case Parser::Instruction::GRT:       os << "GRT";    break;
        case Parser::Instruction::LES:       os << "LES";    break;
        case Parser::Instruction::EQU:       os << "EQU";    break;
        case Parser::Instruction::JUMPZ:     os << "JUMPZ";  break;
        case Parser::Instruction::JUMP:      os << "JUMP";   break;
        case Parser::Instruction::LABEL:     os << "LABEL";  break;
            
        default:         os.setstate(std::ios_base::failbit);
    }
    
    if (t.operand > 0) std::cout << t.operand;
    
    std::cout << std::endl;
    
    return os;
}

std::ostream &operator<<(std::ostream &os, Parser::SyntaxError &e) {
    os << "\nsyntax error in line " << e.lineNumber << ":\t" << e.line << "\n" << e.error_msg << std::endl;
    return os;
}


#endif /* parser_h */
