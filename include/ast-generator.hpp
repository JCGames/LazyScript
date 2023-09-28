#ifndef GENERATOR
#define GENERATOR

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <typeinfo>
#include <stack>
#include "node.hpp"
#include "token.hpp"

class Generator
{
    std::vector<Token>* tokens;
    int pos;
    int tokenLineIndex;
    Token currentToken;

    std::string flag;

    bool hasValueAt(int index, TokenType type);
    bool hasValueAt(int index);
    void printNode(Node* current, int depth);
    void generateByteCodeForBlock(Node* block, std::ofstream& fout);
    void generateByteCodeForNode(Node* node, std::ofstream& fout);
    bool tokenIsOfType(Token token, TokenType type[], int size);
    void generateASTBlock(Block* block, const int& level);
    Token peek();

    void moveNext();

    // RULES
    Node* createAssignmentAST(std::string variableName);
    Node* readExpression();
    Node* readFactor();
    Node* readPower();
    Node* readTerm();
    Node* readFuncCall();

    public:
        Generator(std::vector<Token>* tokens);
        ~Generator();

        Block* generateAST();
        void printBlock(Block* block, int depth);

        void generateByteCode(Node* root, const char* filename);
};

#endif // GENERATOR