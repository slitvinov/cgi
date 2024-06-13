#define _XOPEN_SOURCE 700
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
static char *url_decode(char *);
static const char name[] = "pydoc=";
int main(int argc, char **argv) {
  char *query_string, *query_decoded;
  if (puts("Content-Type: text/plain\n") < 0)
    return 1;
  query_string = getenv("QUERY_STRING");
  if (query_string == NULL || strncmp(name, query_string, sizeof name - 1) != 0)
    return 1;
  query_string += sizeof name - 1;
  query_decoded = url_decode(query_string);
  printf("%s\n", argv[0]);
  if (fork() == 0)
    if (strcmp(argv[0], "pydoc.cgi") == 0)
      execlp("python", "python", "-m", "pydoc", query_decoded, (char *)NULL);
    else
      execlp("man", "man", query_decoded, (char *)NULL);
}

static char from_hex(char c) {
  return isdigit(c) ? c - '0' : tolower(c) - 'a' + 10;
}

static char *url_decode(char *s) {
  char *buf = malloc(strlen(s) + 1), *p = buf;
  while (*s) {
    if (*s == '%') {
      if (s[1] && s[2]) {
        *p++ = from_hex(s[1]) << 4 | from_hex(s[2]);
        s += 2;
      }
    } else if (*s == '+') {
      *p++ = ' ';
    } else {
      *p++ = *s;
    }
    s++;
  }
  *p = '\0';
  return buf;
}
