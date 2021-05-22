clear
cd ~
cd Desktop
cd Assignments
echo "S***howing test folder does not exist***"
ls
echo "***Creating test dir***"
mkdir test
echo "***shoing newly created test dir***"
ls
ecjp "***Changing to test dir***"
cd test
echo "***shoing no empty dir***"
ls
echo "***going back one folder***"
cd ..
echo "***copying test.txt to test dir***"
cp test.txt test
echo "***going back to test dir***"
cd test
echo "***shoing file was copied to test dir***"
ls
echo "***reading contents of test.txt***"
cat test.txt
echo "***pinging www.google.com 3 times***"
ping -c 3 www.google.com
