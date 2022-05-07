#!/bin/bash
DIR="build"

cmake --build "$DIR/" --config Debug 2> log/build.log