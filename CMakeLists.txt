cmake_minimum_required(VERSION 4.0)
project(BrainFuckInterpreter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(zerofucks 
        src/Interpreter.cpp
        src/BrainFuck.cpp
        src/JIT.cpp
        src/Runner.cpp
)

target_link_libraries(zerofucks PRIVATE dl)