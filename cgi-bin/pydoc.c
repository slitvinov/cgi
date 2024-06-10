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

static char from_hex(char ch) {
  return isdigit(ch) ? ch - '0' : tolower(ch) - 'a' + 10;
}

static char *url_decode(char *str) {
  char *pstr = str, *buf = malloc(strlen(str) + 1), *pbuf = buf;
  while (*pstr) {
    if (*pstr == '%') {
      if (pstr[1] && pstr[2]) {
	*pbuf++ = from_hex(pstr[1]) << 4 | from_hex(pstr[2]);
	pstr += 2;
      }
    } else if (*pstr == '+') {
      *pbuf++ = ' ';
    } else {
      *pbuf++ = *pstr;
    }
    pstr++;
  }
  *pbuf = '\0';
  return buf;
}
