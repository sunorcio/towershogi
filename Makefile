# to compile for linux, get the following binaries from its source or from your own distribution:
#  glew - https://github.com/nigels-com/glew
#  SDL2 - https://github.com/libsdl-org/SDL
#  opengl drivers




default: test


#[linux^windows]
TARGET_OS = linux
#[dynamic^static]
TARGET_LINK = dynamic
#[incremental^unified]
TARGET_BUILD = incremental

#[off^on]
TARGET_RELEASE = off
#[off^on]
TARGET_DEBUG = off
#[off^on]
TARGET_SANITIZE = off

#[string], binary name
TARGET_BIN = isola_example


# /\ /\ MAKEFILE CONFIGURATION /\ /\
# || ||                        || ||
# rules are at the bottom of this file




ifeq (${TARGET_OS}, linux)
else ifeq (${TARGET_OS}, windows)
else
 $(error wrong TARGET_OS value)
endif

ifeq (${TARGET_LINK}, dynamic)
else ifeq (${TARGET_LINK}, static)
else
 $(error wrong TARGET_LINK value)
endif

ifeq (${TARGET_BUILD}, incremental)
else ifeq (${TARGET_BUILD}, unified)
else
 $(error wrong TARGET_BUILD value)
endif

ifeq (${TARGET_RELEASE}, on)
else ifeq (${TARGET_RELEASE}, off)
else
 $(error wrong TARGET_RELEASE value)
endif

ifeq (${TARGET_DEBUG}, on)
else ifeq (${TARGET_DEBUG}, off)
else
 $(error wrong TARGET_DEBUG value)
endif

ifeq (${TARGET_SANITIZE}, on)
else ifeq (${TARGET_SANITIZE}, off)
else
 $(error wrong TARGET_SANITIZE value)
endif




#GLOBALDEP = Makefile
GLOBALDEP =
#PRERULE =
PRERULE = isola bin
#POSTRULE =
POSTRULE = compdb




ifeq (${TARGET_BUILD}, incremental)

# #HDR = ${shell find . -type f -name '*.h' ! -path '*/bin/*'}
#HDR =
#SRC =
SRC = ${shell find . -type f -name '*.c' ! -path '*/bin/*' ! -name 'all.c'}
#DEP =
DEP = ${shell find . -type f -name '*.d' ! -path '*/bin/*'}
OBJ = ${SRC:.c=.o}


else ifeq (${TARGET_BUILD}, unified)

SRC = all.c
OBJ = ${SRC:.c=.o}

all.c:
	find . -type f -name '*.c' ! -path '*/bin/*' > all.c.temp
	sed -e 's/\.\///' all.c.temp > all.c1.temp
	sed -e 's/.*/#include "&"/' all.c1.temp > all.c
	rm *.temp -f


endif




ifeq (${TARGET_OS}, linux)

 CC = clang

 INCS = -I./

 ifeq (${TARGET_LINK}, dynamic)
  LIBS = -lSDL2 -lGLEW -lGLU -lGL -lm
 else ifeq (${TARGET_LINK}, static)
	LIBS = -Wl,-Bstatic /home/santi/main/towershogi/bin/SDL3-3.2.18/build/libSDL3.a -lGLEW -pthread -lm -Wl,-Bdynamic -lGLU -lGL
 endif


 #CFLAGS = -O3 -ffast-math -pipe -march=native
 CFLAGS = ${INCS} -Wall -Wextra -pedantic -Wno-unused-parameter -Wno-unused-function -Wno-unused-variable -Wno-unused-result -Wno-sign-compare -Wno-unsafe-buffer-usage -std=c89 -D_REENTRANT
 #LDFLAGS = -flto=full -v
 LDFLAGS = ${LIBS}

 ifeq (${TARGET_BUILD}, incremental)
  #CFLAGS +=
  CFLAGS += -MMD -MF ${@:.o=.d} -MJ $@.json
  #LDFLAGS +=
  LDFLAGS +=
 endif

 ifeq (${TARGET_RELEASE}, on)
  #CFLAGS += -march=native
  CFLAGS += -O3 -ffast-math -pipe -march=x86-64
  #LDFLAGS +=
  LDFLAGS += -flto=full
 endif

 ifeq (${TARGET_DEBUG}, on)
  #CFLAGS += -DISOLA_DBG
  CFLAGS += -g -fno-omit-frame-pointer -mno-omit-leaf-frame-pointer
  #LDFLAGS +=
  LDFLAGS +=
 endif

 ifeq (${TARGET_SANITIZE}, on)
  #CFLAGS += -Werror
  CFLAGS += -fsanitize=undefined -fsanitize=address -Weverything -std=c89
  #LDFLAGS +=
  LDFLAGS += -fsanitize=undefined -fsanitize=address
 endif


else ifeq (${TARGET_OS}, windows)

 CC = x86_64-w64-mingw32-gcc

 INCS = -I./ -I./bin/glew-2.2.0/include -I./bin/SDL2-2.30.3/x86_64-w64-mingw32/include
 LIBS = -L./bin/glew-2.2.0/lib/Release/x64 -L./bin/SDL2-2.30.3/x86_64-w64-mingw32/lib -Wl,-Bstatic -static-libgcc -lmingw32 -lSDL2main -lSDL2 -lglew32s -lglu32 -lopengl32 -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -lsetupapi -lcfgmgr32 -luuid


 #CFLAGS =
 CFLAGS = ${INCS} -D_REENTRANT -DWIN32_LEAN_AND_MEAN -Wall -Wextra -Wpedantic -std=c89 -Wno-unused-parameter -Wno-unused-function -Wno-unused-variable -Wno-unused-result -Wno-sign-compare -Wno-old-style-declaration
 #LDFLAGS = -v -mwindows
 LDFLAGS = ${LIBS}

 ifeq (${TARGET_RELEASE}, on)
  #CFLAGS +=
  CFLAGS += -O3 -ffast-math -pipe -DGLEW_STATIC
  #LDFLAGS +=
  LDFLAGS +=
 endif

 ifeq (${TARGET_DEBUG}, on)
  #CFLAGS += -DISOLA_DBG
  CFLAGS += -g -fno-omit-frame-pointer
  #LDFLAGS +=
  LDFLAGS +=
 endif

 ifeq (${TARGET_SANITIZE}, on)
  #CFLAGS +=
  CFLAGS += -Weverything -Werror -std=c89
  #LDFLAGS +=
  LDFLAGS +=
 endif


endif




include ${DEP}

${OBJ}: ${GLOBALDEP} | ${PRERULE}

${OBJ}: %.o : %.c
	${CC} -c $< -o $@ ${CFLAGS}




ifeq (${TARGET_OS}, linux)

bin:
	mkdir bin


else ifeq (${TARGET_OS}, windows)

bin:
	mkdir bin
	wget https://github.com/libsdl-org/SDL/releases/download/release-2.30.3/SDL2-devel-2.30.3-mingw.zip -P bin/
	wget https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0-win32.zip -P bin/
	unzip bin/SDL2* -d bin/
	unzip bin/glew* -d bin/
	rm bin/*.zip -f


endif




${TARGET_BIN}: ${OBJ} | ${POSTRULE}
	${CC} -o $@.out ${OBJ} ${LDFLAGS}


clean:
	rm ${OBJ} -f

deepclean:
	rm perf.* -f
	rm ${TARGET_BIN}.out ${TARGET_BIN}.exe a.out -f
	rm ${shell find . -type f -name '*.o' ! -path '*/bin/*'} -f
	rm ${shell find . -type f -name '*.d' ! -path '*/bin/*'} -f
	rm ${shell find . -type f -name '*.o.json' ! -path '*/bin/*'} -f

update: deepclean
	rm bin -rf
	rm isola -rf

compdb: ${OBJ}
	sed -e '1s/^/[\n/' -e '$$s/,$$/\n]/' ${shell find . -type f -name "*.o.json" ! -path '*/bin/*'} > compile_commands.json

flamegraph:
	su -c "perf record -F 999 -g --call-graph dwarf ./a.out; perf script | /opt/FlameGraph/stackcollapse-perf.pl > perf.fold; /opt/FlameGraph/flamegraph.pl perf.fold > perf.svg"

test: ${TARGET_BIN}
	./${TARGET_BIN}.out

debug: clean
	make a TARGET_BIN=a TARGET_DEBUG=on clean

sanitize: clean
	make a TARGET_BIN=a TARGET_SANITIZE=on clean

release:
	make clean ${TARGET_BIN} TARGET_OS=linux TARGET_LINK=static TARGET_BUILD=unified TARGET_RELEASE=on

windows:
	make clean ${TARGET_BIN} TARGET_OS=windows TARGET_LINK=static TARGET_BUILD=unified TARGET_RELEASE=on

isola:
	git clone https://github.com/sunorcio/isola --depth 1
	cp isola/isola_config.h isola_config.h -n
	cp isola_config.h isola/isola_config.h
	@echo -e '\033[0;31m''!!! ISOLA HAS BEEN UPDATED, RUN MAKE AGAIN !!!''\033[0m'
	@exit 1

isola/isola_config.h: isola_config.h
	cp isola_config.h isola/isola_config.h




.PHONY: default clean deepclean update compdb flamegraph test debug sanitize release windows
