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

#include <stdio.h>
#include <string.h>

/* This program opens up a 800 x 600 window in a 10:1 pixel scale.
 * Each frame it clears the pixel buffer to white, then plots the 
 * center pixel red and the pixel pointed by the mouse blue.
 */

#define WIDTH 800
#define HEIGHT 600
#define SCALE 10
#define FULLSCREEN FALSE

int main(void) {
    int mouseX, mouseY;
    const int width = WIDTH * SCALE, height = HEIGHT * SCALE;
    const int halfWidth = width / 2, halfHeight = height / 2;
    const pixel_info red = {255, 0, 0, 255}, blue = {0, 0, 255, 0};

    if (!rbxeStart("Example", WIDTH, HEIGHT, SCALE, FULLSCREEN)) return EXIT_FAILURE;

    while (rbxeRun()) {
        rbxeMousePos(&mouseX, &mouseY);

        if (rbxeKeyPressed(KEY_ESCAPE)) {
            break;
        }

        rbxeClear(255);
        rbxeSetPixel(halfWidth, halfHeight, red);
        rbxePlotCircle(halfWidth, halfHeight, 20, red);

        if (mouseX >= 0 && mouseX < width && mouseY >= 0 && mouseY < height) {
            rbxeSetPixel(mouseX, mouseY, blue);
        }
    }

    return rbxeEnd();
}

