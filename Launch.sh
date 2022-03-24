#!/bin/sh
DIR="build"
if [ ! -z "$1" ]
then
	if [ "$1" = "-m" ]
	then
		make -C "$DIR" -j
		if [ ! $? -eq 0 ]
		then
			exit 1
		fi
	fi
fi
clear
./"$DIR"/launch-moteur.sh --debugger
