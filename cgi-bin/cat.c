#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *names[] = {
    "AUTH_TYPE",       "CONTENT_LENGTH",  "CONTENT_TYPE", "GATEWAY_INTERFACE",
    "PATH_INFO",       "PATH_TRANSLATED", "QUERY_STRING", "REMOTE_ADDR",
    "REMOTE_HOST",     "REMOTE_IDENT",    "REMOTE_USER",  "REQUEST_METHOD",
    "SCRIPT_NAME",     "SERVER_NAME",     "SERVER_PORT",  "SERVER_PROTOCOL",
    "SERVER_SOFTWARE",
};
static const char *http[] = {
    "HTTP_ACCEPT",
    "HTTP_COOKIE",
    "HTTP_REFERER",
    "HTTP_USER_AGENT",
};

int main() {
  long n, i;
  int c;
  char *value, *end;

  if ((value = getenv("CONTENT_LENGTH")) == NULL || value[0] == '\0')
    goto err;
  n = strtol(value, &end, 10);
  if (*end != '\0')
    goto err;
  puts("Content-Type: text/plain\n");
  for (i = 0; i < sizeof names / sizeof *names; i++) {
    value = getenv(names[i]);
    if (value == NULL)
      printf("%s undefined\n", names[i]);
    else
      printf("%s='%s'\n", names[i], value);
  }
  for (i = 0; i < n; i++) {
    c = getchar();
    putchar(c);
  }
  return 0;
err:
  puts("Content-Type: text/plain");
  puts("Status: 501 Not Implemented\n");
  puts("CONTENT_LENGTH is not set, empty, or not a number; nothing to cat");
  return 1;
}
