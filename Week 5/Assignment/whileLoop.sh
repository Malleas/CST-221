#!/bin/bash
count=0
#tutorialspoint (n.d.) Unix/Linux Shell - The While Loop.  Retrieved from https://www.tutorialspoint.com/unix/while-loop.htm

while [ $count -lt 11 ]
do
   echo $count
   count=`expr $count + 1`
done