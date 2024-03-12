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
  int i, c;
  long content_length, j;
  char *value;
  if (puts("Content-Type: text/plain\n") < 0)
    return 1;
  for (i = 0; i < sizeof names / sizeof *names; i++) {
    value = getenv(names[i]);
    if (value == NULL) {
      if (printf("%s undefined\n", names[i]) < 0)
        return 1;
    } else {
      if (printf("%s='%s'\n", names[i], value) < 0)
        return 1;
    }
  }
  if ((value = getenv("SERVER_PROTOCOL")) != NULL) {
    if (strcmp("INCLUDED", value) == 0 || strncmp("HTTP", value, 4) == 0)
      for (i = 0; i < sizeof http / sizeof *http; i++) {
        value = getenv(http[i]);
        if (value == NULL) {
          if (printf("    %s undefined\n", http[i]) < 0)
            return 1;
        } else {
          if (printf("    %s='%s'\n", http[i], value) < 0)
            return 1;
        }
      }
  }
  content_length = atol(getenv("CONTENT_LENGTH"));
  if (content_length > 0) {
    for (i = 0; i < content_length; i++) {
      c = getchar();
      putchar(c);
    }
  }
}
