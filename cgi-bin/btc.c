#include <ctype.h>
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  char c;
  CURLcode res;
  CURL *curl_handle;
  size_t i;
  struct Buf mem;
  mem.buf = NULL;
  mem.size = 0;
  curl_global_init(CURL_GLOBAL_ALL);
  if ((curl_handle = curl_easy_init()) == NULL)
    goto err;
  curl_easy_setopt(
      curl_handle, CURLOPT_URL,
      "https://min-api.cryptocompare.com/data/price?tsyms=usd&fsym=btc");
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, callback);
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&mem);
  if ((res = curl_easy_perform(curl_handle)) != CURLE_OK)
    goto err;
  puts("Content-type: text/plain\n");
  for (i = 0; i < mem.size; i++) {
    c = mem.buf[i];
    if (isdigit(c) || c == '.')
      putchar(mem.buf[i]);
  }
  putchar('\n');
  curl_easy_cleanup(curl_handle);
  free(mem.buf);
  curl_global_cleanup();
  return 0;
err:
  puts("Content-Type: text/plain");
  puts("Status: 502 Curl failed\n");
  return 0;
}
