#!/usr/bin/bash


project=`zenity --forms --title="Details" --text="Project Name and Intention" --separator="," --add-entry="Model Name" --add-entry="Model-relevant Job" --add-entry="Path" --width 600 --height 100`

accepted=$?
if (($accepted != 0)); then
    exit 1
fi

name=$(awk -F, '{print $1}' <<<$project)
job=$(awk -F, '{print $2}' <<<$project)
path=$(awk -F, '{print $3}' <<<$project)

cd $path

pwd
echo job

case $job in
	"build")
		
		## editor
		~/Software/Bachata/bachata.gambas "/projects/Bachata/current/.gambas/configFile" &
 
		## parser
		~/Software/Auana/auana &

		## shell
		terminology -e fish &
		
		## file manager 
		spacefm &
		
		## kate anyway
		kate /projects/Bachata/current/.gambas/bachata_keywords &
		;;
		
	"run")
	
		terminology -e fish &
		;;
esac