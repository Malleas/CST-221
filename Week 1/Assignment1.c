#include <stdio.h>

void getNum();
int doMath(int input1, int input2);

int main()
{
  getNum();
}

void getNum(){
  int input1;
  int input2;
  printf("Enter a number to do some math: \n");
  scanf("%d", &input1 );
  printf("Enter a second number to add it to the first number: \n");
  scanf("%d", &input2 );
  printf("The sume of %d and %d is %d\n", input1, input2, doMath(input1, input2));
}

int doMath(int input1, int input2){
  int sum = 0;
  sum = input1 + input2;
  return sum;
}
