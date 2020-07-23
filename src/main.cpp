
#include <iostream>
#include <fstream>
#include <cstdio>
#include "lexer.h"
#include "parser.h"

int main(int argc, const char * argv[]) {
    
    std::string filename = "";
    
    std::ifstream ifile;
    
//    std::cout << "Enter filename (.rat file): ";
//    std::getline(std::cin, filename);
//    ifile.open(filename);
    
    ifile.open("/Users/naoki_atkins/Documents/Code/CPSC323-Compilers-Project/test/correct.rat");

    // Parser parser(ifile);

    Lexer lexer(ifile);

    // Lexer::OutputType token1 = lexer();
    // std::cout << token1 << std::endl;
    // Lexer::OutputType token2 = lexer();
    // std::cout << token2 << std::endl;
    // Lexer::OutputType token3 = lexer();
    // std::cout << token3 << std::endl;
    // Lexer::OutputType token4 = lexer();
    // std::cout << token4 << std::endl;

    // lexer.backUp(token2);

    // Lexer::OutputType token5 = lexer();
    // std::cout << token5 << std::endl;

    // lexer.backUp(token1);

    // Lexer::OutputType token6 = lexer();
    // std::cout << token6 << std::endl;


    while (!ifile.eof()) {
        Lexer::OutputType token = lexer();
        std::cout << "Token : " << token;
        // std::cout << "Line number : " << lexer.lineNum << "\n\n";
        std::cout << ifile.tellg() << std::endl;
    }

    // Lexer ErrorOutput();

    
    // parser();

    // std::cout << "Hello world" << std::endl;
    
    ifile.close();
}
