cmake_minimum_required(VERSION 3.0)
project(BrainFuckInterpreter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(SOURCES
        src/Interpreter.cpp
        src/BrainFuck.cpp
        src/Runner.cpp
)

add_executable(bf_interpreter $(SOURCES))