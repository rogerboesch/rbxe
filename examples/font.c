#define RBXE_APPLICATION

#include <rbxe.h>
#include <rbxe-font.h>

#include <stdio.h>
#include <string.h>

/* This program opens up a 800 x 600 window in a 2:1 pixel scale.
 * Each frame it clears the pixel buffer to white, then draws a 
 * 'Hello World' string where the mouse is  */

#define WIDTH 800
#define HEIGHT 600
#define SCALE 1 / 2

int main(void) {
    int mouseX, mouseY;
    const int width = WIDTH * SCALE, height = HEIGHT * SCALE;
    const size_t buflen = width * height * sizeof(Pixel);
    const Pixel black = {0, 0, 0, 255}, white = {255, 255, 255, 255};
    char str[255];

    Pixel* pixbuf = rbxeStart("Font", WIDTH, HEIGHT, width, height);

    if (!pixbuf) {
        fprintf(stderr, "Could not init RBXE\n");
        return 1;
    }

    rbxeFontInit();

    while (rbxeRun(pixbuf)) {
        rbxeMousePos(&mouseX, &mouseY);

        if (rbxeKeyPressed(KEY_ESCAPE)) {
            break;
        }

        memset(pixbuf, 255, buflen);

        if (mouseX >= 0 && mouseX < width && mouseY >= 0 && mouseY < height) {
            sprintf(str, "%d,%d", mouseX, mouseY);
            rbxeFontDrawString(10, 10, str, black, white);
        }
    }

    return rbxeEnd(pixbuf);
}

