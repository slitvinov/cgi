#!/usr/bin/awk -f

BEGIN {
    print "\
Content-type: text/html\n\
\n\
<HTML>\n\
<HEAD><TITLE>awk</TITLE></HEAD>\n\
<BODY>\n\
<PRE>\
"
    for (i in ARGV)
	print ARGV[i]
    print "\
</PRE>\n\
</BODY>\n\
</HTML>\
"
}
