# RBXE - Roger Boesch's Pixel Engine

Simple pixel engine written in C using OpenGL. It's meant
to be extremely simple, lightweight, and easy to use. It
is compatible from C89 to C++20 as a header only solution.


## Hello World
```C

#define RBXE_APPLICATION
#include <rbxe.h>

int main(void) {
    Pixel* pixbuf = rbxeStart("title", 800, 600, 100, 75);

    while (rbxeRun(pixbuf)) {
        if (rbxeKeyPressed(KEY_ESCAPE)) {
            break;
        }
    }

    return rbxeEnd(pixbuf);
}

```


## Header-Only

As a header only solution, you need to define 
```RBXE_APPLICATION``` before including rbxe.h to access the
implementation details. 

```C
#define RBXE_APPLICATION
#include <rbxe.h>
```

You should only define RBXE_APPLICATION in a single
translation unit.


## Plotting

By initializing rbxe you are given a 8-bit RGBA pixel buffer in a row-major order 
where the coordinate (0, 0) corresponds to the top left pixel of the image. At each
frame you render from the CPU side by directly accessing and modifying the values of 
the pixel buffer giving you fast and direct control over every pixel in the screen. 
This is an example on how you can plot a pixel at position (x, y) to red.

```C

Pixel red = {255, 0, 0, 255};
pixbuf[y * width + x] = red;

```

Where ```width``` is the fourth argument given to ```rbxeStart()```. 


## Dependencies

* [GLFW](https://github.com/glfw/glfw)
* [GLEW](https://github.com/nigels-com/glew) (only on Linux and Windows)

### MacOS

To install **GLEW**:

```shell
brew install glew
```

### Linux

To install **GLEW** and **GLFW**

```shell
sudo apt-get install libglew-dev
sudo apt-get install libglfw3-dev
```

## Build Scripts

To build ALL examples, simply use: 

```shell
make all
```

Compile a specific example by including rbxe.h as a header only solution:

```shell
make example/mandelbrot.c
```

The resulting executable will be called *mandelbrot* and is created in the *bin* folder.

To delete the compiled binaries and object files, do:

```shell
make clean
```

## API

```C
typedef struct Pixel {
    unsigned char r, g, b, a;
} Px;
```

Main and only tyepedefed structure used by the engine. The simple pixel 
engine handles only 4 channel RGBA buffers. 

```C
Pixel* rbxeStart(const char* title, int winwidth, int winheight, int scrwidth, int scrheight);
```

Creates a window with title, width and height specified by the first three
arguments. The fourth and fifth arguments represent the width and height of 
the pixel buffer that is allocated and returned to be rendered. The pixel 
buffer is a contiguous array of type Pixel in row-major order. The size in memory
of the returned pixel buffer is exactly the width by the height by the size of
the ```Pixel``` struct.

```C
int rbxeStep(void);
```

This function flushes the previous frame from the screen, swaps the rendering
buffers, polls for inputs, and checks if the rendering window is still opened. It
returns zero if the window was closed, otherwise it returns a non-zero value.
Unless using ```rbxeRun()```, this functions should be called once every frame.

```C
void rbxeRender(const Pixel* pixbuf);
```

Renders the pixel buffer passed as argument to the screen. This function can be
called more than once each frame and you can pass any texture that has the
same resolution and size in memory as the one returned by ```rbxeStart()```.
Otherwise, it results in undefined behaviour. Rendering a pixel with an alpha value
of zero will result in the pixel having the color of the background or what was
already rendered on that pixel. An alpha value of 255 will result in the exact
linear RGB color of the rendered pixel. All values in between are automatically
interpolated.

```C
int rbxeRun(const Pixel* pixbuf);
```

This function is exactly the same as calling ```rbxeStep()``` and
```rbxeRender()```. It clears the screen from the previous frame, renders the
pixel buffer passed as argument and checks for input. It returns a non-zero
value if the rendering window is still opened, otherwise it returns zero. This
function does all what's necesary to run and render with RBXE and it's most useful
as the check to the main rendering loop.

```C
int rbxeEnd(Px* pixbuf);
```

Deallocates the pixel buffer and closes the render window along with OpenGL
libraries.

```C
void rbxeScreenSize(int* width, int* height);
void rbxeWindowSize(int* width, int* height);
```

These functions return the width and height of the initialized screen and
window respectively, inside the first and second pointers passed to the
function. Expects valid integers as arguments.

```C
void rbxeBackgroundColor(Pixel color);
```

Sets the color of the background to the color specified by the passed argument.
It has permanent state, so it does not need to be called every frame, but it
can be changed at any time. The background color is set to black by default.

```C
int rbxeKeyDown(const int key);
int rbxeKeyPressed(const int key);
int rbxeKeyReleased(const int key);
int rbxeMouseDown(const int button);
int rbxeMousePressed(const int button);
int rbxeMouseReleased(const int button);
```

These keyboard and mouse input functions return 0 or 1 depending on the state 
of the key or button passed as argument. The values to be tested are defined by
macros in rbxe.h with the syntax KEY_NAME and MOUSE_NAME where NAME is the
identifier of the key or button. For mouse input, the values are
```MOUSE_LEFT```, ```MOUSE_RIGHT``` and ```MOUSE_MIDDLE```. Some keyboard
examples are ```KEY_ESCAPE```, ```KEY_SPACE```, ```KEY_CONTROL```,
```KEY_LEFT```, ```KEY_RIGHT```, and for character keys ```KEY_W```.
These macro definitions have the same identifiers and values as glfw input codes,
they are essentially shorter and less verbose versions.

```C
char rbxeKeyChar(void);
```

Returns the last character key pressed by the user.

```C
void rbxeMousePos(int* xptr, int* yptr);
```

Gets the X and Y position of the mouse cursor relative to the pixel buffer
in screen coordinates and puts those values in the pointers passed in as
arguments. It will return negative or larger than the screen 
numbers if the mouse is outside of the screen. Returns valid pixel positions 
if the mouse is inside the pixel buffer and the render window. Expects valid
integers as arguments.

```C
void rbxeMouseVisible(int);
```

Sets the native mouse cursor from your OS to be invisible if you pass a 0, or 
visible if you pass any other value.

```C
double rbxeTime(void);
```

Returns time from the moment RBXE was initialized in seconds.

