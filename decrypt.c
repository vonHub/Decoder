#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

unsigned int key = 0;

void decodeBlock(unsigned int block);
unsigned char toByte(char a, char b);

int main(int argc, char ** argv) {

    if (argc != 2 && argc != 3) {
        printf("Usage: decrypt [key]\n");
        exit(1);
    }

    if (argc == 3 && strcmp(argv[1], "-b")) {
        printf("Usage: decrypt -b [binary key]\n");
        exit(1);
    }

    if (argc == 3 && strlen(argv[2]) != 32) {
        printf("Needs a 32 bit key\n");
        exit(1);
    }

    if (argc == 3) {
        // Get 32 bit key from input
        int index;
        for (index = 0; index < 32; index++) {
            if (argv[2][index] == '1') {
                key += pow(2, 31 - index);
            }
        }
    } else {
        key = (unsigned int)strtoul(argv[1], NULL, 0);
    }

    char a = '\0';
    char b = '\0';
    int err = scanf("\\x%c%c", &a, &b);

    while (err != 0) { // While the next byte exists

        // Four bytes for a 32 bit key
        unsigned int block = 0;
        for (int i = 0; i < 4; i++) {

            // Make room for next byte
            if (i > 0) block = block << 8;

            // Scan byte and check for error
            if (err == 0) {
                if (i == 0) break;
                printf("Error: input is not a proper size\n");
                exit(1);
            }

            // Add byte to block
            unsigned char byte = toByte(a, b);
            block += byte;

            // Go get the next byte
            err = scanf("\\x%c%c", &a, &b);
        }
        printf("%u\n", block);
    }

}

unsigned char toByte(char a, char b) {

    // Sanity check
    if (!(a >= '0' && a <= '9') && !(a >= 'a' && a <= 'f')) {
        printf("Please give buffer in lowercase hexadecimal\n");
        exit(1);
    }

    if (!(b >= '0' && b <= '9') && !(b >= 'a' && b <= 'f')) {
        printf("Please give buffer in lowercase hexadecimal\n");
        exit(1);
    }

    unsigned char result = 0;
    if (a >= 'a' && a <= 'f') {
        result += 10;
        a -= 'a';
    } else {
        a -= '0';
    }
    result += a;
    result = result << 4;
    if (b >= 'a' && b <= 'f') {
        result += 10;
        b -= 'a';
    } else {
        b -= '0';
    }
    result += b;
    return result;
}
