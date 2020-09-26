CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -Werror -g
SOURCE= *.c
OUTPUT= dns

install:
	${CC} ${CFLAGS} -o ${OUTPUT} ${SOURCE}
