
#include <iostream>
#include <fstream>
#include "lexer.h"
#include "parser.h"

int main(int argc, const char * argv[]) {
    
    std::string filename = "";
    
    std::ifstream ifile;
    
    std::cout << "Enter filename (.rat file): ";
    std::getline(std::cin, filename);
    
    ifile.open(filename);

    Parser parser(ifile);
    
    parser();
    
    ifile.close();
}
