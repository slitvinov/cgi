#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define TWOPI (2.0 * M_PI)

#define COLOR_SAT 0.95f
#define COLOR_LUM 0.55f
#define METALLIC  0.1
#define ROUGHNESS 0.6

enum { M_DEFAULT = 3, M_MIN = 2, M_MAX = 9 };
enum { N_DEFAULT = 120, N_MIN = 20, N_MAX = 250 };

enum { F32_BYTES = 4, U16_BYTES = 2, VEC3_BYTES = 12, QUAD_TRIS = 2, TRI_IDXS = 3 };

enum {
  GLB_VERSION = 2, GLB_HEADER_BYTES = 12, GLB_CHUNK_HDR_BYTES = 8, GLB_ALIGNMENT = 4,
  JSON_PAD_BYTE = 0x20, BIN_PAD_BYTE = 0x00, JSON_BUF_SIZE = 8192,
};

enum {
  GL_ARRAY_BUFFER = 34962, GL_ELEMENT_ARRAY_BUFFER = 34963,
  GL_FLOAT = 5126, GL_UNSIGNED_SHORT = 5123, GL_TRIANGLES = 4,
};

static int qs_int(const char *qs, const char *key, int def, int lo, int hi) {
  if (!qs) return def;
  size_t klen = strlen(key);
  const char *p = qs;
  while (*p) {
    if (strncmp(p, key, klen) == 0 && p[klen] == '=') {
      const char *v = p + klen + 1;
      char *end;
      long n = strtol(v, &end, 10);
      if (end != v) {
        if (n < lo) n = lo;
        if (n > hi) n = hi;
        return (int)n;
      }
    }
    p = strchr(p, '&');
    if (!p) break;
    p++;
  }
  return def;
}

static void wu32(unsigned char *b, size_t o, uint32_t v) {
  b[o]   = (unsigned char)(v & 0xff);
  b[o+1] = (unsigned char)((v >> 8) & 0xff);
  b[o+2] = (unsigned char)((v >> 16) & 0xff);
  b[o+3] = (unsigned char)((v >> 24) & 0xff);
}
static void wu16(unsigned char *b, size_t o, uint16_t v) {
  b[o]   = (unsigned char)(v & 0xff);
  b[o+1] = (unsigned char)((v >> 8) & 0xff);
}
static void wf32(unsigned char *b, size_t o, float v) {
  uint32_t u;
  memcpy(&u, &v, F32_BYTES);
  wu32(b, o, u);
}

static void hsl2rgb(float h, float s, float l, float *r, float *g, float *b) {
  if (s == 0.0f) { *r = *g = *b = l; return; }
  float q = l < 0.5f ? l * (1.0f + s) : l + s - l * s;
  float p = 2.0f * l - q;
  float m[3];
  for (int i = 0; i < 3; i++) {
    float t = h * 6.0f + (float)i * 2.0f;
    if (t < 0.0f) t += 6.0f;
    if (t > 6.0f) t -= 6.0f;
    if      (t < 1.0f) m[i] = p + (q - p) * t;
    else if (t < 3.0f) m[i] = q;
    else if (t < 4.0f) m[i] = p + (q - p) * (4.0f - t);
    else               m[i] = p;
  }
  *r = m[0]; *g = m[1]; *b = m[2];
}

static void tranguloid(double u, double v, int m,
                       double *x, double *y, double *z) {
  double phase = TWOPI / (double)m;
  double cu  = cos(u),    su  = sin(u);
  double c2u = cos(2*u),  s2u = sin(2*u);
  double cv  = cos(v),    cvp = cos(v + phase);
  *x = 2.0 * sin((double)m * u) / (2.0 + cv);
  *y = 2.0 * (su + 2.0 * s2u) / (2.0 + cvp);
  *z = (cu - 2.0 * c2u) * (2.0 + cv) * (2.0 + cvp) / 4.0;
}

int main(void) {
  const char *qs = getenv("QUERY_STRING");
  int m = qs_int(qs, "m", M_DEFAULT, M_MIN, M_MAX);
  int N = qs_int(qs, "n", N_DEFAULT, N_MIN, N_MAX);

  const int nv = (N + 1) * (N + 1);
  const int ni = N * N * QUAD_TRIS * TRI_IDXS;

  const size_t pos_bytes = (size_t)nv * VEC3_BYTES;
  const size_t col_bytes = (size_t)nv * VEC3_BYTES;
  const size_t idx_bytes = (size_t)ni * U16_BYTES;
  const size_t binlen = pos_bytes + col_bytes + idx_bytes;

  unsigned char *bin = malloc(binlen);
  if (!bin) { fprintf(stderr, "tranguloid.cgi: oom\n"); return 1; }

  double minx = INFINITY, miny = INFINITY, minz = INFINITY;
  double maxx = -INFINITY, maxy = -INFINITY, maxz = -INFINITY;
  size_t off = 0;

  for (int i = 0; i <= N; i++) {
    double u = -M_PI + TWOPI * (double)i / (double)N;
    for (int j = 0; j <= N; j++) {
      double v = -M_PI + TWOPI * (double)j / (double)N;
      double px, py, pz;
      tranguloid(u, v, m, &px, &py, &pz);
      wf32(bin, off, (float)px); off += F32_BYTES;
      wf32(bin, off, (float)py); off += F32_BYTES;
      wf32(bin, off, (float)pz); off += F32_BYTES;
      if (px < minx) minx = px;
      if (px > maxx) maxx = px;
      if (py < miny) miny = py;
      if (py > maxy) maxy = py;
      if (pz < minz) minz = pz;
      if (pz > maxz) maxz = pz;
    }
  }

  size_t coff = pos_bytes;
  for (int i = 0; i <= N; i++) {
    float hue = (float)i / (float)N;
    float r, g, b;
    hsl2rgb(hue, COLOR_SAT, COLOR_LUM, &r, &g, &b);
    for (int j = 0; j <= N; j++) {
      wf32(bin, coff, r); coff += F32_BYTES;
      wf32(bin, coff, g); coff += F32_BYTES;
      wf32(bin, coff, b); coff += F32_BYTES;
    }
  }

  size_t ioff = pos_bytes + col_bytes;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      int a  = i * (N + 1) + j;
      int b_ = a + 1;
      int c  = (i + 1) * (N + 1) + j;
      int d  = c + 1;
      wu16(bin, ioff, (uint16_t)a);  ioff += U16_BYTES;
      wu16(bin, ioff, (uint16_t)c);  ioff += U16_BYTES;
      wu16(bin, ioff, (uint16_t)b_); ioff += U16_BYTES;
      wu16(bin, ioff, (uint16_t)b_); ioff += U16_BYTES;
      wu16(bin, ioff, (uint16_t)c);  ioff += U16_BYTES;
      wu16(bin, ioff, (uint16_t)d);  ioff += U16_BYTES;
    }
  }

  char json[JSON_BUF_SIZE];
  int jn = snprintf(json, sizeof(json),
    "{\"asset\":{\"version\":\"2.0\",\"generator\":\"tranguloid.cgi\"},"
    "\"scene\":0,\"scenes\":[{\"nodes\":[0]}],"
    "\"nodes\":[{\"mesh\":0}],"
    "\"meshes\":[{\"primitives\":[{\"attributes\":{\"POSITION\":0,\"COLOR_0\":1},"
    "\"indices\":2,\"material\":0,\"mode\":%d}]}],"
    "\"materials\":[{\"pbrMetallicRoughness\":{"
    "\"baseColorFactor\":[1,1,1,1],\"metallicFactor\":%.1f,\"roughnessFactor\":%.1f},"
    "\"doubleSided\":true}],"
    "\"buffers\":[{\"byteLength\":%zu}],"
    "\"bufferViews\":["
    "{\"buffer\":0,\"byteOffset\":0,\"byteLength\":%zu,\"target\":%d},"
    "{\"buffer\":0,\"byteOffset\":%zu,\"byteLength\":%zu,\"target\":%d},"
    "{\"buffer\":0,\"byteOffset\":%zu,\"byteLength\":%zu,\"target\":%d}"
    "],"
    "\"accessors\":["
    "{\"bufferView\":0,\"componentType\":%d,\"count\":%d,\"type\":\"VEC3\","
    "\"max\":[%.6g,%.6g,%.6g],\"min\":[%.6g,%.6g,%.6g]},"
    "{\"bufferView\":1,\"componentType\":%d,\"count\":%d,\"type\":\"VEC3\"},"
    "{\"bufferView\":2,\"componentType\":%d,\"count\":%d,\"type\":\"SCALAR\"}"
    "]}",
    GL_TRIANGLES,
    METALLIC, ROUGHNESS,
    binlen,
    pos_bytes, GL_ARRAY_BUFFER,
    pos_bytes, col_bytes, GL_ARRAY_BUFFER,
    pos_bytes + col_bytes, idx_bytes, GL_ELEMENT_ARRAY_BUFFER,
    GL_FLOAT, nv, maxx, maxy, maxz, minx, miny, minz,
    GL_FLOAT, nv,
    GL_UNSIGNED_SHORT, ni);
  if (jn < 0 || jn >= (int)sizeof(json)) {
    fprintf(stderr, "tranguloid.cgi: json overflow\n");
    free(bin);
    return 1;
  }
  size_t jsonlen = (size_t)jn;

  size_t json_pad = (jsonlen + (GLB_ALIGNMENT - 1)) & ~(size_t)(GLB_ALIGNMENT - 1);
  size_t bin_pad  = (binlen  + (GLB_ALIGNMENT - 1)) & ~(size_t)(GLB_ALIGNMENT - 1);
  size_t total    = GLB_HEADER_BYTES + GLB_CHUNK_HDR_BYTES + json_pad
                  + GLB_CHUNK_HDR_BYTES + bin_pad;

  printf("Content-Type: model/gltf-binary\n\n");

  unsigned char hdr[GLB_HEADER_BYTES];
  hdr[0] = 'g'; hdr[1] = 'l'; hdr[2] = 'T'; hdr[3] = 'F';
  wu32(hdr, 4, GLB_VERSION);
  wu32(hdr, 8, (uint32_t)total);
  fwrite(hdr, 1, GLB_HEADER_BYTES, stdout);

  unsigned char ch0[GLB_CHUNK_HDR_BYTES];
  wu32(ch0, 0, (uint32_t)json_pad);
  ch0[4] = 'J'; ch0[5] = 'S'; ch0[6] = 'O'; ch0[7] = 'N';
  fwrite(ch0, 1, GLB_CHUNK_HDR_BYTES, stdout);
  fwrite(json, 1, jsonlen, stdout);
  for (size_t k = jsonlen; k < json_pad; k++) putchar(JSON_PAD_BYTE);

  unsigned char ch1[GLB_CHUNK_HDR_BYTES];
  wu32(ch1, 0, (uint32_t)bin_pad);
  ch1[4] = 'B'; ch1[5] = 'I'; ch1[6] = 'N'; ch1[7] = BIN_PAD_BYTE;
  fwrite(ch1, 1, GLB_CHUNK_HDR_BYTES, stdout);
  fwrite(bin, 1, binlen, stdout);
  for (size_t k = binlen; k < bin_pad; k++) putchar(BIN_PAD_BYTE);

  if (fflush(stdout) != 0 || ferror(stdout)) {
    fprintf(stderr, "tranguloid.cgi: output error\n");
    free(bin);
    return 1;
  }
  free(bin);
  return 0;
}
