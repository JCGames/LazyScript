#ifndef NODE
#define NODE

#include <string>
#include <vector>

enum class NodeType
{
    NONE,
    BLOCK,
    BOOLEAN,
    NUMBER,
    STRING,
    VARIABLE,
    ASSIGNMENT,
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    MODULUS,
    POWER,
    NEGATE,
    FUNCTION,
    FUNCTION_CALL
};

/// @brief NodeType to string
std::string nodeTypeToString(NodeType type);

class Node
{
    protected:
        NodeType type;

    public:
        virtual ~Node();

        virtual NodeType getType() const;
};

class Block : public Node
{
    std::vector<Node*> nodes;

    public:
        Block();
        ~Block();

        std::vector<Node*>& getNodes();
        void addNode(Node* node);

        NodeType getType() const;
};

class Boolean : public Node
{
    bool value;

    public:
        Boolean(bool value);
        Boolean();
        ~Boolean();

        bool getValue() const;
        void setValue(bool value);

        NodeType getType() const override;
};

class Number : public Node
{
    double value;
    
    public:
        Number(double value);
        Number();
        ~Number();

        double getValue() const;
        void setValue(double value);
        static double toNumber(Node* node);

        NodeType getType() const override;
};

class String : public Node
{
    std::string value;

    public:
        String(std::string value);
        String();
        ~String();    

        std::string getValue() const;
        void setValue(std::string value);

        NodeType getType() const override;
};

class Variable : public Node
{
    std::string name;

    public:
        Variable(std::string name);
        Variable();
        ~Variable();    

        std::string getName() const;
        void setName(std::string name);

        NodeType getType() const override;
};

/// @brief Defined as an operator with a left and right value to operate on.
class DoubleOperator : public Node
{
    Node* left;
    Node* right;

    public:
        DoubleOperator(NodeType type, Node* left, Node* right);
        DoubleOperator(NodeType type);
        ~DoubleOperator();

        Node* getLeft() const;
        Node* getRight() const;
        void setLeft(Node* left);
        void setRight(Node* right);

        NodeType getType() const override;
};

/// @brief Defined as an operator with only one value to operate on.
class SingleOperator : public Node
{
    Node* node;

    public:
        SingleOperator(Node* node);
        SingleOperator();
        ~SingleOperator();

        Node* getNode() const;
        void setNode(Node* node);

        NodeType getType() const override;
};

// NOTE: not yet defined
class Function : public Node
{
    public:
        Function();
        ~Function();

        NodeType getType() const override;
};

class FunctionCall : public Node
{
    std::string functionName;
    std::vector<Node*> parameterList;

    public:
        FunctionCall(std::string functionName, std::vector<Node*> parameterList);
        FunctionCall(std::string functionName);
        FunctionCall();
        ~FunctionCall();

        std::string getName() const;
        int getParameterCount() const;
        std::vector<Node*>& getParameters();
        void setName(std::string);
        void addParameter(Node* node);

        NodeType getType() const override;
};

#endif // NODE