#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE * aFile;
unsigned char memory[256], registers[16], pc, a, b, c, d, cd;
const char * print(unsigned char pc, unsigned char a, unsigned char b, unsigned char cd, unsigned char * registers);

int main()
{
    aFile = fopen("a3.txt", "r");
    if (aFile == NULL)
    {
        printf("Error. Could not open file.");
        exit(1);
    }
    char theStr[10];
    int i = 0;
    unsigned int num = 0;

    while(fgets(theStr, 10, aFile) != NULL)
    {
        sscanf(theStr, "%X", &num);
        memory[i] = num >> 0x08;
        memory[i + 1] = num & 0x00ff;
        i += 2;
    }
    fclose(aFile);

    a = memory[pc] >> 0x04;
    b = memory[pc] & 0x0f;
    c = memory[pc + 1] >> 0x04;
    d = memory[pc + 1] & 0x0f;
    cd = memory[pc + 1];

    print(pc, a, b, cd, registers);

    while(1)
    {
        if (a == 0x01)
        {
            //  1RXY  Load register R with the value at memory address XY
            registers[b] = memory[cd];
            pc += 2;
            a = memory[pc] >> 0x04;
            b = memory[pc] & 0x0f;
            c = memory[pc + 1] >> 0x04;
            d = memory[pc + 1] & 0x0f;
            cd = memory[pc + 1];
            print(pc, a, b, cd, registers);
        }
        if (a == 0x02)
        {
            //  2RXY  Load register R with the value XY
            registers[b] = cd;
            pc += 2;
            a = memory[pc] >> 0x04;
            b = memory[pc] & 0x0f;
            c = memory[pc + 1] >> 0x04;
            d = memory[pc + 1] & 0x0f;
            cd = memory[pc + 1];
            print(pc, a, b, cd, registers);
        }
        if (a == 0x03)
        {
            //  3RXY  Store the value in register R at memory address XY
            memory[cd] = registers[b];
            pc += 2;
            a = memory[pc] >> 0x04;
            b = memory[pc] & 0x0f;
            c = memory[pc + 1] >> 0x04;
            d = memory[pc + 1] & 0x0f;
            cd = memory[pc + 1];
            print(pc, a, b, cd, registers);
        }
        if (a == 0x04)
        {
            //  40RS  Copy/move the value in register R to register S
            registers[b] = registers[d];
            pc += 2;
            a = memory[pc] >> 0x04;
            b = memory[pc] & 0x0f;
            c = memory[pc + 1] >> 0x04;
            d = memory[pc + 1] & 0x0f;
            cd = memory[pc + 1];
            print(pc, a, b, cd, registers);
        }
        if (a == 0x05)
        {
            //  5RST  Add the values in registers R and S and put the answer in register T
            registers[d] = registers[b] + registers[c];
            pc += 2;
            a = memory[pc] >> 0x04;
            b = memory[pc] & 0x0f;
            c = memory[pc + 1] >> 0x04;
            d = memory[pc + 1] & 0x0f;
            cd = memory[pc + 1];
            print(pc, a, b, cd, registers);
        }
        if (a == 0x07)
        {
            //  7RST  Bit-wise OR the values in registers R and S and put the answer in register T
            registers[d] = registers[b] | registers[c];
            pc += 2;
            a = memory[pc] >> 0x04;
            b = memory[pc] & 0x0f;
            c = memory[pc + 1] >> 0x04;
            d = memory[pc + 1] & 0x0f;
            cd = memory[pc + 1];
            print(pc, a, b, cd, registers);

        }
        if (a == 0x08)
        {
            //  8RST  Bit-wise AND the values in registers R and S and put the answer in register T
            registers[d] = registers[b] & registers[c];
            pc += 2;
            a = memory[pc] >> 0x04;
            b = memory[pc] & 0x0f;
            c = memory[pc + 1] >> 0x04;
            d = memory[pc + 1] & 0x0f;
            cd = memory[pc + 1];
            print(pc, a, b, cd, registers);
        }
        if (a == 0x09)
        {
            //  9RST  Bit-wise XOR the values in registers R and S and put the answer in register T
            registers[d] = registers[b] ^ registers[c];
            pc += 2;
            a = memory[pc] >> 0x04;
            b = memory[pc] & 0x0f;
            c = memory[pc + 1] >> 0x04;
            d = memory[pc + 1] & 0x0f;
            cd = memory[pc + 1];
            print(pc, a, b, cd, registers);
        }
        if (a == 0x0A)
        {
            //  AR0X  Rotate the contents of register R X times to the right
            for (i = 0; i < d; i++)
            {
                unsigned char temp = registers[b] & 0x01;
                registers[b] = registers[b] >> 1;
                registers[b] = registers[b] | (temp << 0x07);
            }
            pc += 2;
            a = memory[pc] >> 0x04;
            b = memory[pc] & 0x0f;
            c = memory[pc + 1] >> 0x04;
            d = memory[pc + 1] & 0x0f;
            cd = memory[pc + 1];
            print(pc, a, b, cd, registers);
        }
        if (a == 0x0B)
        {
            //  BRXY  Jump to the instruction located at memory address XY if the value in
            //  register R is equal to the value in register 0 (i.e. change the PC to XY)
            if (registers[b] == registers[0])
            {
                pc = cd;
                a = memory[pc] >> 0x04;
                b = memory[pc] & 0x0f;
                c = memory[pc + 1] >> 0x04;
                d = memory[pc + 1] & 0x0f;
                cd = memory[pc + 1];
                print(pc, a, b, cd, registers);
            }
				else
				{
					pc += 2;
					a = memory[pc] >> 0x04;
					b = memory[pc] & 0x0f;
               c = memory[pc + 1] >> 0x04;
               d = memory[pc + 1] & 0x0f;
               cd = memory[pc + 1];
					print(pc, a, b, cd, registers);
				}
        }
        if (a == 0x0C)
        {
            //  C000  Halt
            break;
        }
    }
    return 0;
}


const char * print(unsigned char pc, unsigned char a, unsigned char b, unsigned char cd, unsigned char * registers)
{
    int i;
    printf("%02X %X%X%02X - [%02X ", pc, a, b, cd, registers[0]);

    for (i = 1; i < 15; i++)
    {
        printf("%02X ", registers[i]);
    }
    printf("%02X]\n", registers[15]);
}





