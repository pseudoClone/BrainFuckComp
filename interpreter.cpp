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
        BrainFuck()
        {
                buffer.push_back(0);
        }
        void moveRight(void);
        void moveLeft(void);
        std::uint8_t read(void) const;
        void write(uint8_t value);
        void increment(void);
        void decrement(void);
};

void BrainFuck::moveRight(void)
{
        pointerHead++;
        if (pointerHead >= buffer.size())
        {
                buffer.push_back(0);
        }
}

void BrainFuck::moveLeft(void)
{
        if (pointerHead == 0)
        {
                throw std::out_of_range("BrainFuck Error: Trying to move pointer head to negative index");
        }
        pointerHead--;
}

std::uint8_t BrainFuck::read() const
{
        return buffer[pointerHead];
}

void BrainFuck::write(std::uint8_t value)
{
        buffer[pointerHead] = value;
}

void BrainFuck::increment()
{
        buffer[pointerHead]++;
}

void BrainFuck::decrement()
{
        buffer[pointerHead]--;
}

class Interpreter
{
private:
        std::ifstream file{};

public:
        BrainFuck bf{};
        Interpreter(const std::string &filename) : file{filename}
        {
                if (!file.is_open())
                {
                        throw std::runtime_error("File Error: Error opening file");
                }
        }
        void eatChars(void)
        {
                std::string program{
                    std::istreambuf_iterator<char>{file},
                    std::istreambuf_iterator<char>{}};
                /*Begin and END iterators */

                for (size_t ip{}; ip < program.size(); ip++)
                {
                        switch (program[ip])
                        {
                        case '>':
                                bf.moveRight();
                                break;
                        case '<':
                                bf.moveLeft();
                                break;
                        case '+':
                                bf.increment();
                                break;
                        case '-':
                                bf.decrement();
                                break;
                        case '.':
                                std::cout << (char)bf.read(); /* Write to output*/
                                break;
                        case ',':
                                char input{};
                                std::cin.get(input); /* Read from input */
                                bf.write(input);
                                break;
                        default:
                                break;
                        }
                }
        }
};

int main(int argc, char **argv)
{
        //
}