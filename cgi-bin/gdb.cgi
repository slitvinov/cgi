#!/bin/sh

cat <<!
Content-type: text/html

<HTML>
<HEAD><TITLE>shell</TITLE></HEAD>
<BODY>
<PRE>
`echo begin arg
for i
do printf '%s\n' "$i"
done
echo end arg
env`
</PRE>
</BODY>
</HTML>
!
