#!/bin/sh
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

echo "CMake"
cmake . -B"$DIR"


#echo "Build"
#cmake --build "$DIR/" --config Debug

$SHELL 