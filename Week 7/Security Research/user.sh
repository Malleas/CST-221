#!/bin/bash

input=$1
groupName=$2
oppFlag=$3

echo $input
echo $groupName
echo $oppFlag

getent group $groupName || sudo groupadd $groupName

export IFS=" "
while read u; do
    echo "$u"
    users=$u
done <users.txt
userArray=()
echo "more users"
echo  "$users"
for user in $users; do
  echo $user
  userArray+=($user)
  done
echo "foo"
count=0
ucount=0
  for (( i = 0; i < 5; i++)); do
      password=${userArray[$count+1]}
      count=`expr $count + 2`
      echo $password
      userName=${userArray[$ucount]}
      ucount=`expr $ucount + 2`
      echo $userName
      sudo useradd -m -p "$password" "$userName"
      sudo usermod -a -G "$groupName" "$userName"
        done
      grep -oE '[1-5]:x:100[1-5]:100[1-5]::/home/[1-5]:/bin/sh' /etc/passwd
      grep "$groupName" /etc/group
removeCounter=0
    for (( i = 0; i < 5; i++ )); do
      userName=${userArray[$removeCounter]}
      removeCounter=`expr $removeCounter + 2`
      echo $userName
      sudo userdel -r "$userName"

    done
     grep -oE '[1-5]:x:100[1-5]:100[1-5]::/home/[1-5]:/bin/sh' /etc/passwd
      grep "$groupName" /etc/group