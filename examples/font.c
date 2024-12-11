/*
 * RBXE | The Pixel Engine by Roger Boesch
 *
 * Copyright (C) 2024 Roger Boesch
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#define RBXE_ENGINE
#include <rbxe.h>
#include <rbxe-font.h>

#include <stdio.h>
#include <string.h>

/* This program opens up a 800 x 600 window in a 2:1 pixel scale.
 * Each frame it clears the pixel buffer to white, then draws a 
 * 'Hello World' string where the mouse is  */

#define WIDTH 800
#define HEIGHT 600
#define SCALE 2
#define FULLSCREEN TRUE

int main(void) {
    int mouseX, mouseY;
    const int width = WIDTH * SCALE, height = HEIGHT * SCALE;
    const Pixel black = {0, 0, 0, 255}, white = {255, 255, 255, 255};
    char str[255];

    if (!rbxeStart("Font", WIDTH, HEIGHT, SCALE, FULLSCREEN)) return EXIT_FAILURE;

    rbxeFontInit();

    while (rbxeRun()) {
        rbxeMousePos(&mouseX, &mouseY);

        if (rbxeKeyPressed(KEY_ESCAPE)) {
            break;
        }

        rbxeClear(255);
 
        if (mouseX >= 0 && mouseX < width && mouseY >= 0 && mouseY < height) {
            sprintf(str, "%d,%d", mouseX, mouseY);
            rbxeFontDrawString(10, 10, str, black, white);
        }
    }

    return rbxeEnd();
}

