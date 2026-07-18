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
bmp.cgi\
maze.cgi\
map.cgi\
nph-julia.cgi\
error.cgi\
man.cgi\
pydoc.cgi\
vars.cgi\

all: $M
.c.cgi:; $(CC) $< $(CFLAGS) -o $@
man.cgi:; $(CC) $< $(CFLAGS) -o $@ pydoc.c
btc.cgi: btc.c; @set -x; $(CURLCC) btc.c $(CFLAGS) $(CURLFLAGS) -o $@
map.cgi: map.c; $(CC) map.c $(CFLAGS) -fopenmp -o $@
nph-julia.cgi: nph-julia.c; $(CC) nph-julia.c $(CFLAGS) -fopenmp -o $@ -lm
clean:; rm $M
