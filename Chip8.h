#ifndef CHIP8_H
#define CHIP8_H

#include <stack>
#include <queue>
#include <array>
#include <string>

class Chip8 
{
public:
    Chip8();
    ~Chip8();

    bool loadRom(std::string filename);
    void printByte(uint16_t address);
    void setKeystates(std::array<bool, 16> new_keystates);
    void fetchDecodeExecute();
    std::array<bool, 64*32> getDisplay();

private:
    std::array<uint8_t, 4096> memory;
    std::array<uint8_t, 16> registers;
    uint16_t PC;
    uint16_t I;
    std::stack<uint16_t> stack;
    std::array<bool, 64*32> display;
    std::array<bool, 16> keystates; // true for pressed

    // todo: add timers
};

#endif // CHIP8_H