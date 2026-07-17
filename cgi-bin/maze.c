#include <stdio.h>
#include <stdlib.h>

static const char *const escapes[256] = {
    ['<'] = "&lt;",
    ['>'] = "&gt;",
    ['&'] = "&amp;",
    ['"'] = "&quot;",
    ['\''] = "&#39;",
};

static void html_escape(const char *s) {
  for (; *s; s++) {
    const char *e = escapes[(unsigned char)*s];
    if (e)
      fputs(e, stdout);
    else
      putchar(*s);
  }
}

int main(void) {
  const char *path = getenv("PATH_INFO");
  const char *script;

  if (!path) {
    script = getenv("SCRIPT_NAME");
    if (!script)
      script = "";
    printf("Status: 301 Moved Permanently\n"
           "Location: %s/\n\n",
           script);
    return 0;
  }

  fputs("\
Content-type: text/html\n\
\n\
<html><body>\n\
<h1>Index of ",
        stdout);
  html_escape(path);
  puts("\
</h1>\n\
<a href=\"../\">../</a><br>\n\
<a href=\"a/\">a/</a><br>\n\
<a href=\"b/\">b/</a><br>\n\
<a href=\"c/\">c/</a><br>\n\
</body></html>");
  return 0;
}
