#include <stdio.h>
#include <stdlib.h>

static const char *names[] = {
    "AUTH_TYPE",      "CONTENT_LENGTH",  "CONTENT_TYPE", "GATEWAY_INTERFACE",
    "PATH_INFO",      "PATH_TRANSLATED", "QUERY_STRING", "REMOTE_ADDR",
    "REMOTE_HOST",    "REMOTE_IDENT",    "REMOTE_USER",  "REQUEST_METHOD",
    "SCRIPT_NAME",    "SERVER_NAME",     "SERVER_PORT",  "SERVER_PROTOCOL",
    "SERVER_SOFTWARE"};

int main() {
  int i;
  char *value;
  if (puts("Content-Type: text/plain\n") < 0)
    return 1;
  for (i = 0; i < sizeof names / sizeof *names; i++) {
    value = getenv(names[i]);
    if (value == NULL)
      printf("%s undefined\n", names[i]);
    else
      printf("%s='%s'\n", names[i], value);
  }
  return 0;
}
