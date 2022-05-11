#!/bin/bash
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


if [ "$OSTYPE" == "msys" ]; then
	./"$DIR"/launch-moteur.sh --debugger 2> log/exec.log
else 
	./"$DIR"/launch-moteur.sh --debugger 2> log/exec.log
fi