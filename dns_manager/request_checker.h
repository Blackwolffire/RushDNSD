#pragma once
#include "dns.h"
#include "analyser.h"

// If request's fileds are valid return 1 else 0
int request_checker(dns *request);
