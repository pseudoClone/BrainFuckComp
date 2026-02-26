#ifndef JIT_HPP
#define JIT_HPP
#include <cstdint>
#include <unordered_map>
#include <string>
#include <vector>
class JIT
{
private:
        typedef enum
        {
                OP_MOVE_RIGHT,
                OP_MOVE_LEFT,
                OP_INC,
                OP_DEC,
                OP_READ,
                OP_WRITE,
                OP_JNZ,
                OP_JZ
        } BF_OPERATION;
        static BF_OPERATION charToOP(char c);
        /*
        @brief Takes the BF_OPERATION and also takes argument
        @details In case of immediate operation it takes the count otherwise it take the address of the JUMPs
        */
        typedef struct
        {
                BF_OPERATION op{};
                std::size_t non_immediate_arg{};
        } IR_MNEMONIC;

        std::vector<IR_MNEMONIC> ir; // Store program to corresponding mnemonic

public:
        void parse(const std::string &src);
        void optimize(void);
        void *emit(void);
};
#endif // JIT_HPP