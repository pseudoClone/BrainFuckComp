#include "JIT.hpp"
#include <stdexcept>
#include <stack>

JIT::BF_OPERATION JIT::charToOP(char c)
{
        switch (c)
        {
        case '>':
                return JIT::BF_OPERATION::OP_MOVE_RIGHT;
        case '<':
                return JIT::BF_OPERATION::OP_MOVE_LEFT;
        case '+':
                return JIT::BF_OPERATION::OP_INC;
        case '-':
                return JIT::BF_OPERATION::OP_DEC;
        case '.':
                return JIT::BF_OPERATION::OP_WRITE;
        case ',':
                return JIT::BF_OPERATION::OP_READ;
        case '[':
                return JIT::BF_OPERATION::OP_JZ;
        case ']':
                return JIT::BF_OPERATION::OP_JNZ;

        default:
                throw std::invalid_argument("Invalid BF Keyword");
        }
}

void JIT::parse(const std::string &src)
{
        ir.clear(); // Remove if existing. I fear buffer overflows
        std::string clean{};
        for (auto c : src)
        {
                // Basically Python equivalent of if(c not in "+-><.,[]")
                // npos denotes there exist no position for the given character in the string container
                if (std::string("><+-.,[]").find(c) != std::string::npos)
                {
                        clean.push_back(c);
                }
        }
        std::vector<size_t> bracketMatch(clean.size(), 0); // (Size, fill)
        std::stack<size_t> bracketStack{};

        for (size_t i = 0; i < clean.size(); ++i)
        {
                if (clean[i] == '[')
                {
                        bracketStack.push(i);
                }
                else if (clean[i] == ']')
                {
                        if (bracketStack.empty())
                        {
                                throw std::runtime_error("Error: Unmatched ]");
                        }
                        size_t open = bracketStack.top();
                        bracketMatch[open] = i;
                        bracketMatch[i] = open;
                }
        }
        if (!bracketStack.empty())
        {
                throw std::runtime_error("Error: Unmatched brackets");
        }

        for (size_t i = 0; i < clean.size();)
        { // We will increase i based on RLE and not one at a time because we don't know if count is 0 or 1, 2, etc.
                char c = clean[i];
                BF_OPERATION op = charToOP(c);

                if (op == JIT::BF_OPERATION::OP_INC || op == JIT::BF_OPERATION::OP_DEC || op == JIT::BF_OPERATION::OP_MOVE_LEFT || op == JIT::BF_OPERATION::OP_MOVE_RIGHT)
                {
                        size_t count_op = 1;
                        while (i + count_op < clean.size() && clean[i + count_op] == c)
                        {
                                ++count_op;
                        }
                        ir.push_back({op, count_op});
                        i += count_op;
                }
                else if (op == OP_JZ || op == OP_JNZ)
                {
                        ir.push_back({op, bracketMatch[i]}); // Find the precomputed bracketMatch and put it in ir
                        ++i;
                }
                else
                {
                        ir.push_back({op, 0});
                }
        }
}