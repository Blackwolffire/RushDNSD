#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _POSIX_C_SOURCE 2

#include <unistd.h>
#include <getopt.h>

#include "dns_runner.h"

int main(int argc, char **argv)
{
  char *ip = NULL, *filename = NULL;
  uint16_t port = 0;
  int opt;
  int len;

  if (argc <= 3){
    printf("Usage: %s -p [port] -a [IP, IP, ...] -f [filename]\n", argv[0]);
    return EXIT_FAILURE;
  }

  while ((opt = getopt(argc, argv, "paf")) != -1){
    switch (opt){
      case 'p':
        port = atoi(optarg);
        break;
      case 'a':
        len = strlen(optarg);
        ip = calloc(len + 1, sizeof(char));
        strncpy(ip, optarg, len);
        break;
      case 'f':
        len = strlen(optarg);
        filename = calloc(len + 1, sizeof(char));
        strncpy(filename, optarg, len);
        break;
      default:
        printf("Usage: %s -p [port] -a [IP, IP, ...] -f [filename]\n",
               argv[0]);
        if (filename)
          free(filename);
        if (ip)
          free(ip);
        return EXIT_FAILURE;
    }
  }

  if (!filename){
    printf("Usage: %s -p [port] -a [IP, IP, ...] -f [filename]\n",
           argv[0]);
    if (ip)
      free(ip);
    return EXIT_FAILURE;
  }

  if (!ip){
    ip = calloc(10, sizeof(char));
    strncpy(ip, "127.0.0.1", 10);
  }
  if (!port)
    port = 53;


  dns_engine *engine = dns_init(filename, port, ip);
  dns_run(engine);
  dns_quit(engine);

  free(ip);
  return EXIT_SUCCESS;
}
