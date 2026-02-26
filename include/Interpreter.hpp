#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "BrainFuck.hpp"
#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
#include <fstream>
#include <algorithm>
#include <stack>
#include "JIT.hpp"
#include <optional>

class Interpreter
{
private:
        BrainFuck bf{};

        std::string program{};                     // Program buffer, load program here
        std::vector<size_t> bracketMatchingDict{}; // For matching the opening and closing brackets

        std::optional<JIT> jit{};
        bool useJIT{};

        void executeInterpreted(void);
        void executeJIT(void);

public:
        Interpreter(const std::string &filename);
        /*
        @brief Load program into string buffer
        */
        void loadProgram(const std::string &file);
        /*
        @brief Map the idx of opening bracket to closing bracket
        Precomputes the map before interpretation
         */
        void bracketMatch(void);
        /*
        @brief Token consuming function
         */
        void execute(void);
        inline void enableJIT()
        {
                useJIT = true;
        }
};
#endif /* INTERPRETER_HPP */