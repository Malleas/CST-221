//
// Created by matt on 6/1/21.
//

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

char binaryArray[32] = {
        '0', '0', '0', '0',
        '0', '0', '0', '0',
        '0', '0', '0', '0',
        '0', '0', '0', '0',
        '0', '0', '0', '0',
        '0', '0', '0', '0',
        '0', '0', '0', '0',
        '0', '0', '0', '0'
};

void resetArray() {
    for (int i = 0; i < 15; ++i) {
        binaryArray[i] = '0';
    }
}

void convertToBinary(int input, char* binaryArray){
    resetArray();
    int temp = input;
    int index = 0;
    while (temp != 0) {
        int remainder = temp % 2;
        if (remainder == 0) {
            binaryArray[index] = '0';
        } else {
            binaryArray[index] = '1';
        }

        temp = temp / 2;
        index++;
    }
}

int main(int argc, char const *argv[]) {
    //variables
    int input;


    //ask for a number between 0 and 1000
    printf("Enter a number between 0 and 1000!\n");
    //capture number using scanf()
    scanf("%d", &input);
    printf("You entered %d\n", input);
    //convert number from input to binary, print results
    printf("Binary for input is: \n");
    convertToBinary(input, binaryArray);
    for (int i = 31; i >= 0; --i) {
        printf("%c", binaryArray[i]);
    }
    printf("\n");

    //convert number to hex, print results
    printf(" Input of %d in Hex is: %X\n", input, input);
    //shift number left 10, masking the lower bits to 0
    //OR the number 0x3FF, print the results in binary, hex and decimal
    int temp2 = (input << 10) & 0xFFFFFC00 | 0x3FF;
    printf("Decimal for temp2 is: %d\n", temp2);
    printf("Hex for temp2 is: %x\n", temp2);
    convertToBinary(temp2, binaryArray);
    printf("Binary for temp2 is: \n");
    for (int i = 31; i >= 0; --i) {
        printf("%c", binaryArray[i]);
    }
    printf("\n");

}