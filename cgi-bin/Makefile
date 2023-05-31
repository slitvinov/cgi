.POSIX:
.SUFFIXES:
.SUFFIXES: .cgi
.SUFFIXES: .c

CFLAGS = -O2 -g
CURLFLAGS = `curl-config --cflags` `curl-config --libs`
M = \
btc.cgi\
cat.cgi\
c.cgi\
error.cgi\
vars.cgi\

all: $M
.c.cgi:; $(CC) $< $(CFLAGS) -o $@
btc.cgi: btc.c; $(CC) btc.c $(CFLAGS) $(CURLFLAGS) -o $@
clean:; rm $M
