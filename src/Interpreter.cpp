#include "Interpreter.hpp"
#include <sys/mman.h> /*This is for mmap. Fucking awesome function */

Interpreter::Interpreter(const std::string &filename)
{
        loadProgram(filename);
        bracketMatch();
}

void Interpreter::loadProgram(const std::string &filename)
{
        std::ifstream fileObj{filename}; /* Naming confusion, don't fucking use file as the object identifier */
        if (!fileObj)
        {
                throw std::runtime_error("Interpreter Error: Error opening file");
        }
        program.assign(std::istreambuf_iterator<char>{fileObj}, std::istreambuf_iterator<char>{});

        /* Whitespace and trailing spaces erasure*/
        std::string clearedProgram{};
        for (char ch : program)
        {
                if (ch == '>' || ch == '<' || ch == '+' || ch == '-' || ch == '.' || ch == ',' || ch == '[' || ch == ']')
                {
                        clearedProgram += ch;
                }
        }
        program = std::move(clearedProgram);
}

void Interpreter::bracketMatch()
{
        bracketMatchingDict.resize(program.size());
        std::stack<size_t> stack{};

        for (size_t i = 0; i < program.size(); ++i)
        {
                if (program[i] == '[')
                {
                        stack.push(i); // We are storing location using push, not the value
                }
                else if (program[i] == ']')
                {
                        if (stack.empty())
                        {
                                throw std::out_of_range("Matching bracket not found");
                        }
                        auto match = stack.top();
                        stack.pop();
                        bracketMatchingDict[match] = i;
                        bracketMatchingDict[i] = match;
                        // Could have done this efficiently with legit dictionaries, but this is faster
                }
        }
        // If still not emptied, we are in trouble
        if (!stack.empty())
        {
                throw std::out_of_range("Matching bracket not found");
        }
}

void Interpreter::execute(void)
{
        if (useJIT)
                executeJIT();
        else
                executeInterpreted();
}

void Interpreter::executeInterpreted(void)
{
        size_t tok_idx{};
        while (tok_idx < program.size())
        {
                switch (program[tok_idx])
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
                {
                        char input{};
                        if (!std::cin.get(input))
                        {
                                bf.write(0);
                        } /* Read from input */
                        else
                        {
                                bf.write((uint8_t)input);
                        }
                        break;
                }
                case '[':
                {
                        if (bf.read() == 0)
                        {
                                tok_idx = bracketMatchingDict[tok_idx];
                        }
                        break;
                }
                case ']':
                {
                        if (bf.read() != 0)
                        {
                                tok_idx = bracketMatchingDict[tok_idx];
                        }
                        break;
                }
                }
                ++tok_idx;
        }
}
void Interpreter::executeJIT(void)
{
        if (program.find('[') != std::string::npos)
        {
                executeInterpreted();
                return;
        }
        jit.emplace(); // No program here, overload BTW
        jit->parse(program);
        jit->optimize();

        void *func = jit->emit();
        if (!func)
        {
                executeInterpreted();
                return;
        }

        auto &tape = bf.getBuffer();
        if (tape.size() < 30000)
        {
                tape.resize(30000);
        }

        using JitFunc = void (*)(uint8_t *, uint8_t *);
        auto jitted = reinterpret_cast<JitFunc>(func);

        uint8_t *tape_base = bf.data();
        uint8_t *data_ptr = tape_base + bf.getPointerHead();

        jitted(tape_base, data_ptr);

        bf.setPointerHead(data_ptr - tape_base);

        munmap(func, 4096);
}