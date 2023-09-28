#include "../include/parser.hpp"

Parser::Parser()
{
    isEOF = false;
    c = '\0';
    currentLine = 1;
}

Parser::~Parser()
{

}

/// @brief Will only push back the buffer if it is not empty
void Parser::tryPushBuffer(TokenType type, std::string& buffer, int line, std::vector<Token>& tokens)
{
    if (buffer.empty()) return;

    if (buffer == "true" || buffer == "false")
    {
        tokens.push_back({ TokenType::BOOLEAN, buffer, line });
        buffer.clear();
        return;
    }
    else if (buffer == "if")
    {
        tokens.push_back({ TokenType::KEYWORD, buffer, line });
        buffer.clear();
        return;
    }
    else if (buffer == "and")
    {
        tokens.push_back({ TokenType::OP_AND, buffer, line });
        buffer.clear();
        return;
    }
    else if (buffer == "or")
    {
        tokens.push_back({ TokenType::OP_OR, buffer, line });
        buffer.clear();
        return;
    }
    else if (buffer == "band")
    {
        tokens.push_back({ TokenType::OP_BAND, buffer, line });
        buffer.clear();
        return;
    }
    else if (buffer == "bor")
    {
        tokens.push_back({ TokenType::OP_BOR, buffer, line });
        buffer.clear();
        return;
    }
    else if (buffer == "bxor")
    {
        tokens.push_back({ TokenType::OP_BXOR, buffer, line });
        buffer.clear();
        return;
    }
    else if (buffer == "bls")
    {
        tokens.push_back({ TokenType::OP_BLS, buffer, line });
        buffer.clear();
        return;
    }
    else if (buffer == "brs")
    {
        tokens.push_back({ TokenType::OP_BRS, buffer, line });
        buffer.clear();
        return;
    }
    else if (buffer == "binv")
    {
        tokens.push_back({ TokenType::OP_BINV, buffer, line });
        buffer.clear();
        return;
    }
    else if (buffer == "return")
    {
        tokens.push_back({ TokenType::KEYWORD, buffer, line });
        buffer.clear();
        return;
    }

    tokens.push_back({ type, buffer, line });
    buffer.clear();
}

void Parser::updateParseValues()
{
    isEOF = fin.eof();
    c = fin.get();
}

// public
std::vector<Token> Parser::parseTokens(const char* filename)
{
    fin.open(filename);

    if (!fin.is_open())
    {
        fprintf(stderr, "File could not be opened!\n");
        exit(1);
    }
    
    std::vector<Token> resultVector;

    std::string buffer;

    while (true)
    {
        updateParseValues();

        // printf("%i\n", c);

        // END_OF_LINE
        if (c == '\n')
        {
            tryPushBuffer(TokenType::STATEMENT, buffer, currentLine, resultVector);
            resultVector.push_back({ TokenType::END_OF_LINE, "END_OF_LINE", currentLine });
            ++currentLine;
        }

        // END_OF_FILE
        else if (c == -1)
        {
            tryPushBuffer(TokenType::STATEMENT, buffer, currentLine, resultVector);
            resultVector.push_back({ TokenType::END_OF_FILE, "END_OF_FILE", currentLine });
            break;
        }

        // EQUALS
        else if (c == '=' && fin.peek() == '=')
        {
            tryPushBuffer(TokenType::STATEMENT, buffer, currentLine, resultVector);
            c = fin.get();
            resultVector.push_back({ TokenType::OP_EQUALS, "==", currentLine });
        }

        // NOT EQUAL
        else if (c == '!' && fin.peek() == '=')
        {
            tryPushBuffer(TokenType::STATEMENT, buffer, currentLine, resultVector);
            c = fin.get();
            resultVector.push_back({ TokenType::OP_NOT_EQUAL, "!=", currentLine });
        }

        // LESS THAN OR EQUAL OPERATOR
        else if (c == '<' && fin.peek() == '=')
        {
            tryPushBuffer(TokenType::STATEMENT, buffer, currentLine, resultVector);
            c = fin.get();
            resultVector.push_back({ TokenType::OP_LTHANE, "<=", currentLine });
        }

        // GREATER THAN OR EQUAL OPERATOR
        else if (c == '>' && fin.peek() == '=')
        {
            tryPushBuffer(TokenType::STATEMENT, buffer, currentLine, resultVector);
            c = fin.get();
            resultVector.push_back({ TokenType::OP_GTHANE, ">=", currentLine });
        }

        // INCREMENT
        else if (c == '+' && fin.peek() == '+')
        {
            tryPushBuffer(TokenType::STATEMENT, buffer, currentLine, resultVector);
            c = fin.get();
            resultVector.push_back({ TokenType::OP_INC, "++", currentLine });
        }

        // DEINCREMENT
        else if (c == '-' && fin.peek() == '-')
        {
            tryPushBuffer(TokenType::STATEMENT, buffer, currentLine, resultVector);
            c = fin.get();
            resultVector.push_back({ TokenType::OP_DINC, "--", currentLine });
        }

        // ASSIGNMENT OPERATOR
        else if (c == '=')
        {
            tryPushBuffer(TokenType::STATEMENT, buffer, currentLine, resultVector);
            resultVector.push_back({ TokenType::OP_ASSIGNMENT, "=", currentLine });
        }

        // LESS THAN OPERATOR
        else if (c == '<')
        {
            tryPushBuffer(TokenType::STATEMENT, buffer, currentLine, resultVector);
            resultVector.push_back({ TokenType::OP_LTHAN, "<", currentLine });
        }

        // GREATER THAN OPERATOR
        else if (c == '>')
        {
            tryPushBuffer(TokenType::STATEMENT, buffer, currentLine, resultVector);
            resultVector.push_back({ TokenType::OP_GTHAN, ">", currentLine });
        }

        // LOGICAL NOT
        else if (c == '!')
        {
            tryPushBuffer(TokenType::STATEMENT, buffer, currentLine, resultVector);
            resultVector.push_back({ TokenType::OP_LOGICAL_NOT, "!", currentLine });
        }

        // ADD OPERATOR
        else if (c == '+')
        {
            tryPushBuffer(TokenType::STATEMENT, buffer, currentLine, resultVector);
            resultVector.push_back({ TokenType::OP_ADD, "+", currentLine });
        }

        // SUBTRACT OPERATOR
        else if (c == '-')
        {
            tryPushBuffer(TokenType::STATEMENT, buffer, currentLine, resultVector);
            resultVector.push_back({ TokenType::OP_SUB, "-", currentLine });
        }

        // MULTIPLY OPERATOR
        else if (c == '*')
        {
            tryPushBuffer(TokenType::STATEMENT, buffer, currentLine, resultVector);
            resultVector.push_back({ TokenType::OP_MULT, "*", currentLine });
        }

        // DIVIDE OPERATOR
        else if (c == '/')
        {
            tryPushBuffer(TokenType::STATEMENT, buffer, currentLine, resultVector);
            resultVector.push_back({ TokenType::OP_DIV, "/", currentLine });
        }

        // MODULUS OPERATOR
        else if (c == '%')
        {
            tryPushBuffer(TokenType::STATEMENT, buffer, currentLine, resultVector);
            resultVector.push_back({ TokenType::OP_MOD, "%", currentLine });
        }

        // POWER OPERATOR
        else if (c == '^')
        {
            tryPushBuffer(TokenType::STATEMENT, buffer, currentLine, resultVector);
            resultVector.push_back({ TokenType::OP_POW, "^", currentLine });
        }

        // OPEN PARAN
        else if (c == '(')
        {
            tryPushBuffer(TokenType::STATEMENT, buffer, currentLine, resultVector);
            resultVector.push_back({ TokenType::OPEN_PARAN, "(", currentLine });
        }

        // CLOSE PARAN
        else if (c == ')')
        {
            tryPushBuffer(TokenType::STATEMENT, buffer, currentLine, resultVector);
            resultVector.push_back({ TokenType::CLOSE_PARAN, ")", currentLine });
        }

        // OPEN BRACK
        else if (c == '{')
        {
            tryPushBuffer(TokenType::STATEMENT, buffer, currentLine, resultVector);
            resultVector.push_back({ TokenType::OPEN_BRACK, "{", currentLine });
        }

        // OPEN BRACK
        else if (c == '}')
        {
            tryPushBuffer(TokenType::STATEMENT, buffer, currentLine, resultVector);
            resultVector.push_back({ TokenType::CLOSE_BRACK, "}", currentLine });
        }

        // OPEN BRACK
        else if (c == '[')
        {
            tryPushBuffer(TokenType::STATEMENT, buffer, currentLine, resultVector);
            resultVector.push_back({ TokenType::OPEN_SBRACK, "[", currentLine });
        }

        // OPEN BRACK
        else if (c == ']')
        {
            tryPushBuffer(TokenType::STATEMENT, buffer, currentLine, resultVector);
            resultVector.push_back({ TokenType::CLOSE_SBRACK, "]", currentLine });
        }

        // COMMA
        else if (c == ',')
        {
            tryPushBuffer(TokenType::STATEMENT, buffer, currentLine, resultVector);
            resultVector.push_back({ TokenType::COMMA, ",", currentLine });
        }

        // NUMBERS
        else if (isdigit(c))
        {
            if (!buffer.empty())
            {
                fprintf(stderr, "Number is not formatted correctly on line %i!", currentLine);
                exit(1);
            }

            bool hasHadDot = false;
            char peek = fin.peek();

            buffer += c;

            // IF NUMBER IS ONLY ONE DIGIT
            if (!isdigit(peek) && peek != '.')
            {
                tryPushBuffer(TokenType::NUMBER, buffer, currentLine, resultVector);
            }

            // IF NUMBER IS MORE THAN ONE DIGIT
            else
            {
                while (true)
                {
                    updateParseValues();

                    peek = fin.peek();

                    if ((!isdigit(peek) && peek != '.') || (peek == '.' && hasHadDot))
                    {
                        buffer += c;
                        tryPushBuffer(TokenType::NUMBER, buffer, currentLine, resultVector);
                        break;
                    }
                    else if (c == '.')
                    {
                        hasHadDot = true;
                    }
                        
                    buffer += c;
                }
            }
        }

        // DOT
        else if (c == '.')
        {
            tryPushBuffer(TokenType::STATEMENT, buffer, currentLine, resultVector);
            resultVector.push_back({ TokenType::DOT, ".", currentLine });
        }

        // STRINGS
        else if (c == '"')
        {
            // in case any value was being read directly to the left of the string
            tryPushBuffer(TokenType::STATEMENT, buffer, currentLine, resultVector);
            
            while (true)
            {
                updateParseValues();

                if (isEOF || c == '\n')
                {
                    fprintf(stderr, "String on line %i, was never closed!", currentLine);
                    exit(1);
                }
                else if (c == '"')
                {
                    resultVector.push_back({ TokenType::STRING, buffer, currentLine });
                    buffer.clear();
                    break;
                }
                else
                {
                    buffer += c;
                }
            }
        }

        // STATEMENTS
        else if (!iswspace(c))
        {
            buffer += c;
        }

        // PUSHING BUFFER TO RESULT VECTOR: when whitespace is encountered
        else
        {
            tryPushBuffer(TokenType::STATEMENT, buffer, currentLine, resultVector);
        }
    }

    return resultVector;
} 

void Parser::printTokens(std::vector<Token>* tokens)
{
    for (auto token : *tokens)
        printf("TokenType: %i, Value: |%s|, Line: %i\n", (int)token.type, token.value.c_str(), token.line);
}