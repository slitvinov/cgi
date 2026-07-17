#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

enum { w = 200, h = 200 };

static const unsigned char palette[][3] = {
    {200, 60, 60},
    {60, 200, 60},
    {60, 60, 200},
    {200, 200, 60},
};
enum { npalette = sizeof palette / sizeof *palette };

static unsigned char pixels[h][w];

static void put_u16(unsigned short v) {
  putchar(v & 0xff);
  putchar((v >> 8) & 0xff);
}

static void put_u32(unsigned int v) {
  putchar(v & 0xff);
  putchar((v >> 8) & 0xff);
  putchar((v >> 16) & 0xff);
  putchar((v >> 24) & 0xff);
}

int main(void) {
  int row_size = ((w * 3 + 3) / 4) * 4;
  int pad = row_size - w * 3;
  int file_size = 14 + 40 + row_size * h;
  int x, y, i;
  int nthreads = 4;
  const char *path = getenv("PATH_INFO");

  if (path && *path == '/')
    path++;
  if (path && *path) {
    char *end;
    long n = strtol(path, &end, 10);
    if (*end == '\0' && n > 0)
      nthreads = (int)n;
    else {
      puts("Status: 400 Bad Request\n"
           "Content-type: text/plain\n"
           "\n"
           "bad thread count");
      return 1;
    }
  }
  omp_set_num_threads(nthreads);

#pragma omp parallel for schedule(static)
  for (y = 0; y < h; y++) {
    int tid = omp_get_thread_num() % npalette;
    for (x = 0; x < w; x++)
      pixels[y][x] = tid;
  }

  printf("Content-type: image/bmp\n\n");
  putchar('B');
  putchar('M');
  put_u32(file_size);
  put_u32(0);
  put_u32(54);
  put_u32(40);
  put_u32(w);
  put_u32(h);
  put_u16(1);
  put_u16(24);
  put_u32(0);
  put_u32(0);
  put_u32(0);
  put_u32(0);
  put_u32(0);
  put_u32(0);

  for (y = 0; y < h; y++) {
    for (x = 0; x < w; x++) {
      const unsigned char *c = palette[pixels[y][x]];
      putchar(c[0]);
      putchar(c[1]);
      putchar(c[2]);
    }
    for (i = 0; i < pad; i++)
      putchar(0);
  }
  return 0;
}
