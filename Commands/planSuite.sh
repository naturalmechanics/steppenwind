#!/usr/bin/bash

program=`zenity --forms --title="Planning" --text="Using software : " --separator="," --add-entry="Enter Command name" --width 120 --height 20`
bash -c "${program}"
