#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  const char *value;
  puts("\
Content-type: text/html\n\
\n\
<HTML>\n\
  <HEAD><TITLE>c</TITLE></HEAD>\n\
  <BODY>\n\
    <PRE>");
  while (*++argv)
    printf("%s\n", *argv);
  value = getenv("PATH_INFO");
  if (value) {
    fputs("PATH_INFO='", stdout);
    fputs(value, stdout);
    putchar('\'');
  }
  puts("\
    </PRE>\n\
  </BODY>\n\
</HTML>");
}
