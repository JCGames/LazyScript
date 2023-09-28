#ifndef RUNNER
#define RUNNER

#include <iostream>
#include <string>
#include <stack>
#include <map>
#include "../include/node.hpp"

class Runner
{
    

    /// @brief Runs a block
    /// @param block The block to run
    /// @param _return The return value of the block
    /// @return True if the block returned a value, false if the block did not return a value
    bool runBlock(Block* block);

    public:
        void run(Block* mainBlock);        
};

#endif // RUNNER