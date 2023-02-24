.POSIX:
.SUFFIXES:
.SUFFIXES: .cgi
.SUFFIXES: .c

CFLAGS = -O2 -g
M = c.cgi

all: $M
.c.cgi:; $(CC) $(CFLAGS) $< -o $@

clean:; rm $M
