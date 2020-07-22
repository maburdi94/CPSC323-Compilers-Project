#include <map>
#include <iostream>
#include <set>
#include <string>



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


int main() {
    if (follow["DeclList"].find("{") != follow["DeclList"].end()) {
        std::cout << "REturn true" << std::endl;
    }

    std::cout << "Hello" << std::endl;
    std::cout << "World" << std::endl;
    return 0;
}

