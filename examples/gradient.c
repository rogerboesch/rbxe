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

#define WIDTH 800
#define HEIGHT 600
#define SCALE 4
#define FULLSCREEN TRUE

static pixel_info rbxeGradientAt(int x, int y) {
    pixel_info px = {0, 0, 0, 255};
    px.r = x;
    px.b = y;

    return px;
}

static void rbxeDrawGradient(void) {
    int x, y;

    for (y = 0; y < HEIGHT; ++y) {
        for (x = 0; x < WIDTH; ++x) {
            rbxeSetPixel(x, y, rbxeGradientAt(x, y));
        }
    }
}

int main(void) {
    if (!rbxeStart("Gradient", 800, 600, SCALE, FULLSCREEN)) return EXIT_FAILURE;

    rbxeDrawGradient();

    while (rbxeRun()) {
        if(rbxeKeyPressed(KEY_ESCAPE)) {
            break;
        }
    }

    return rbxeEnd();
}

