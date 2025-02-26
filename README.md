A setup to run CGI scripts.

<pre>
$ python3 -m http.server --cgi &
</pre>

open in a browser <http://localhost:8000/cgi-bin/awk.cgi>

<pre>
$ (cd cgi-bin && make)
+ curl-config --cc
+ curl-config --cflags --libs
+ gcc btc.c -O2 -g -lcurl -o btc.cgi
c99 cat.c -O2 -g -o cat.cgi
c99 c.c -O2 -g -o c.cgi
c99 error.c -O2 -g -o error.cgi
c99  -O2 -g -o man.cgi pydoc.c
c99 pydoc.c -O2 -g -o pydoc.cgi
c99 vars.c -O2 -g -o vars.cgi
</pre>

or without `libcurl`
<pre>
(cd cgi-bin && make CURLFLAGS= CURLCC=gcc cat.cgi c.cgi error.cgi man.cgi pydoc.cgi)
c99 cat.c -O2 -g -o cat.cgi
c99 c.c -O2 -g -o c.cgi
c99 error.c -O2 -g -o error.cgi
c99  -O2 -g -o man.cgi pydoc.c
c99 pydoc.c -O2 -g -o pydoc.cgi
</pre>

open <http://localhost:8000/cgi-bin/vars.cgi>

<pre>
$ curl --data-binary @README.md -X POST -s http://localhost:8000/cgi-bin/vars.cgi
</pre>

<pre>
$ curl --data-binary @README.md -H 'Content-Type: text/markdown' -X POST -s http://localhost:8000/cgi-bin/vars.cgi
</pre>
