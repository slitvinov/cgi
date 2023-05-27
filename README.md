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
$ curl -d @README.md -X POST -s 'http://localhost:8000/cgi-bin/vars.cgi'
AUTH_TYPE undefined
CONTENT_LENGTH='400'
CONTENT_TYPE='application/x-www-form-urlencoded'
GATEWAY_INTERFACE='CGI/1.1'
PATH_INFO=''
PATH_TRANSLATED='/home/lisergey/cgi'
QUERY_STRING=''
REMOTE_ADDR='127.0.0.1'
REMOTE_HOST=''
REMOTE_IDENT undefined
REMOTE_USER undefined
REQUEST_METHOD='POST'
SCRIPT_NAME='/cgi-bin/vars.cgi'
SERVER_NAME='penguin'
SERVER_PORT='8000'
SERVER_PROTOCOL='HTTP/1.0'
SERVER_SOFTWARE='SimpleHTTP/0.6 Python/3.9.2'
</pre>

<pre>
$ curl -d @README.md -H 'Content-Type: text/markdown' -X POST -s 'http://localhost:8000/cgi-bin/vars.cgi'
AUTH_TYPE undefined
CONTENT_LENGTH='821'
CONTENT_TYPE='text/markdown'
GATEWAY_INTERFACE='CGI/1.1'
PATH_INFO=''
PATH_TRANSLATED='/home/lisergey/cgi'
QUERY_STRING=''
REMOTE_ADDR='127.0.0.1'
REMOTE_HOST=''
REMOTE_IDENT undefined
REMOTE_USER undefined
REQUEST_METHOD='POST'
SCRIPT_NAME='/cgi-bin/vars.cgi'
SERVER_NAME='penguin'
SERVER_PORT='8000'
SERVER_PROTOCOL='HTTP/1.0'
SERVER_SOFTWARE='SimpleHTTP/0.6 Python/3.9.2'
</pre>
