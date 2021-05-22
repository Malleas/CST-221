#!/bin/sh
echo "My shell script test."
cd ~
cd Desktop
cd Assignments

echo $1
echo $2

gcc $1 -o $2

./$2
