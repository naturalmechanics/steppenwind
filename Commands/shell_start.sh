#!/usr/bin/bash

if [ $# -eq 0 ] 
then
	/projects/Steppenwind/current/steppenwind --no-messagebus-manager
else
	case $1 in
		--default)
			/projects/Steppenwind/current/steppenwind --configFilePath /projects/Steppenwind/current/default --no-messagebus-manager
			;;
		*)
			/projects/Steppenwind/current/steppenwind --configFilePath $1 --no-messagebus-manager
			;;
	esac
fi





