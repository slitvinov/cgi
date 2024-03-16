#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
static const char name[] = "pydoc=";
int main() {
  char *query_string;
  int status;
  if (puts("Content-Type: text/plain\n") < 0)
    return 1;
  query_string = getenv("QUERY_STRING");
  if (query_string == NULL || strncmp(name, query_string, sizeof name - 1) != 0)
    return 1;
  query_string += sizeof name - 1;
  if (fork() == 0)
    execlp("python", "python", "-m", "pydoc", query_string, (char *)NULL);
}
