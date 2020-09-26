CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -Werror
SOURCE= *.c
OUTPUT= dns

install:
	${CC} ${CFLAGS} -o ${OUTPUT} ${SOURCE}
