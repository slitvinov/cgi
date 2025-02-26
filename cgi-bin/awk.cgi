#!/usr/bin/awk -f

BEGIN {
    print "\
Content-type: text/html\n\
\n\
<!DOCTYPE html>\n\
<HTML LANG=\"en\">\n\
  <HEAD><TITLE>awk CGI</TITLE></HEAD>\n\
  <BODY>\n\
    <PRE>\
"
    print("ARGC: " ARGC)
    for (i = 0; i < ARGC; i++)
	print i " [" ARGV[i] "]"
    print "\
    </PRE>\n\
  </BODY>\n\
</HTML>\
"
}
