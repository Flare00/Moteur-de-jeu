#!/bin/bash
DIR="build"

if [ ! -d "./$DIR" ]
then
	echo "Generating $DIR"
	mkdir "$DIR"

else
	echo "Emptying $DIR"
	rm -r "$DIR/"
	mkdir "$DIR"
fi


if [ ! -d "./log" ]
then
	mkdir "log"
fi

cmake . -B"$DIR" 2> log/cmake.log