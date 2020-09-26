#pragma once

#include <stdlib.h>
#include "dns.h"


void *response_forge(dns *ans); // struct -> packet (void*)
