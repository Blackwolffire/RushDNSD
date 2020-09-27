# Compiler and flags
CC=gcc
CFLAGS= -std=c99 -Wall -Wextra -Werror -pedantic -g
CFLAGS+= -Isrc
CFLAGS+= -Isrc/request_to_response
CFLAGS+= -Isrc/collector -Isrc/catch

LDLIBS=
VPATH= src/
VPATH+= src/request_to_response/
VPATH+= src/collector/ src/catch

EXEC = server_dns

# FILES
SRC= server_dns.c
SRC+= analyser.c request_checker.c request_processer.c find_zone.c
SRC+= dns_runner.c network_wrapper.c
SRC+= dns_engine.c my_free.c my_libc.c
SRC+= response_forger.c
SRC+= request_parser.c
SRC+= catch.c collector.c



OBJS= ${SRC:.c=.o}

all: ${EXEC}
	${RM} ${OBJS}

${EXEC}: ${OBJS}

clean:
	${RM} ${OBJS} ${EXEC}
