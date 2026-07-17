#include <ctype.h>
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { MAX_RESPONSE_SIZE = 65536 };

struct Buf {
  char *buf;
  size_t size;
};

static size_t callback(void *contents, size_t size, size_t nmemb, void *userp) {
  char *ptr;
  size_t realsize;
  struct Buf *mem;

  realsize = size * nmemb;
  mem = userp;
  if (mem->size + realsize > MAX_RESPONSE_SIZE) {
    fprintf(stderr, "btc: response exceeds %d bytes, aborting\n",
            MAX_RESPONSE_SIZE);
    return 0;
  }
  if ((ptr = realloc(mem->buf, mem->size + realsize)) == NULL) {
    fprintf(stderr, "btc: realloc failed\n");
    return 0;
  }
  mem->buf = ptr;
  memcpy(&(mem->buf[mem->size]), contents, realsize);
  mem->size += realsize;
  return realsize;
}

int main(void) {
  CURLcode res;
  CURL *curl_handle = NULL;
  long http_code = 0;
  int opt_failed = 0;
  size_t i;
  struct Buf mem;
  mem.buf = NULL;
  mem.size = 0;

  curl_global_init(CURL_GLOBAL_ALL);
  if ((curl_handle = curl_easy_init()) == NULL) {
    fprintf(stderr, "btc: curl_easy_init failed\n");
    goto err;
  }

  opt_failed |= curl_easy_setopt(
      curl_handle, CURLOPT_URL,
      "https://api.coingecko.com/api/v3/simple/price?ids=bitcoin&vs_currencies=usd");
  opt_failed |= curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, callback);
  opt_failed |= curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&mem);
  opt_failed |= curl_easy_setopt(curl_handle, CURLOPT_CONNECTTIMEOUT, 5L);
  opt_failed |= curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, 10L);
  opt_failed |= curl_easy_setopt(
      curl_handle, CURLOPT_USERAGENT,
      "btc.cgi/1.0 (+https://oracle.slitvinov.com)");
  opt_failed |= curl_easy_setopt(curl_handle, CURLOPT_NOSIGNAL, 1L);
  opt_failed |= curl_easy_setopt(curl_handle, CURLOPT_PROTOCOLS_STR, "https");
  opt_failed |= curl_easy_setopt(curl_handle, CURLOPT_MAXFILESIZE_LARGE,
                                  (curl_off_t)MAX_RESPONSE_SIZE);
  if (opt_failed) {
    fprintf(stderr, "btc: curl_easy_setopt failed\n");
    goto err;
  }

  if ((res = curl_easy_perform(curl_handle)) != CURLE_OK) {
    fprintf(stderr, "btc: curl_easy_perform failed: %s\n",
            curl_easy_strerror(res));
    goto err;
  }

  curl_easy_getinfo(curl_handle, CURLINFO_RESPONSE_CODE, &http_code);
  if (http_code != 200) {
    fprintf(stderr, "btc: unexpected HTTP status %ld\n", http_code);
    goto err;
  }

  puts("Content-Type: text/plain\n");
  for (i = 0; i < mem.size; i++)
    if (isdigit((unsigned char)mem.buf[i]) || mem.buf[i] == '.')
      putchar(mem.buf[i]);
  putchar('\n');

  free(mem.buf);
  curl_easy_cleanup(curl_handle);
  curl_global_cleanup();
  return 0;

err:
  puts("Content-Type: text/plain");
  puts("Status: 502 Curl failed\n");
  free(mem.buf);
  if (curl_handle)
    curl_easy_cleanup(curl_handle);
  curl_global_cleanup();
  return 0;
}
