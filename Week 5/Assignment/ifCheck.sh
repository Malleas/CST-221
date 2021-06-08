#!/bin/bash
if [ $1 -gt 50 ]; then
   echo "$1 is greater than 50"
elif [ $1 -lt 50 ]; then
    echo "$1 is less than 50"
else
   echo "$1 is equal to 50"
fi