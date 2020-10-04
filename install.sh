#!/bin/sh

: ${PREFIX=$HOME/.local}

me=install.sh
err () {
    cat <<!
$me: $*
!
exit 2
}

(cd .w
 tar zxf mini_httpd-1.30.tar.gz  --strip-components 1 || err 'tar failed'
 make CC='cc -DHAVE_INT64T' BINDIR=$PREFIX/bin MANDIR=$PREFIX/man || err 'make failed'
 make install BINDIR=$PREFIX/bin MANDIR=$PREFIX/man || err 'make install failed'
) || exit 2

cat >&2 <<'!'
***
open http://localhost:8080/cgi-bin/main
or
open http://localhost:8080/cgi-bin/main?key=val
***
!

mini_httpd -D -c 'cgi-bin/*' -p 8080
