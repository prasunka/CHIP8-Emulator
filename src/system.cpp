#include <iostream>
#include <fstream>

#include "system.h"

unsigned char CHIP8Fontset[80] =
{
    0xF0, 0x90, 0x90, 0x90, 0xF0, //0
    0x20, 0x60, 0x20, 0x20, 0x70, //1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
    0x90, 0x90, 0xF0, 0x10, 0x10, //4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
    0xF0, 0x10, 0x20, 0x40, 0x40, //7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
    0xF0, 0x90, 0xF0, 0x90, 0x90, //A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
    0xF0, 0x80, 0x80, 0x80, 0xF0, //C
    0xE0, 0x90, 0x90, 0x90, 0xE0, //D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
    0xF0, 0x80, 0xF0, 0x80, 0x80  //F
};

CHIP8::CHIP8()
{
}

CHIP8::~CHIP8()
{
}

void CHIP8::initialise(){

    opcode     = 0;
    I          = 0;
    pc         = 0x200;
    sp         = 0;
    delayTimer = 0;
    soundTimer = 0;

    for (int i = 0; i < 16; ++i){
        stack[i] = 0;
        R[i]     = 0;
        key[i]   = 0;
    }

    for (int i = 0; i < 64 * 32; ++i){
        graphics[i] = 0;
    }

    for (int i = 0; i < 4096; ++i){
        memory[i] = 0;
    }

    for (int i = 0; i < 80; ++i){
        memory[i] = CHIP8Fontset[i];
    }

}

bool CHIP8::loadROM(const std::string &filename){
    std::ifstream in;
    std::cerr << "Loading program: " << filename << std::endl;
    in.open(filename, std::ios::in | std::ios::binary);

    if(!in.is_open()){
        std::cerr << "Program couldn't be opened!" << std::endl;
        return false;
    }

    // get length of file:
    in.seekg (0, in.end);
    int length = in.tellg();
    in.seekg (0, in.beg);

    std::cerr << "\nProgram loaded: " << filename
              << "\nSize          : " << length << std::endl;

    char *buffer = new char[length];

    in.read(buffer, length);

    if (length < 4096 - 512){
        for (int i = 0; i < length; ++i){
            memory[512 + i] = buffer[i];
        }
    }
    else{
        std::cerr << "Program too big to fit in memory!" << std::endl;
        in.close();
        return false;
    }

    return true;
}

void CHIP8::emulateCycle(){

}