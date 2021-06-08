#!/bin/bash
read -p "Enter a name to search for: " VAR1
count=0
for VAR2 in $(sort<names.txt);do
  echo "Name -> $VAR2"
  if [[ "$VAR2" == "$VAR1" ]]; then
    count=`expr $count + 1`
  fi

done
  if [ $count -gt 0 ]; then
      echo "Name $VAR1 found in the list of names.txt"
  else
      echo "Name $VAR1 not found in names.txt"
  fi
