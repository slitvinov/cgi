.POSIX:
.SUFFIXES:
.SUFFIXES: .cgi
.SUFFIXES: .c

CFLAGS = -O2 -g
M = \
cat.cgi\
c.cgi\
error.cgi\
vars.cgi\

all: $M
.c.cgi:; $(CC) $(CFLAGS) $< -o $@

clean:; rm $M
