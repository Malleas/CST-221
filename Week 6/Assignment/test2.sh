#!/bin/bash
#AskUbunto (16, December 2020) cp recursive with specific file extension.  Retrieved from:
#https://askubuntu.com/questions/1300558/cp-recursive-with-specific-file-extension
cd ~
mkdir mycode
cd ~/CLionProjects/CST-221/
rsync -r -f '+ *.c' -f '+ **/' -f '- *' --prune-empty-dirs ~/CLionProjects/CST-221/ ~/mycode/
cd ~
mkdir mycode2
cp -r ~/mycode  ~/mycode2
mv ~/mycode ~/deadcode
rm -rf ~/deadcode