# Compiler and flags
CC=gcc
CFLAGS= -std=c99 -Wall -Wextra -Werror -pedantic
CFLAGS+= -Isrc 
CFLAGS+= -Isrc/request_to_response
LDLIBS= -lm -lSDL2
VPATH= src/
VPATH+= src/request_to_response/

EXEC = dns
# FILES
SRC= dns.c
SRC+= analyser.c request_checker.c request_processer.c
OBJS= ${SRC:.c=.o}

all: ${EXEC}

${EXEC}: ${OBJS}

clean:
	${RM} ${OBJS} ${EXEC}
