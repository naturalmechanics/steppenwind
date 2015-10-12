#!/usr/bin/fish
project=`zenity --forms --title="Details" --text="Project Name and Intention" --separator="," --add-entry="Simulation Name" --add-entry="Simulation-relevant Job" --add-entry="Path" --width 600 --height 100`

accepted=$?
if (($accepted != 0)); then
    exit 1
end

name=$(awk -F, '{print $1}' <<<$project)
job=$(awk -F, '{print $2}' <<<$project)
path=$(awk -F, '{print $3}' <<<$project)


cd $path

# now starting the suite

case $job in
		"build")
		
		## editor
		kate -s $name &
 
		## compiler , this will need interaction
		konsole -e fish &

		## execution shell, this will have to handle output in real time :
		terminology -e fish &
		
		## file manager 
		spacefm &
		;;

		"run") # anything that happens here can be sent to other machines

		## execution shell, this will have to handle output in real time :
		terminology -e fish &
		;;
	#case "codetransfer" # local and global backups
		## transfer
		#konsole -e fish & #for now git


	#case "binarytransfer"

esac





## viewer
# some day a QCanvas viewer

## validator
#: 

## linker
#:

