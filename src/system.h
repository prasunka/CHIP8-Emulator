#include <string>

class CHIP8
{
public:

    /**
     * Empty default constructor
     */
    CHIP8();
    /**
     * Empty default destructor
     */
    ~CHIP8();

    /**
     * Initialise the system
     * Clear the memory and initialise CPU variables.
     */
    void initialise();

    /**
     * Load the program into memory.
     *
     * @param filename The path to the ROM to load
     */
    void loadROM(const std::string& filename);
    /**
     * Emulate one cycle of the CPU.
     */
    void emulateCycle();

    /**
     * Graphics screen with 64 * 32 pixels.
     */
    unsigned char graphics[64 * 32];

private:
    unsigned short opcode;
    unsigned short I;
    unsigned short pc;
    unsigned short stack[16];
    unsigned short sp;

    unsigned char R[16];
    unsigned char key[16];
    unsigned char memory[4096];
    unsigned char delayTimer;
    unsigned char soundTimer;
};