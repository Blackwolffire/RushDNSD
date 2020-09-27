#include <stdio.h>
#include <stdlib.h>

#include "dns_runner.h"

int main(int argc, char **argv)
{
  char ip[] = "127.0.0.1";
  if (argc <= 2){
    printf("Usage: %s [port] [IPs]\n", argv[0]);
    return EXIT_SUCCESS;
  }

  dns_engine *engine = dns_init("prout", 53, ip);
  (void)engine;
  dns_quit(engine);

  return EXIT_SUCCESS;
}
