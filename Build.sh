#!/bin/bash
DIR="build"

cmake --build "$DIR/" --config Release 2> log/build.log