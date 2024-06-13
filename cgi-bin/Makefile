.POSIX:
.SUFFIXES:
.SUFFIXES: .cgi
.SUFFIXES: .c

CFLAGS = -O2 -g
CURLFLAGS = `curl-config --cflags --libs`
CURLCC = `curl-config --cc`
M = \
btc.cgi\
cat.cgi\
c.cgi\
error.cgi\
man.cgi\
pydoc.cgi\
vars.cgi\

all: $M
.c.cgi:; $(CC) $< $(CFLAGS) -o $@
man.cgi:; $(CC) $< $(CFLAGS) -o $@ pydoc.c
btc.cgi: btc.c; @set -x; $(CURLCC) btc.c $(CFLAGS) $(CURLFLAGS) -o $@
clean:; rm $M
