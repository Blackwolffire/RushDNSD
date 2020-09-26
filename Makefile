# Compiler and flags
CC=gcc
CFLAGS= -std=c99 -Wall -Wextra -Werror -pedantic -g
CFLAGS+= -Isrc
CFLAGS+= -Isrc/request_to_response
LDLIBS=
VPATH= src/
VPATH+= src/request_to_response/

EXEC = server_dns
# FILES
SRC= server_dns.c
SRC+= analyser.c request_checker.c request_processer.c
SRC+= server_dns.c dns_runner.c network_wrapper.c
OBJS= ${SRC:.c=.o}

all: ${EXEC}

${EXEC}: ${OBJS}

clean:
	${RM} ${OBJS} ${EXEC}
