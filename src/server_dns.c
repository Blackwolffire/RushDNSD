#include <stdio.h>
#include <stdlib.h>

#include "dns_runner.h"

int main(int argc, char **argv)
{
  if (argc <= 2){
    printf("Usage: %s [port] [IPs]\n", argv[0]);
    return EXIT_SUCCESS;
  }

  dns_engine *engine = dns_init("prout", 53, "127.0.0.1");
  (void)engine;

  return EXIT_SUCCESS;
}
