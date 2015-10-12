#!/usr/bin/bash

project=`zenity --file-selection --directory --title="Location of Project to maintain" --width 600 --height 100  2>>/tmp/sessionError` 
bash -c "${project}/maintenance.sh ${project} >>/tmp/sessionLog 2>>/tmp/sessionError"
