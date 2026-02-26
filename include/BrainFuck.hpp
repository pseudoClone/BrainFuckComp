#ifndef BRAINFUCK_HPP
#define BRAINFUCK_HPP

#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
#include <fstream>
#include <algorithm>

class BrainFuck
{
private:
        std::vector<std::uint8_t> buffer{};
        size_t pointerHead{};

public:
        /*
        @brief Initialize at least one cell upon program execution
         */
        BrainFuck()
        {
                buffer.push_back(0);
        }
        /*
        @brief Handle right move keyword token
         */
        void moveRight(void);
        /*
        @brief Handle left move keyword token
         */
        void moveLeft(void);
        /*
        @brief Handle "," token/keyword for reading from stdin
         */
        std::uint8_t read(void) const;
        /*
        @brief Handle "." move keyword token for writing to stdout
         */
        void write(uint8_t value);
        /*
        @brief Handle cell value increment token
         */
        void increment(void);
        /*
        @brief Handle cell value decrement keyword token
         */
        void decrement(void);
};

#endif /* BRAINFUCK_HPP */