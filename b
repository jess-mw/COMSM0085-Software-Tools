#!/bin/sh

#similar to Werror in C
set -euo pipefail
#first argument on compile line stored in instruction
instruction="$1"
#second argument stored in filename
filename="$2"
executable="$(basename "$filename" .c)"

if [ "$executable" = "$filename" ]
then
   filename="$filename".c
fi

#if the file exists
if [ -e "$filename" ]
then

   if [ "$instruction" = "compile" ]
   then
      gcc -Wall "$filename" -o "$executable" && echo "compiled"

   elif [ "$instruction" = "build" ]
   then
      gcc -Wall "$filename" -o "$executable" && ./"$executable" && echo "built"

   elif [ "$instruction" = "run" ]
   then
      ./"$executable"

   else
      echo "${instruction} isn't a valid instruction, try compile, run, or build"
   fi

else
   echo "${filename} does not exist"

fi
