
class CHIP8
{
public:
    CHIP8();
    ~CHIP8();

    void initialise();
    void emulateCycle();

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