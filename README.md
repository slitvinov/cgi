A setup to run CGI scripts.

<pre>
$ python3 -m http.server --cgi &
</pre>

open in a browser <http://localhost:8000/cgi-bin/awk.cgi>

<pre>
$ (cd cgi-bin && make)
c99 -O2 -g cat.c -o cat.cgi
c99 -O2 -g c.c -o c.cgi
c99 -O2 -g error.c -o error.cgi
c99 -O2 -g vars.c -o vars.cgi
</pre>

open <http://localhost:8000/cgi-bin/vars.cgi>

<pre>
$ curl --data-binary @README.md -X POST -s http://localhost:8000/cgi-bin/vars.cgi
</pre>

<pre>
$ curl --data-binary @README.md -H 'Content-Type: text/markdown' -X POST -s http://localhost:8000/cgi-bin/vars.cgi
</pre>
