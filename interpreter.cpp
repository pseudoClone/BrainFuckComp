#include<iostream>
#include<vector>
#include<cstdint>
#include<string>
#include<fstream>
#include<algorithm>

class BrainFuck{
        private:
                std::vector<std::uint8_t> buffer {};
                size_t pointerHead{};
        public:
                BrainFuck(){
                        buffer.push_back(0);
                }
                void moveRight(void);
                void moveLeft(void);
                std::uint8_t read(void) const;
                void write(uint8_t value);
                void increment(void);
                void decrement(void);
                void loop(void);
};

void BrainFuck::moveRight(void){
        pointerHead++;
        if(pointerHead >= buffer.size()){
                buffer.push_back(0);
        }
}

void BrainFuck::moveLeft(void){
        if(pointerHead == 0){
                throw std::out_of_range("BrainFuck Error: Trying to move pointer head to negative index");
        }
        pointerHead--;
}

std::uint8_t BrainFuck::read() const{
        return buffer[pointerHead];
}

void BrainFuck::write(std::uint8_t value){
        buffer[pointerHead] = value;
}

void BrainFuck::increment(){
        buffer[pointerHead]++;
}

void BrainFuck::decrement(){
        buffer[pointerHead]--;
}

void BrainFuck::loop(){

}


class Interpreter{
        private:
                std::ifstream file{};
        public:
                BrainFuck bf{};
                Interpreter(const std::string& filename): file{filename}{
                        std::ifstream file{filename};
                        if(!file.is_open()){
                                throw std::runtime_error("File Error: Error opening file");
                        }
                }
        void eatChars(void);
        void loop
};

void Interpreter::eatChars(void){
        std::istreambuf_iterator<char> begin{file};
        std::istreambuf_iterator<char> end{};

        std::for_each(begin, end, [&](char c){
                switch (c)
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
                                bf.write(); /* Write to output*/
                                break;
                        case ',':
                                std::cout << (char)bf.read(); /* Read from input */
                                break;
                        default:
                                break;
                }
        });
}

int main(int argc, char **argv){
        //
}