#ifndef TOKEN
#define TOKEN

#include <string>

enum class TokenType
{
    END_OF_FILE,
    END_OF_LINE,
    STATEMENT,
    STRING,
    NUMBER,
    OP_ASSIGNMENT,
    OP_GTHAN,
    OP_LTHAN,
    OP_GTHANE,
    OP_LTHANE,
    OP_EQUALS,
    OP_LOGICAL_NOT,
    OP_NOT_EQUAL,
    OP_MULT,
    OP_DIV,
    OP_MOD,
    OP_POW,
    OP_ADD,
    OP_INC,
    OP_DINC,
    OP_SUB,
    OP_AND,
    OP_OR,
    OP_BAND,
    OP_BOR,
    OP_BXOR,
    OP_BLS,
    OP_BRS,
    OP_BINV,
    OPEN_PARAN,
    CLOSE_PARAN,
    OPEN_BRACK,
    CLOSE_BRACK,
    OPEN_SBRACK,
    CLOSE_SBRACK,
    DOT,
    BOOLEAN,
    COMMA,
    KEYWORD,
};

struct Token
{
    TokenType type;
    std::string value;
    int line;

    static bool isExpression(const Token& token)
    {
        return token.type == TokenType::OP_ADD || token.type == TokenType::OP_SUB;
    }

    static bool isFactor(const Token& token)
    {
        return token.type == TokenType::OP_MULT || token.type == TokenType::OP_DIV;
    }

    static bool isPower(const Token& token)
    {
        return token.type == TokenType::OP_POW;
    }
    
    /// @brief Is end of line or file
    static bool isEndOfLoF(const Token& token)
    {
        return token.type == TokenType::END_OF_FILE || token.type == TokenType::END_OF_LINE;
    }
};

#endif // TOKEN