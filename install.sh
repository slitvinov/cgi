#!/bin/sh

: ${WGET=wget}
: ${PREFIX=$HOME/.local}
: ${WORK=`pwd`/.w}

me=install.sh
err () {
    printf >&2 %s\\n "$me: $*"
    exit 2
}

"$WGET" -q -nc http://acme.com/software/mini_httpd/mini_httpd-1.30.tar.gz || err 'wget failed'
tar zxf mini_httpd-1.30.tar.gz  --strip-components 1 || err 'tar failed'
make CC='cc -DHAVE_INT64T' BINDIR=$PREFIX/bin MANDIR=$PREFIX/man || err 'make failed'
make install BINDIR=$PREFIX/bin MANDIR=$PREFIX/man || err 'make install failed'

printf >&2 "***\nopen http://localhost:8080/cgi-bin/test\n***\n"
mini_httpd -D -c 'cgi-bin/*' -p 8080
