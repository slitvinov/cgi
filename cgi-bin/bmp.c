#include <stdio.h>
#include <time.h>
#include <unistd.h>

enum { w = 200, h = 200 };

static unsigned int rng_state;

/* xorshift32: hand-rolled PRNG, no library random functions used */
static unsigned int next_rand(void) {
  rng_state ^= rng_state << 13;
  rng_state ^= rng_state >> 17;
  rng_state ^= rng_state << 5;
  return rng_state;
}

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
  int stack_marker;
  unsigned int rx, ry, px, py;
  struct timespec ts;

  clock_gettime(CLOCK_REALTIME, &ts);
  rng_state = (unsigned int)ts.tv_nsec ^ (unsigned int)getpid() ^
              (unsigned int)(unsigned long)&stack_marker;
  if (rng_state == 0)
    rng_state = 1;

  /* random per-request offset: same XOR pattern, different phase each time */
  rx = next_rand() & 0xff;
  ry = next_rand() & 0xff;

  printf("Content-type: image/bmp\n\n");

  /* file header (14 bytes) */
  putchar('B');
  putchar('M');
  put_u32(file_size);
  put_u32(0);  /* reserved */
  put_u32(54); /* pixel data offset */

  /* DIB header: BITMAPINFOHEADER (40 bytes) */
  put_u32(40);
  put_u32(w);
  put_u32(h);
  put_u16(1);  /* planes */
  put_u16(24); /* bits per pixel */
  put_u32(0);  /* compression */
  put_u32(0);  /* image size */
  put_u32(0);  /* x pixels per meter */
  put_u32(0);  /* y pixels per meter */
  put_u32(0);  /* colors used */
  put_u32(0);  /* important colors */

  for (y = 0; y < h; y++) {
    for (x = 0; x < w; x++) {
      px = (x + rx) & 0xff;
      py = (y + ry) & 0xff;
      putchar((px ^ py) & 0xff); /* B */
      putchar(px);               /* G */
      putchar(py);               /* R */
    }
    for (i = 0; i < pad; i++)
      putchar(0);
  }

  if (fflush(stdout) != 0 || ferror(stdout)) {
    fprintf(stderr, "bmp.cgi: output error\n");
    return 1;
  }
  return 0;
}
