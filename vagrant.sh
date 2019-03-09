#!/bin/bash

function usage {
	echo "Usage $0 command <params>"
	echo "Where command:"
	echo "	-c to compile"
	echo "	-r <params> to run"
	echo "	-t to run tests"
	exit 1
}

if [ $# -lt 1 ]
then
	usage
fi

if [ "$1" = "-c" ]
then
	vagrant ssh -c 'cd /avionics && make x86'
elif [ "$1" = "-r" ]
then
	shift 1
	vagrant ssh -c "cd /avionics && sudo ./avionics-x86.exe -c config.json $@"
elif [ "$1" = "-t" ]
then
	vagrant ssh -c "cd /avionics && make tests && sudo ./avionics-tests.exe"
else
	usage
fi