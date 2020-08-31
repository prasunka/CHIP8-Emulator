#include <iostream>
#include <string>

#include "system.h"

int main(int argc, char** argv){

    CHIP8 emulator;

    emulator.initialise();

    while(true){
        emulator.emulateCycle();
        std::cout << "Running cycle!" << std::endl;
    }

    return 0;
}
