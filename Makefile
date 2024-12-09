# Makefile
CC=gcc
STD=-std=c89
OPT=-O2
WFLAGS=-Wall -Wextra -pedantic
INCDIR=-I/opt/homebrew/Cellar/glfw/3.4/include -I.
LIBDIR=-L/opt/homebrew/Cellar/glfw/3.4/lib
LIBS=-lglfw

OS=$(shell uname -s)
ifeq ($(OS),Darwin)
    LIBS+=-framework OpenGL
else
    LIBS+=-lm -lGL -lGLEW
endif

CFLAGS=$(STD) $(OPT) $(WFLAGS) $(INCDIR)
LFLAGS=$(STD) $(OPT) $(LIBDIR) $(LIBS)

all: bin/hello bin/example bin/sandsim bin/mandelbrot bin/gameoflife bin/gradient bin/pendulum

bin/hello : obj/hello.o
	$(CC) $(LFLAGS) obj/hello.o -o bin/hello

obj/hello.o : examples/hello.c rbxe.h | makedir
	$(CC) $(CFLAGS) -c examples/hello.c -o obj/hello.o

bin/example : obj/example.o
	$(CC) $(LFLAGS) obj/example.o -o bin/example

obj/example.o : examples/example.c rbxe.h | makedir
	$(CC) $(CFLAGS) -c examples/example.c -o obj/example.o

bin/sandsim : obj/sandsim.o
	$(CC) $(LFLAGS) obj/sandsim.o -o bin/sandsim

obj/sandsim.o : examples/sandsim.c rbxe.h | makedir
	$(CC) $(CFLAGS) -c examples/sandsim.c -o obj/sandsim.o

bin/mandelbrot : obj/mandelbrot.o
	$(CC) $(LFLAGS) $(LIBS) obj/mandelbrot.o -o bin/mandelbrot

obj/mandelbrot.o : examples/mandelbrot.c rbxe.h | makedir
	$(CC) $(CFLAGS) -c examples/mandelbrot.c -o obj/mandelbrot.o

bin/gameoflife : obj/gameoflife.o
	$(CC) $(LFLAGS) $(LIBS) obj/gameoflife.o -o bin/gameoflife

obj/gameoflife.o : examples/gameoflife.c rbxe.h | makedir
	$(CC) $(CFLAGS) -c examples/gameoflife.c -o obj/gameoflife.o

bin/gradient : obj/gradient.o
	$(CC) $(LFLAGS) obj/gradient.o -o bin/gradient

obj/gradient.o : examples/gradient.c rbxe.h | makedir
	$(CC) $(CFLAGS) -c examples/gradient.c -o obj/gradient.o

bin/pendulum : obj/pendulum.o
	$(CC) $(LFLAGS) obj/pendulum.o -o bin/pendulum

obj/pendulum.o : examples/pendulum.c rbxe.h | makedir
	$(CC) $(CFLAGS) -c examples/pendulum.c -o obj/pendulum.o

makedir:
	mkdir -p bin
	mkdir -p obj

clean:
	rm -r bin
	rm -r obj