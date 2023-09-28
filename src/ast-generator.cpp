#include "../include/ast-generator.hpp"

/*
* generator.cpp
* 
* In charge of generating the program AST and bytecode.
*
* Copyright 2023 - Christian Marcellino - All rights reserved.
*
*/

Generator::Generator(std::vector<Token>* tokens)
{
    this->tokens = tokens;
    this->pos = 0;
    this->tokenLineIndex = 0;
    this->flag = "";
    this->currentToken = tokens->at(pos);
}

Generator::~Generator()
{
    
}

bool Generator::hasValueAt(int index, TokenType type)
{
    if (index >= 0 && index < tokens->size())
        return tokens->at(index).type == type;
    return false;
}

bool Generator::hasValueAt(int index)
{
    if (index >= 0 && index < tokens->size())
        return true;
    return false;
}

void Generator::moveNext()
{
    if (pos < tokens->size() - 1)
    {
        if (currentToken.type == TokenType::END_OF_LINE)
            tokenLineIndex = 0;
        else
            ++tokenLineIndex;

        ++pos;
        currentToken = tokens->at(pos);
    }
}

bool Generator::tokenIsOfType(Token token, TokenType type[], int size)
{
    for (int i = 0; i < size; ++i)
    {
        if (token.type == type[i])
            return true;
    }

    return false;
}

Token Generator::peek()
{
    if (pos + 1 < tokens->size())
        return tokens->at(pos + 1);

    return tokens->at(pos);
}

/*
*
*   ----------- CREATING NODES -----------
*
*/

/// @brief Requires that the currentToken be an assignment token.
/// @param variable the STATEMENT token that begins the assignment expression.
/// @return A pointer to the new assignment node.
Node* Generator::createAssignmentAST(std::string variableName)
{
    DoubleOperator* result = new DoubleOperator(NodeType::ASSIGNMENT, new Variable(variableName), nullptr);
    
    moveNext();
    result->setRight(readExpression());

    return result;
}

Node* Generator::readExpression()
{
    Node* result = readFactor();

    if (result == nullptr)
    {
        fprintf(stderr, "Cannot parse expression on line: %i\n", currentToken.line);
        exit(1);
    }

    while (!Token::isEndOfLoF(currentToken) && Token::isExpression(currentToken))
    {
        if (currentToken.type == TokenType::OP_ADD)
        {
            moveNext();
            Node* rightNode = readFactor();
            Node* lastResult = result;
            result = new DoubleOperator(NodeType::ADD, lastResult, rightNode);
        }
        else if (currentToken.type == TokenType::OP_SUB)
        {
            moveNext();
            Node* rightNode = readFactor();
            Node* lastResult = result;
            result = new DoubleOperator(NodeType::SUBTRACT, lastResult, rightNode);
        }
    }

    return result;
}

Node* Generator::readFactor()
{
    Node* factor = readPower();

    if (factor == nullptr)
    {
        fprintf(stderr, "Could not parse factor on line: %i, token value: |%s|\n", currentToken.line, currentToken.value.c_str());
        exit(1);
    }

    while (!Token::isEndOfLoF(currentToken) && Token::isFactor(currentToken))
    {
        if (currentToken.type == TokenType::OP_MULT)
        {
            moveNext();
            Node* rightNode = readPower();
            Node* lastResult = factor;
            factor = new DoubleOperator(NodeType::MULTIPLY, lastResult, rightNode);
        }
        else if (currentToken.type == TokenType::OP_DIV)
        {
            moveNext();
            Node* rightNode = readPower();
            Node* lastResult = factor;
            factor = new DoubleOperator(NodeType::DIVIDE, lastResult, rightNode);
        }
        else if (currentToken.type == TokenType::OP_MOD)
        {
            moveNext();
            Node* rightNode = readPower();
            Node* lastResult = factor;
            factor = new DoubleOperator(NodeType::MODULUS, lastResult, rightNode);
        }
    }

    return factor;
}

Node* Generator::readPower()
{
    Node* power = readTerm();

    while (!Token::isEndOfLoF(currentToken) && Token::isPower(currentToken))
    {
        if (currentToken.type == TokenType::OP_POW)
        {
            moveNext();
            Node* rightNode = readTerm();
            Node* lastResult = power;
            power = new DoubleOperator(NodeType::POWER, lastResult, rightNode);
        }
    }

    return power;
}

Node* Generator::readTerm()
{
    Node* term = nullptr;

    if (currentToken.type == TokenType::OPEN_PARAN)
    {
        moveNext();
        term = readExpression();
        if (currentToken.type != TokenType::CLOSE_PARAN)
        {
            fprintf(stderr, "Missing ) on line: %i\n", currentToken.line);
            exit(1);
        }
    }
    else if (currentToken.type == TokenType::NUMBER)
    {
        term = new Number(std::stod(currentToken.value));
    }
    else if (currentToken.type == TokenType::BOOLEAN)
    {
        term = new Boolean(currentToken.value == "true");
    }
    else if (currentToken.type == TokenType::STRING)
    {
        term = new String(currentToken.value);
    }
    else if (currentToken.type == TokenType::STATEMENT)
    {
        if (peek().type == TokenType::OPEN_PARAN)
        {
            term = readFuncCall();
        }
        else
        {
            term = new Variable(currentToken.value);
        }
    }
    else
    {
        fprintf(stderr, "Incorrect format of expression on line: %i\n", currentToken.line);
        exit(1);
    }

    moveNext();
    return term;
}

Node* Generator::readFuncCall()
{
    FunctionCall* functionCall = new FunctionCall(currentToken.value);

    moveNext();
    moveNext();

    if (currentToken.type == TokenType::CLOSE_PARAN)
    {
        return functionCall;
    }

    while (!Token::isEndOfLoF(currentToken))
    {
        Node* parameter = readExpression();

        functionCall->addParameter(parameter);

        if (currentToken.type == TokenType::CLOSE_PARAN)
        {
            return functionCall;
        }
        else if (currentToken.type != TokenType::COMMA)
        {
            fprintf(stderr, "Parameters are not seperated with commas on line: %i\n", currentToken.line);
            exit(1);
        }

        moveNext();
    }

    fprintf(stderr, "Missing ) on line: %i\n", currentToken.line);
    exit(1);
}

/*
*
*   ----------- GENERATING AST -----------
*
*/

Block* Generator::generateAST()
{
    Block* root = new Block();
    generateASTBlock(root, 0);

    return root;
}

void Generator::generateASTBlock(Block* block, const int& level)
{
    Node* nodeToAdd = nullptr;

    while (true)
    {
        // End of block condition
        if (currentToken.type == TokenType::CLOSE_BRACK || currentToken.type == TokenType::END_OF_FILE) 
            break;
        
        // Parse function calls
        if (currentToken.type == TokenType::STATEMENT && peek().type == TokenType::OPEN_PARAN)
            block->addNode(readFuncCall());

        // Parse assignments
        else if (currentToken.type == TokenType::STATEMENT && peek().type == TokenType::OP_ASSIGNMENT)
        {
            std::string variableName = currentToken.value;
            moveNext();

            block->addNode(createAssignmentAST(variableName));

            if (!Token::isEndOfLoF(currentToken))
            {
                fprintf(stderr, "Incorrect assignment format on line: %i\n", currentToken.line);
                exit(1);
            }
        }

        // Parse sub-blocks
        else if (currentToken.type == TokenType::OPEN_BRACK)
        {
            moveNext();

            Block* childBlock = new Block(); // create sub-block
            generateASTBlock(childBlock, level + 1); // generate sub-block
            block->addNode(childBlock); // add sub-block to current blocks node list
        }

        else if (!Token::isEndOfLoF(currentToken))
        {
            fprintf(stderr, "Error on line: %i\n", currentToken.line);
            exit(1);
        }

        moveNext();
    }

    // Error checking for sub-blocks
    if (level > 0 && currentToken.type != TokenType::CLOSE_BRACK)
    {
        fprintf(stderr, "Missing } on line: %i\n", currentToken.line);
        exit(1);
    }
}

/*
*
*   ----------- PRINTING -----------
*
*/

/// @brief Prints out a node
void Generator::printBlock(Block* block, int depth)
{
    std::string calculatedPrefix = "";
    for (int i = 0; i < depth; ++i)
        calculatedPrefix += "\t";

    printf("%sBlock:\n", calculatedPrefix.c_str());

    for (int i = 0; i < block->getNodes().size(); ++i)
    {
        if (block->getNodes()[i]->getType() == NodeType::BLOCK)
        {
            printBlock((Block *)block->getNodes()[i], depth + 1);
        }
        else
        {
            printNode(block->getNodes()[i], depth + 1);
        }
    }
}

/// @brief Prints a single node to the console
/// @param node The node to print
/// @param depth The node depth
void Generator::printNode(Node* node, int depth)
{
    if (node->getType() == NodeType::BLOCK) return;

    std::string calculatedPrefix = "";
    for (int i = 0; i < depth; ++i)
        calculatedPrefix += "\t";

    const char* prefix = calculatedPrefix.c_str();

    printf("%s* %s\n", prefix, nodeTypeToString(node->getType()).c_str());

    switch (node->getType())
    {
        case NodeType::BOOLEAN:
            printf("%sBool Value: %i\n", prefix, ((Boolean *)node)->getValue());
            break;
        case NodeType::NUMBER:
            printf("%sNumber Value: %f\n", prefix, ((Number *)node)->getValue());
            break;
        case NodeType::STRING:
            printf("%sString Value: %s\n", prefix, ((String *)node)->getValue().c_str());
            break;
        case NodeType::VARIABLE:
            printf("%sName: %s\n", prefix, ((Variable *)node)->getName().c_str());
            break;
        case NodeType::ASSIGNMENT:
        case NodeType::ADD:
        case NodeType::SUBTRACT:
        case NodeType::MULTIPLY:
        case NodeType::DIVIDE:
        case NodeType::MODULUS:
        case NodeType::POWER:
            printf("%sLeft:\n", prefix);
            printNode(((DoubleOperator *)node)->getLeft(), depth + 1);
            printf("%sRight:\n", prefix);
            printNode(((DoubleOperator *)node)->getRight(), depth + 1);
            break;
        case NodeType::NEGATE:
            printf("%sNegative Of:\n", prefix);
            printNode(((SingleOperator *)node)->getNode(), depth + 1);
            break;
        case NodeType::FUNCTION:
            break;
        case NodeType::FUNCTION_CALL:
            {
                FunctionCall* functionCall = (FunctionCall *)node;

                printf("%sFunction Name: %s\n", prefix, functionCall->getName().c_str());

                for (int i = 0; i < functionCall->getParameters().size(); ++i)
                {
                    printf("%sParameter: %i\n", prefix, i);
                    printNode(functionCall->getParameters()[i], depth + 1);
                }
            }
            break;
    }
}