#include<iostream>
#include<vector>
#include<cstdint>
#include<string>
#include<fstream>

class BrainFuck{
        private:
                std::vector<std::uint8_t> buffer {};
                size_t pointerHead{};
        public:
                BrainFuck(){
                        buffer.push_back(0);
                }
                void moveRight();
                void moveLeft();
                std::uint8_t read() const;
                void write(uint8_t value);
};

void BrainFuck::moveRight(void){
        pointerHead++;
        if(pointerHead >= buffer.size()){
                buffer.push_back(0);
        }
}

void BrainFuck::moveLeft(void){
        if(pointerHead == 0){
                throw std::out_of_range("BrainFuck Error: Trying to move pointer head to negative indices");
        }
        pointerHead--;
}

std::uint8_t BrainFuck::read() const{
        return buffer[pointerHead];
}

void BrainFuck::write(std::uint8_t value){
        buffer[pointerHead] = value;
}

void jump(){}


class Interpreter{
        public:
                Interpreter(const std::string& filename){
                        std::ifstream file{filename};
                        if(!file.is_open()){
                                throw std::runtime_error("File Error: Error opening file");
                        }
                }
};