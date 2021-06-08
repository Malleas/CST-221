#!/bin/bash
# Found a simple solution for this here:
# Gotimer, Gene (28, August 2009) looping through `ls` results in bash shell script.  Retrieved from:
# https://superuser.com/questions/31464/looping-through-ls-results-in-bash-shell-script
for f in *; do
    echo "File -> $f"
done