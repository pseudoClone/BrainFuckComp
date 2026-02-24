#include "BrainFuck.hpp"

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