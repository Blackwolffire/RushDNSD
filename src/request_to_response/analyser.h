#pragma once
#include "dns.h"
#include "dns_engine.h"
#include "request_processer.h"
#include "request_checker.h"
#include "find_zone.h"

// checks and makes final request
dns *anayser(dns *request, zone *soa, bin_tree *tree);
