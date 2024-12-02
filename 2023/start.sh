#!/bin/bash

if [[ $# -eq 0 ]] ; then
    echo "Give number as a parameter"
    exit 1
fi

name="day-$1"

if [[ -e $name ]]; then
    echo "Directory already exist"
    exit 1
fi

cp -r template $name
echo "directory $name created"
