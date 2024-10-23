#include "Chip8.h"

#include <fstream>
#include <iostream>
#include <iomanip>


Chip8::Chip8()
{
    memory.fill(0);
    registers.fill(0);
    PC = 0x200;
    I = 0x0;

    display.fill(false);
    keystates.fill(false);
}

Chip8::~Chip8()
{

}

bool Chip8::loadRom(std::string filename)
{
    std::ifstream romFile (filename, std::ifstream::binary);

    char byte;
    while (romFile.read(&byte, sizeof(byte)))
    {
        memory[PC] = byte;
        PC += 1;
    }

    PC = 0x200;

    return true;
}

void Chip8::printByte(uint16_t address)
{
    std::cout << std::hex << std::setfill('0') 
              << std::setw(2) << std::uppercase 
              << (static_cast<unsigned int>(memory[address]) & 0xFF) << " ";

    return;
}

void Chip8::setKeystates(std::array<bool, 16> new_keystates)
{
    keystates = new_keystates;
    return;
}

std::array<bool, 64*32> Chip8::getDisplay()
{
    return display;
}

void Chip8::fetchDecodeExecute()
{
    // fetch
    uint16_t instruction = memory[PC];
    instruction = instruction << 8;
    instruction = instruction | memory[PC+1];
    PC += 2;

    //std::cout << std::hex << instruction << std::endl;

    // decode
    uint16_t type = instruction & 0b1111000000000000;
    type = type >> 12;
    uint16_t x    = instruction & 0b0000111100000000; // vx register lookup
    x = x >> 8;
    uint16_t y    = instruction & 0b0000000011110000; // vy register lookup
    y = y >> 4;
    uint16_t n    = instruction & 0b0000000000001111; // 4bit number
    uint16_t nn   = instruction & 0b0000000011111111; // 8bit immediate number
    uint16_t nnn  = instruction & 0b0000111111111111; // 12bit immediate address

    // execute
    if (type == 0x0)
    {
        if (x == 0x0 && y == 0xe && n == 0x0) // 00E0
        {
            display.fill(false);
            return;
        }
    }

    if (type == 0x1)
    {
        PC = nnn;
        return;
    }

    if (type == 0x6)
    {
        registers[x] = nn;
        return;
    }

    if (type == 0x7)
    {
        registers[x] += nn;
        return;
    }

    if (type == 0xa)
    {
        I = nnn;
        return;
    }

    if (type == 0xd)
    {
        int x_coord = registers[x] % 64;
        int y_coord = registers[y] % 32;
        registers[15] = 0;

        for (int i = 0; i < n && y_coord + i < 32; i++)
        {
            uint8_t row = memory[I + i];
            uint8_t mask = 0b10000000;

            for (int j = 0; j < 8 && x_coord + j < 64; j++)
            {
                if ((row & mask) != 0)
                {
                    if(display[(y_coord + i) * 64 + x_coord + j])
                    {
                        registers[15] = 1;
                    }
                    display[(y_coord + i) * 64 + x_coord + j] = !display[(y_coord + i) * 64 + x_coord + j];
                }
                mask = mask >> 1;
            }
        }
        return;
    }



    std::cout << "unknown instruction: " << std::hex << std::uppercase << instruction << std::endl;
    return;
}