// Written for the purposes of demoing Team 23's encoder.
// Takes a 32-bit key as a argument
// Reads a buffer in from stdin
// Reverses the additive XOR encoding implemented by Metasploit
// Prints result to stdout

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

unsigned int key = 0;

void printBlock(unsigned int block);
unsigned char toByte(char a, char b);
char * toHex(unsigned char byte);

int main(int argc, char ** argv) {

    // Check arguments
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

    // Get key
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

    // Read in bytes, decode, and print to stdout
    while (err != 0) { // While the next byte exists

        unsigned int block = 0; // 32-bit encrypted block

        // Four bytes for 32 bit key size
        for (int i = 0; i < 4; i++) {

            // Check if current byte exists
            if (err == 0) {
                if (i == 0) break;
                printf("Error: input is not 32-bit aligned\n");
                exit(1);
            }

            // Add byte to block
            // Metasploit prints the payload with the least
            // siginificant byte first
            // Shift the byte just read over by the right amount
            unsigned char byte = toByte(a, b);
            block += byte << (8 * i);

            // Go get the next byte
            err = scanf("\\x%c%c", &a, &b);
        }
        // Now XOR the key with the block
        // And generate the new key
        // Note that the fact that unsigned ints are
        // naturally 32 bits long prevents us from
        // having to cut off any extra bits
        unsigned int original = block ^ key;
 
        // Debugging       
        //printf("Key: %u\n", key);
        //printf("Encrypted: %u\n", block);
        //printf("Decrypted: %u\n", original);
        
        key = original + key;
        printBlock(original);
    }
    printf("\n");

}

// Takes two lowercase hexadecimal digits
// And converts them into a byte
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

// Prints an unsigned int as a block of four bytes
// Least to most sigificant
void printBlock(unsigned int block) {

    // Split into four bytes
    unsigned char bytes[4];
    for (int i = 0; i < 4; i++) {
        bytes[3 - i] = block % 256;
        block = block >> 8;
    }

    // Print bytes from LEAST to MOST significant
    for (int i = 3; i >= 0; i--) {
        char * h = toHex(bytes[i]);
        printf("\\x%s", toHex(bytes[i]));
        free(h);
    }
}

// Returns pointer to two-char string containing
// the hex representation of a single byte
char * toHex(unsigned char byte) {
    int a = byte / 16;
    int b = byte % 16;
    char first;
    char second;
    if (a >= 0 && a <= 9) {
        first = '0' + a;
    } else {
        a -= 10;
        first = 'a' + a;
    }
    if (b >= 0 && b <= 9) {
        second = '0' + b;
    } else {
        b -= 10;
        second = 'a' + b;
    }
    char * result = (char *)malloc(3 * sizeof(char));
    result[0] = first;
    result[1] = second;
    result[2] = '\0';
    return result; 
}
