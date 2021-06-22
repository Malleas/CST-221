#!/bin/bash

for i in $(find / -type f -links +2); do
    ls -l $i
done