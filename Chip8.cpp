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
    delayTimer = 0x0;
    soundTimer = 0x0;

    display.fill(false);
    keystates.fill(false);
    displayChanged = false;

    font = {0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
            0x20, 0x60, 0x20, 0x20, 0x70, // 1
            0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
            0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
            0x90, 0x90, 0xF0, 0x10, 0x10, // 4
            0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
            0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
            0xF0, 0x10, 0x20, 0x40, 0x40, // 7
            0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
            0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
            0xF0, 0x90, 0xF0, 0x90, 0x90, // A
            0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
            0xF0, 0x80, 0x80, 0x80, 0xF0, // C
            0xE0, 0x90, 0x90, 0x90, 0xE0, // D
            0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
            0xF0, 0x80, 0xF0, 0x80, 0x80};// F

    for (int i = 0; i < font.size(); i++)
    {
        memory[0x050 + i] = font[i];
    }
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

void Chip8::decreaseTimers()
{
    delayTimer--;
    soundTimer--;
    return;
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
        if (x == 0x0 && y == 0xe && n == 0xe)
        {
            PC = stack.top();
            stack.pop();
            return;
        }
    }

    if (type == 0x1)
    {
        PC = nnn;
        return;
    }

    if (type == 0x2)
    {
        stack.push(PC);
        PC = nnn;
        return;
    }

    if (type == 0x3)
    {
        if (registers[x] == nn)
        {
            PC += 2;
        }
        return;
    }

    if (type == 0x4)
    {
        if (registers[x] != nn)
        {
            PC += 2;
        }
        return;
    }

    if (type == 0x5)
    {
        if (registers[x] == registers[y])
        {
            PC += 2;
        }
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

    if (type == 0x8)
    {
        if (n == 0x0)
        {
            registers[x] = registers[y];
            return;
        }
        
        if (n == 0x1)
        {
            registers[x] = registers[x] | registers[y];
            return;
        }

        if (n == 0x2)
        {
            registers[x] = registers[x] & registers[y];
            return;
        }

        if (n == 0x3)
        {
            registers[x] = registers[x] ^ registers[y];
            return;
        }

        if (n == 0x4)
        {
            registers[15] = 0;
            int sum = registers[x] + registers[y];
            if (sum > 255)
            {
                sum = sum % 256;
                registers[15] = 1;
            }
            registers[x] = sum;
            return;
        }

        if (n == 0x5)
        {
            if (registers[x] > registers[y])
            {
                registers[15] = 1;
            }
            else
            {
                registers[15] = 0;
            }
            registers[x] = registers[x] - registers[y];
            return;
        }

        if (n == 0x6)
        {
            //registers[x] = registers[x] + registers[y];
            registers[15] = registers[x] & 0b00000001;
            registers[x] = registers[x] >> 1;
            return;
        }

        if (n == 0x7)
        {
            if (registers[y] > registers[x])
            {
                registers[15] = 1;
            }
            else
            {
                registers[15] = 0;
            }
            registers[x] = registers[y] - registers[x];
            return;
        }

        if (n == 0xe)
        {
            //registers[x] = registers[x] + registers[y];
            registers[15] = registers[x] & 0b10000000;
            registers[x] = registers[x] << 1;
            return;
        }
    }

    if (type == 0x9)
    {
        if (registers[x] != registers[y])
        {
            PC += 2;
        }
        return;
    }

    if (type == 0xa)
    {
        I = nnn;
        return;
    }

    if (type == 0xb)
    {
        PC = nnn + registers[0];
        return;
    }

    if (type == 0xc)
    {
        registers[x] = (rand() % 256) & nn;
        return;
    }   

    if (type == 0xd)
    {
        displayChanged = true;

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

    if (type == 0xe)
    {
        if (nn == 0x9e)
        {
            if (keystates[registers[x]])
            {
                PC += 2;
            }
            return;
        }
        
        if (nn == 0xa1)
        {
            if (!keystates[registers[x]])
            {
                PC += 2;
            }
            return;
        }
    }

    if (type == 0xf)
    {
        if (nn == 0x07)
        {
            registers[x] = delayTimer;
            return;
        }

        if (nn == 0x15)
        {
            delayTimer = registers[x];
            return;
        }

        if (nn == 0x18)
        {
            soundTimer = registers[x];
            return;
        }

        if (nn == 0x1e)
        {
            I += registers[x];
            if (I > 0x1000)
            {
                registers[15] = 1;
            }
            return;
        }

        if (nn == 0x0a)
        {
            for (int i = 0; i < keystates.size(); i++)
            {
                if (keystates[i] == true)
                {
                    registers[x] = i;
                    return;
                }
            }
            PC -= 2;
            return;
        }

        if (nn == 0x29)
        {
            I = 0x050 + (0b00001111 & registers[x])*5;
            return;
        }

        if (nn == 0x33)
        {
            int i = registers[x];
            memory[I] = i/100;
            memory[I+1] = (i/10) % 10;
            memory[I+2] = (i % 100) % 10;

            return;
        }

        if (nn == 0x55)
        {
            for (int i = 0; i <= x; i++)
            {
                memory[I+i] = registers[i];
            }
            return;
        }

        if (nn == 0x65)
        {
            for (int i = 0; i <= x; i++)
            {
                registers[i] = memory[I+i];
            }
            return;
        }
    }


    std::cout << "unknown instruction: " << std::hex << std::uppercase << instruction << std::endl;
    return;
}