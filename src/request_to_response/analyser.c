#include "analyser.h"

dns *anayser(dns *request){
	if (request_checker(request) == 0)
	{
		// Invalid request
		// forge response for invqlid request

	}
	else
	{
		// valid request
		return make_valid_response(request);
	}
	return request;
}

int func(){
	return 3;
}
