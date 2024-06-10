#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
static char *url_decode(char *);
static const char name[] = "pydoc=";
int main() {
  char *query_string, *query_decoded;
  int status;
  if (puts("Content-Type: text/plain\n") < 0)
    return 1;
  query_string = getenv("QUERY_STRING");
  if (query_string == NULL || strncmp(name, query_string, sizeof name - 1) != 0)
    return 1;
  query_string += sizeof name - 1;
  query_decoded = url_decode(query_string);
  if (fork() == 0)
    execlp("python", "python", "-m", "pydoc", query_decoded, (char *)NULL);
}

static car from_hex(car c) {
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
