#!/bin/sh

if test -z "$PATH_INFO"
then cat <<!
Content-type: text/plain

PATH_INFO is empty
!
else exec xterm -e gdb --args ".$PATH_INFO"
fi
