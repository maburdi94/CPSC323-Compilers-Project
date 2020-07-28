
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
    
    ifile.open("/Users/naoki_atkins/Documents/Code/CPSC323-Compilers-Project/test/test4.rat");

    // Parser parser(ifile);

    Lexer lexer(ifile);

    Lexer::OutputType* ptr;

    ptr[0] = lexer();
    ptr[1] = lexer();
    ptr[2] = lexer();
    ptr[3] = lexer();
    ptr[4] = lexer();

    for (int i = 0; i < 5; i++) {
        std::cout << i << " : " << ptr[i];
    }

    lexer.backUp(ptr[4]);
    lexer.backUp(ptr[3]);

    Lexer::OutputType token1 = lexer();
    std::cout << token1;

    Lexer::OutputType token2 = lexer();
    std::cout << token2;

    // while (!ifile.eof()) {
    //     Lexer::OutputType token = lexer();
    //     std::cout << "Token : " << token;
    //     // std::cout << "Line number : " << lexer.lineNum << "\n\n";
    //     std::cout << ifile.tellg() << std::endl;
    // }

    // Lexer ErrorOutput();

    
    // parser();

    // std::cout << "Hello world" << std::endl;

    // delete[] ptr;
    
    ifile.close();
}
