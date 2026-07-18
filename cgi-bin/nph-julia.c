#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

enum { w = 320, h = 240, frames = 50, maxiter = 800 };

static const char boundary[] = "JULIAFRAME";

static unsigned char rgb[h][w][3];
static unsigned char bmp[54 + w * h * 3];

static void put_u16(unsigned char *p, unsigned short v) {
  p[0] = v & 0xff;
  p[1] = (v >> 8) & 0xff;
}

static void put_u32(unsigned char *p, unsigned int v) {
  p[0] = v & 0xff;
  p[1] = (v >> 8) & 0xff;
  p[2] = (v >> 16) & 0xff;
  p[3] = (v >> 24) & 0xff;
}

static size_t build_bmp(void) {
  int x, y;
  unsigned char *p;
  size_t n = sizeof bmp;
  memset(bmp, 0, 54);
  bmp[0] = 'B';
  bmp[1] = 'M';
  put_u32(bmp + 2, (unsigned int)n);
  put_u32(bmp + 10, 54);
  put_u32(bmp + 14, 40);
  put_u32(bmp + 18, w);
  put_u32(bmp + 22, h);
  put_u16(bmp + 26, 1);
  put_u16(bmp + 28, 24);
  p = bmp + 54;
  for (y = 0; y < h; y++)
    for (x = 0; x < w; x++) {
      *p++ = rgb[y][x][2];
      *p++ = rgb[y][x][1];
      *p++ = rgb[y][x][0];
    }
  return n;
}

int main(void) {
  int f, y, stack_marker;
  unsigned int seed;
  double theta0;
  double pixsize = 1.6 / (w / 2.0);
  struct timespec ts;

  omp_set_num_threads(4);

  clock_gettime(CLOCK_REALTIME, &ts);
  seed = (unsigned int)ts.tv_nsec ^ (unsigned int)getpid() ^
         (unsigned int)(unsigned long)&stack_marker;
  theta0 = (seed % 1000000) / 1000000.0 * 6.28318;

  printf("HTTP/1.0 200 OK\r\n"
         "Content-Type: multipart/x-mixed-replace; boundary=%s\r\n"
         "Cache-Control: no-cache\r\n"
         "\r\n",
         boundary);

  for (f = 0; f < frames; f++) {
    double theta = theta0 + f * 0.012;
    double jcr = 0.7885 * cos(theta);
    double jci = 0.7885 * sin(theta);
    size_t n;

#pragma omp parallel for schedule(dynamic)
    for (y = 0; y < h; y++) {
      int x;
      for (x = 0; x < w; x++) {
        double r = 0, g = 0, b = 0;
        int sx, sy;
        for (sy = 0; sy < 2; sy++)
          for (sx = 0; sx < 2; sx++) {
            double zr = (x - w / 2 + sx * 0.5 - 0.25) * pixsize;
            double zi = (y - h / 2 + sy * 0.5 - 0.25) * pixsize;
            double zr2 = zr * zr, zi2 = zi * zi;
            double dr = 1, di = 0, dtmp;
            int k = 0;
            while (k < maxiter && zr2 + zi2 < 65536.0) {
              dtmp = 2 * (zr * dr - zi * di);
              di = 2 * (zr * di + zi * dr);
              dr = dtmp;
              zi = 2 * zr * zi + jci;
              zr = zr2 - zi2 + jcr;
              zr2 = zr * zr;
              zi2 = zi * zi;
              k++;
            }
            if (k < maxiter) {
              double mz = sqrt(zr2 + zi2);
              double dist = mz * log(mz) / sqrt(dr * dr + di * di);
              double fade = dist / (0.5 * pixsize);
              double nu = log(0.5 * log(zr2 + zi2)) / log(2.0);
              double t = 0.11 * (k + 1 - nu);
              if (fade > 1)
                fade = 1;
              fade = pow(fade, 0.35);
              r += fade * (0.5 + 0.5 * cos(6.28318 * (t + 0.00)));
              g += fade * (0.5 + 0.5 * cos(6.28318 * (t + 0.10)));
              b += fade * (0.5 + 0.5 * cos(6.28318 * (t + 0.20)));
            }
          }
        rgb[y][x][0] = (unsigned char)(255 * r / 4);
        rgb[y][x][1] = (unsigned char)(255 * g / 4);
        rgb[y][x][2] = (unsigned char)(255 * b / 4);
      }
    }

    n = build_bmp();
    printf("--%s\r\n"
           "Content-Type: image/bmp\r\n"
           "Content-Length: %zu\r\n"
           "\r\n",
           boundary, n);
    fwrite(bmp, 1, n, stdout);
    printf("\r\n");

    if (fflush(stdout) != 0 || ferror(stdout)) {
      fprintf(stderr, "nph-julia.cgi: client gone at frame %d\n", f);
      return 0;
    }
  }

  printf("--%s--\r\n", boundary);
  fflush(stdout);
  return 0;
}
