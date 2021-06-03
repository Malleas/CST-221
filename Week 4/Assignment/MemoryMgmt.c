//
// Created by matt on 6/3/21.
//

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

char binaryArray[16] = {
        '0','0','0','0',
        '0','0','0','0',
        '0','0','0','0',
        '0','0','0','0',
};
//copied my print from Assignment 4 of to a function to be used int he converter.
void printBinary(){
    for (int i = 15; i >= 0; --i) {
        printf("%c", binaryArray[i]);
    }
    printf("\n");
}
//decimal to binary converter borrowed from Assignment 4
void convertToBinary(int input, char* binaryArray){
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
    printBinary();
}




int main(int argc, char const *argv[]) {
    //variable
    bool done = false;
    int input;
    //create a while loop that prompts a user for an input of either 4096 or 8191
    while (!done){
        printf("Enter either 4096 or 8191.\n");
        scanf("%d", &input);
        if(input == 4096 || input == 8191){
            done = true;
        }
    }
    printf("you entered: %d\n", input);
    printf("The binary conversion for %d is: ", input);
    convertToBinary(input, binaryArray);
    printf("The hex for page size: %d is %x\n", input, input);

    int startingVirtualAddress = 0x00000000;
    int startingPhysicalAddress = 0x0007FFFF;
    int pageTable[input][2];
    int j = 0;
    for (int i = 0;i < input; ++i) {
        pageTable[i][j] = startingVirtualAddress++;
        j++;
        pageTable[i][j] = startingPhysicalAddress++;
        j--;
    }
    //print the array for testing to make sure it's populating.
    printf("%10s%10s\n", "Virtual", "Physical");
    for (int i = 0; i < input; ++i) {
            printf("%10x%10x\n", pageTable[i][j], pageTable[i][j+1]);
    }

    //prompt user for a hex input between 0x00000000 and 0x000FFFFF
    int lookupNum;
    printf("Enter a number between 0x00000000 and 0x000FFFFF\n");
    scanf("%x", &lookupNum);
    int found = 0;
    for (int i = 0; i < input; ++i) {
        if(pageTable[i][0] == lookupNum){
            printf("The Physical Memory location for %x is: %x\n", lookupNum, pageTable[i][1]);
            found++;
        }
    }
    if(found == 0){
        printf("The location requested <%x> is on disk at location: %x\n", lookupNum, (0x0007ffff+4097));
    }





}
