A setup to run CGI scripts.

<pre>
$ python3 -m http.server --cgi
</pre>

open in a browser <http://localhost:8000/cgi-bin/awk.cgi>

<pre>
$ (cd cgi-bin && make)
</pre>

Open <http://localhost:8000/cgi-bin/vars.cgi>

<pre>
curl -d @README.md -X POST -s 'http://localhost:8000/cgi-bin/vars.cgi'
curl -d @README.md -H 'Content-Type: text/markdown' -X POST -s 'http://localhost:8000/cgi-bin/vars.cgi'
</pre>
