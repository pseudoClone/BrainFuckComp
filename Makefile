CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -02
TARGET = app
OBJS = interpreter.o utils.o

all: $(TARGET)

$(TARGET) : $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

interpreter.o: interpreter.cpp utils.h
	$(CXX) $(CXXFLAGS) -c interpreter.cpp

utils.o: utils.cpp utils.h
	$(CXX) $(CXXFLAGS) -c utils.cpp