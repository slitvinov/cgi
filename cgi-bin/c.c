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

int main(int argc, char **argv) {
  const char *value;
  puts("\
Content-type: text/html\n\
\n\
<HTML>\n\
  <HEAD><TITLE>c</TITLE></HEAD>\n\
  <BODY>\n\
    <PRE>");
  while (*++argv) {
    html_escape(*argv);
    putchar('\n');
  }
  value = getenv("PATH_INFO");
  if (value) {
    fputs("PATH_INFO='", stdout);
    html_escape(value);
    putchar('\'');
  }
  puts("\
    </PRE>\n\
  </BODY>\n\
</HTML>");
  return 0;
}
