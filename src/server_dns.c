#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _POSIX_C_SOURCE 2

#include <unistd.h>

#include "dns_runner.h"

int main(int argc, char **argv)
{
  char *ip, *filename;
  uint16_t port;
  int opt;
  int len;

  if (argc <= 2){
    printf("Usage: %s -p [port] -a [IP, IP, ...] -f [filename]\n", argv[0]);
    return EXIT_SUCCESS;
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
    }
  }

  dns_engine *engine = dns_init("prout", 53, ip);
  dns_run(engine);
  dns_quit(engine);

  free(ip);
  return EXIT_SUCCESS;
}
