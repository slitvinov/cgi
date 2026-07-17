#include <stdio.h>
#include <stdlib.h>

static void html_escape(const char *s) {
  for (; *s; s++) {
    switch (*s) {
    case '<':
      fputs("&lt;", stdout);
      break;
    case '>':
      fputs("&gt;", stdout);
      break;
    case '&':
      fputs("&amp;", stdout);
      break;
    case '"':
      fputs("&quot;", stdout);
      break;
    case '\'':
      fputs("&#39;", stdout);
      break;
    default:
      putchar(*s);
    }
  }
}

int main(void) {
  const char *path = getenv("PATH_INFO");
  const char *script;

  if (!path) {
    script = getenv("SCRIPT_NAME");
    if (!script)
      script = "";
    printf("Status: 301 Moved Permanently\n");
    printf("Location: %s/\n\n", script);
    return 0;
  }

  printf("Content-type: text/html\n\n");
  printf("<html><body>\n<h1>Index of ");
  html_escape(path);
  printf("</h1>\n");
  printf("<a href=\"../\">../</a><br>\n");
  printf("<a href=\"a/\">a/</a><br>\n");
  printf("<a href=\"b/\">b/</a><br>\n");
  printf("<a href=\"c/\">c/</a><br>\n");
  printf("</body></html>\n");
  return 0;
}
