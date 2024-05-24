BIN = triakontadis
CC = clang


#CFLAGS =
CFLAGS = -Wall -Wextra -pedantic -std=c89 -Wno-unused-parameter -Wno-unused-function -Wno-unused-variable -Wno-unused-result -Wno-sign-compare -MJ $@.json
#OPT = -DISOLA_DBG -g
OPT = -Ofast3 -pipe -march=native -flto=full


LIBS = -lGLEW -lGL -lGLU -lSDL2 -lm
LDFLAGS = ${LIBS}

HDR = isola.h render.h logic.h
SRC = main.c isola.c render.c logic.c
OBJ = ${SRC:.c=.o}

DEPS = ${SRC} ${HDR} Makefile



default_rule: test

test: ${BIN}
	./${BIN}
	make clean


.c.o:
	${CC} -c ${CFLAGS} ${OPT} $<

${OBJ}: ${DEPS}



all: ${BIN} Compdb clean

${BIN}: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS} ${OPT}

Compdb: ${OBJ}
	./Compdb

clean:
	rm *.o*
	rm ${BIN}


