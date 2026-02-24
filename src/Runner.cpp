#include "BrainFuck.hpp"
#include "Interpreter.hpp"
#include <stdexcept>

int main(int argc, char const **argv)
{
        if (argc < 2)
        {
                throw std::invalid_argument("Usage: zerofucks <filename>");
                return -1;
        }
        try
        {
                Interpreter interpreter(argv[1]);
                interpreter.execute();
        }
        catch (const std::exception &e)
        {
                std::cerr << e.what() << '\n';
                return -1;
        }

        return 0;
}