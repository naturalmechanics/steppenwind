#!/usr/bin/bash

#
# The main script to control the steppenwind shell
# from the Operating System
# 1. a plain call without any argument starts the shell
# 2. a call with --build switch starts editor and related build toolchain to build the shell itself
# 3. a call with --backup requires a further argument
# 3a. local : local backup to a hdd
# 3b. internet : github update.
# 4. a call with --sync switch syncs from the git hub
# 5. a call with --make switch creates the binaries... 

if [ $# -eq 0 ] 
then
	konsole -e ~/Commands/shell_start.sh &
else
	case $1 in
		--with)
		case $2 in
			default)
				konsole -e ~/Commands/shell_start.sh --default&
			;;
			*)
				konsole -e ~/Commands/shell_start.sh $2&
			;;
		esac
		;;
		--build)
			# kate
			kate -s /projects/Steppenwind/current/steppenwind &
			
			# 
			stuurman /projects/Steppenwind/ &
			
			# konsole to manipulate things
			konsole --workdir /projects/Steppenwind &

			;;
		--backup)
			# start backup tools
			
			;;
		--sync)
			;;
		--make)
			;;
	esac
fi


