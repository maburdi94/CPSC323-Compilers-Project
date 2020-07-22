
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

    ifile.open("/Users/maburdi/Documents/Education/Cal State Fullerton/Summer 2020/CPSC 323 Compilers and Programming Languages/Project/me1.txt");

    Lexer lexer(ifile);
    while (lexer) std::cout << lexer();
    
//    Parser parser(ifile);
//    parser();
    
    ifile.close();
}
