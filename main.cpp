
#include <iostream>
#include <fstream>
#include "lexer.h"
#include "parser.h"

int main(int argc, const char * argv[]) {
    
    std::string filename = "";
    
    std::ifstream ifile;
    
//    std::cout << "Enter filename (.rat file): ";
//    std::getline(std::cin, filename);
//    ifile.open(filename);
    
    ifile.open("/Users/naoki_atkins/Documents/Code/CPSC323-Compilers-Project/test1.rat");

    Parser parser(ifile);

    Lexer lexer(ifile);

    Lexer::OutputType token1 = lexer();
    std::cout << token1 << std::endl;
    Lexer::OutputType token2 = lexer();
    std::cout << token2 << std::endl;

    lexer.backUp(token2);
    Lexer::OutputType token3 = lexer();
    std::cout << token3 << std::endl;

    // while (!ifile.eof()) {
    //     Lexer::OutputType token = lexer();
    //     std::cout << token << std::endl;
    // }

    
    // parser();
    
    ifile.close();
}
