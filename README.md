A setup to run CGI scripts.

<pre>
$ python3 -m http.server --cgi
</pre>

open in a browser http://localhost:8000/cgi-bin/awk.cgi or
http://localhost:8000/cgi-bin/awk.cgi

<pre>
$ (cd cgi-bin && make)
</pre>

Open http://localhost:8000/cgi-bin/c.cgi
