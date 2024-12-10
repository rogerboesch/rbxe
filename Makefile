# Makefile to create all examples
CC=gcc
STD=-std=c89
OPT=-O2
WFLAGS=-Wall -Wextra -pedantic
LIBS=-lglfw

OS=$(shell uname -s)
ifeq ($(OS),Darwin)
	INCDIR=-I/opt/homebrew/Cellar/glfw/3.4/include -I. -I3rdparty
	LIBDIR=-L/opt/homebrew/Cellar/glfw/3.4/lib
    LIBS+=-framework OpenGL
else
	INCDIR=-I. -I3rdparty
    LIBS+=-lm -lGL -lGLU -lGLEW
endif

CFLAGS=$(STD) $(OPT) $(WFLAGS) $(INCDIR)
LFLAGS=$(STD) $(OPT) $(LIBDIR) $(LIBS)

all: bin/hello bin/example bin/font bin/sandsim bin/mandelbrot bin/gameoflife bin/gradient bin/pendulum bin/sprite

bin/hello : obj/hello.o
	$(CC) obj/hello.o -o bin/hello $(LFLAGS)

obj/hello.o : examples/hello.c rbxe.h | makedir
	$(CC) -c examples/hello.c -o obj/hello.o $(CFLAGS)

bin/example : obj/example.o
	$(CC) obj/example.o -o bin/example $(LFLAGS)

obj/example.o : examples/example.c rbxe.h | makedir
	$(CC) -c examples/example.c -o obj/example.o $(CFLAGS)

bin/font : obj/font.o
	$(CC) obj/font.o -o bin/font $(LFLAGS)

obj/font.o : examples/font.c rbxe-font.h rbxe.h | makedir
	$(CC) -c examples/font.c -o obj/font.o $(CFLAGS)

bin/sandsim : obj/sandsim.o
	$(CC) obj/sandsim.o -o bin/sandsim $(LFLAGS)

obj/sandsim.o : examples/sandsim.c rbxe.h | makedir
	$(CC) -c examples/sandsim.c -o obj/sandsim.o $(CFLAGS)

bin/mandelbrot : obj/mandelbrot.o
	$(CC) obj/mandelbrot.o -o bin/mandelbrot $(LFLAGS)

obj/mandelbrot.o : examples/mandelbrot.c rbxe.h | makedir
	$(CC) -c examples/mandelbrot.c -o obj/mandelbrot.o $(CFLAGS)

bin/gameoflife : obj/gameoflife.o
	$(CC) obj/gameoflife.o -o bin/gameoflife $(LFLAGS)

obj/gameoflife.o : examples/gameoflife.c rbxe.h | makedir
	$(CC) -c examples/gameoflife.c -o obj/gameoflife.o $(CFLAGS)

bin/gradient : obj/gradient.o
	$(CC) obj/gradient.o -o bin/gradient $(LFLAGS)

obj/gradient.o : examples/gradient.c rbxe.h | makedir
	$(CC) -c examples/gradient.c -o obj/gradient.o $(CFLAGS)

bin/pendulum : obj/pendulum.o
	$(CC) obj/pendulum.o -o bin/pendulum $(LFLAGS)

obj/pendulum.o : examples/pendulum.c rbxe.h | makedir
	$(CC) -c examples/pendulum.c -o obj/pendulum.o $(CFLAGS)

bin/sprite : obj/sprite.o
	$(CC) obj/sprite.o -o bin/sprite $(LFLAGS)

obj/sprite.o : examples/sprite.c rbxe.h rbxe-sprite.h | makedir
	$(CC) -c examples/sprite.c -o obj/sprite.o $(CFLAGS)

makedir:
	mkdir -p bin
	mkdir -p obj
	cp assets/* bin/

clean:
	rm -r bin
	rm -r obj