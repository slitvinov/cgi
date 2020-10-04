#!/bin/sh

cat <<!
Content-type: text/html

<HTML>
<HEAD><TITLE>shell</TITLE></HEAD>
<BODY>
<PRE>
`
echo begin arg
for i
do printf '%s\n' "$i"
done
echo end arg
awk '
 BEGIN {
     for (i = 0; i < 10; i++)
	 print i, i^2
}'
`
</PRE>
</BODY>
</HTML>
!
