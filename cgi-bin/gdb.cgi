#!/bin/sh

exec xterm -e gdb -args cgi-bin/btc.cgi "$@"
