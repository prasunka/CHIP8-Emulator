#include <iostream>
#include <string>

#include "system.h"

int main(int argc, char** argv){

    if(argc < 2){
        std::cerr << "No ROM specified!\nUsage:\t" << argv[0] << " <ROM path>" << std::endl;
    }
    CHIP8 emulator;

    emulator.initialise();

    if(!emulator.loadROM(argv[1])){
        exit(1);
    }

    while(true){
        emulator.emulateCycle();
        std::cout << "Running cycle!" << std::endl;
        break;
    }

    return 0;
}
