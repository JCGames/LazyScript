#include <iostream>
#include <vector>
#include <string.h>
#include "../include/parser.hpp"
#include "../include/node.hpp"
#include "../include/ast-generator.hpp"
#include "../include/runner.hpp"

using namespace std;

bool DEBUG_MODE = false;

int main(int argc, char* argv[])
{
    // options
    for (int i = 0; i < argc; ++i)
    {
        if (strcmp(argv[i], "-d") == 0)
            DEBUG_MODE = true;
    }

    // error checking
    if (argc < 2)
    {
        fprintf(stderr, "No file name provided!");
        return 1;
    }

    string fileName = argv[1];
    int indexOfPeriod = fileName.find_first_of(".");

    if (indexOfPeriod == -1)
    {
        fprintf(stderr, "File must be a .lzy file!");
        return 1;
    }

    string fileNameExt = fileName.substr(indexOfPeriod, fileName.size() - indexOfPeriod);

    if (fileNameExt != ".lzy")
    {
        fprintf(stderr, "File must be a .lzy file!");
        return 1;
    }

    // parse tokens
    Parser parser;
    vector<Token> tokens = parser.parseTokens(argv[1]);

    if (DEBUG_MODE)
    {
        printf("\n");
        printf("------------- Parsed Tokens --------------\n");
        parser.printTokens(&tokens);
    }

    // generate ast and run
    Generator generator(&tokens);
    Block* root = generator.generateAST();
    
    if (DEBUG_MODE)
    {   
        printf("\n");
        printf("---------- Abstract Syntax Tree ----------\n");
        generator.printBlock(root, 0);
    }

    // Runner runner;
    // runner.run(root);

    // CLEAN UP
    delete root;

    return 0;
}