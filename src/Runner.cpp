#include "BrainFuck.hpp"
#include "Interpreter.hpp"
#include <stdexcept>
#include <cstring>

int main(int argc, char const **argv)
{
        if (argc < 2)
        {
                throw std::invalid_argument("Usage: zerofucks <filename>");
                // return -1; Amateur since it is already handled
        }
        try
        {
                std::string fileName{};
                bool jitMode{};

                if (argc >= 3 && std::strcmp(argv[1], "--jit") == 0) /* Check if strcmp return 0 which is EXIT_SUCCESS ig */
                {
                        jitMode = true;
                        fileName = argv[2];
                }
                else
                {
                        fileName = argv[1]; // Best usecase for optional
                }
                Interpreter interpreter(fileName);
                if (jitMode)
                {
                        interpreter.enableJIT();
                }
                interpreter.execute();
        }

        catch (const std::exception &e)
        {
                std::cerr << e.what() << '\n';
                return -1;
        }

        return 0;
}