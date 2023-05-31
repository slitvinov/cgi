#!/bin/sh

cat <<!
Content-Type: text/plain

!
chmod u+x -- *.cgi 2>&1
ls -al -- *.cgi
