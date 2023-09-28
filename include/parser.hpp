#ifndef PARSER
#define PARSER

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include "token.hpp"

class Parser
{
    std::ifstream fin;
    bool isEOF;
    char c;
    int currentLine;

    void tryPushBuffer(TokenType type, std::string& buffer, int line, std::vector<Token>& tokens);
    void updateParseValues();

    public:
        Parser();
        ~Parser();

        std::vector<Token> parseTokens(const char* filename);
        void printTokens(std::vector<Token>* tokens);
};

#endif // PARSER