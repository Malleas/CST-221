#!/bin/bash

#my pattern works and tested in a regex tester, i'm unsure why or what is causing it to fail when evaluating the input.

echo  "Enter your password"
read "var"
echo "The test string is:"
echo "$var"
echo ""
pattern="^(?=.*[!@#$&*])(?=.*[0-9]).{8}$"
echo "The regex is:"
echo "$pattern"

if [[ $var =~ $pattern ]]; then
    echo "Success"
else
    echo "fail"
fi

