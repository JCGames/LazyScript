#include "../include/node.hpp"

std::string nodeTypeToString(NodeType type)
{
    switch (type)
    {
        case NodeType::NONE: return "None";
        case NodeType::BLOCK: return "Block";
        case NodeType::BOOLEAN: return "Boolean";
        case NodeType::NUMBER: return "Number";
        case NodeType::STRING: return "String";
        case NodeType::VARIABLE: return "Variable";
        case NodeType::ASSIGNMENT: return "Assignment";
        case NodeType::ADD: return "Add";
        case NodeType::SUBTRACT: return "Subtract";
        case NodeType::MULTIPLY: return "Multiply";
        case NodeType::DIVIDE: return "Divide";
        case NodeType::MODULUS: return "Modulus";
        case NodeType::POWER: return "Power";
        case NodeType::NEGATE: return "Negate";
        case NodeType::FUNCTION: return "Function";
        case NodeType::FUNCTION_CALL: return "FunctionCall";
    }

    return "Null";
}   

Node::~Node()
{
    
}

NodeType Node::getType() const
{
    return NodeType::NONE;
}

// Block Class
#pragma region Block

Block::Block() { }
Block::~Block() 
{
    for (int i = 0; i < this->nodes.size(); ++i)
        delete nodes[i];
}

std::vector<Node*>& Block::getNodes() 
{
    return this->nodes;
}

void Block::addNode(Node* node)
{
    this->nodes.push_back(node);
}

NodeType Block::getType() const
{
    return NodeType::BLOCK;
}

#pragma endregion

// Boolean Class
#pragma region Boolean

Boolean::Boolean() 
{
    this->value = false;
}

Boolean::Boolean(bool value)
{
    this->value = value;
}

Boolean::~Boolean() { }

bool Boolean::getValue() const
{
    return this->value;
}

void Boolean::setValue(bool value)
{
    this->value = value;
}

NodeType Boolean::getType() const
{
    return NodeType::BOOLEAN;
}

#pragma endregion

// Number Class
#pragma region Number

Number::Number() 
{
    this->value = 0;
}

Number::Number(double value) 
{
    this->value = value;
}

Number::~Number() { }

double Number::getValue() const
{
    return this->value;
}

void Number::setValue(double value)
{
    this->value = value;
}

double Number::toNumber(Node* node)
{
    NodeType type = node->getType();

    if (type == NodeType::NUMBER)
    {
        return ((Number *)node)->getValue();
    }
    else if (type == NodeType::BOOLEAN)
    {
        return ((Boolean *)node)->getValue();
    }
    else
    {
        fprintf(stderr, "Cannot cast %s to number!", nodeTypeToString(node->getType()).c_str());
        exit(1);
    }
}

NodeType Number::getType() const
{
    return NodeType::NUMBER;
}

#pragma endregion

// String Class
#pragma region String

String::String()
{
    this->value = "";
}

String::String(std::string value)
{
    this->value = value;
}

String::~String() { }

std::string String::getValue() const
{
    return this->value;
}

void String::setValue(std::string value)
{
    this->value = value;
}

NodeType String::getType() const
{
    return NodeType::STRING;
}

#pragma endregion

// Variable Class
#pragma region Variable

Variable::Variable()
{
    this->name = "";
}

Variable::Variable(std::string name)
{
    this->name = name;
}

Variable::~Variable() { }

std::string Variable::getName() const
{
    return this->name;
}

void Variable::setName(std::string name)
{
    this->name = name;
}

NodeType Variable::getType() const
{
    return NodeType::VARIABLE;
}

#pragma endregion

// DoubleOperator Class
#pragma region DoubleOperator

DoubleOperator::DoubleOperator(NodeType type, Node* left, Node* right)
{
    this->type = type;
    this->left = left;
    this->right = right;
}

DoubleOperator::DoubleOperator(NodeType type)
{
    this->type = type;
    this->left = nullptr;
    this->right = nullptr;
}

DoubleOperator::~DoubleOperator()
{
    delete left;
    delete right;
}

Node* DoubleOperator::getLeft() const
{
    return this->left;
}

Node* DoubleOperator::getRight() const
{
    return this->right;
}

void DoubleOperator::setLeft(Node* left)
{
    this->left = left;
}

void DoubleOperator::setRight(Node* right)
{
    this->right = right;
}

NodeType DoubleOperator::getType() const
{
    return this->type;
}

#pragma endregion

// SingleOperator Class
#pragma region SingleOperator

SingleOperator::SingleOperator()
{
    this->node = nullptr;
}

SingleOperator::SingleOperator(Node* node)
{
    this->node = node;
}

SingleOperator::~SingleOperator()
{
    delete node;
}

Node* SingleOperator::getNode() const
{
    return this->node;
}

void SingleOperator::setNode(Node* node)
{
    this->node = node;
}

NodeType SingleOperator::getType() const
{
    return NodeType::NEGATE;
}

#pragma endregion

// Function Class
#pragma region Function

Function::Function() { }
Function::~Function() { }

NodeType Function::getType() const
{
    return NodeType::FUNCTION;
}

#pragma endregion

// FunctionCall Class
#pragma region FunctionCall

FunctionCall::FunctionCall()
{
    this->functionName = "";
}

FunctionCall::FunctionCall(std::string functionName, std::vector<Node*> parameterList)
{
    this->functionName = functionName;
    this->parameterList = parameterList;
}

FunctionCall::FunctionCall(std::string functionName)
{
    this->functionName = functionName;
}

FunctionCall::~FunctionCall()
{
    for (int i = 0; i < parameterList.size(); ++i)
        delete parameterList[i];
}

std::string FunctionCall::getName() const
{
    return this->functionName;
}

int FunctionCall::getParameterCount() const
{
    return this->parameterList.size();
}

std::vector<Node*>& FunctionCall::getParameters()
{
    return this->parameterList;
}

void FunctionCall::setName(std::string functionName)
{
    this->functionName = functionName;
}

void FunctionCall::addParameter(Node* node)
{
    this->parameterList.push_back(node);
}

NodeType FunctionCall::getType() const
{
    return NodeType::FUNCTION_CALL;
}

#pragma endregion
