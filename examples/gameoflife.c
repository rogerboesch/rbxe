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

#include <stdlib.h>
#include <string.h>
#include <time.h>

#define WIDTH 800
#define HEIGHT 600
#define SCALE 2
#define FULLSCREEN TRUE

#define pxAt(px, w, x, y) (px[(((y) * (w)) + (x))].r)

static const pixel_info black = {0, 0, 0, 255};
static const pixel_info white = {255, 255, 255, 255};

static void pxInit(const size_t size) {
    pixel_info* pixbuf = rbxeGetBuffer();
    size_t i;

    for (i = 0; i < size; ++i) {
        memcpy(pixbuf + i, rand() % 2 ? &white : &black, sizeof(pixel_info));
    }
}

static void pxUpdate(pixel_info* buf, const int width, const int height) {
    int x, y, count, index = 0;
    pixel_info* pixbuf = rbxeGetBuffer();

    for (y = 0; y < height; ++y) {
        for (x = 0; x < width; ++x, ++index) {
            count = 0;
            count += (x + 1 < width) && pxAt(pixbuf, width, x + 1, y);
            count += (x + 1 < width) && (y > 0) && pxAt(pixbuf, width, x + 1, y - 1);
            count += (y > 0) && pxAt(pixbuf, width, x, y - 1);
            count += (x > 0) && (y > 0) && pxAt(pixbuf, width, x - 1, y - 1);
            count += (x > 0) && pxAt(pixbuf, width, x - 1, y);
            count += (x > 0) && (y + 1 < height) && pxAt(pixbuf, width, x - 1, y + 1);
            count += (y + 1 < height) && pxAt(pixbuf, width, x, y + 1);
            count += (x + 1 < width) && (y + 1 < height) && pxAt(pixbuf, width, x + 1, y + 1);

            buf[index] = pixbuf[index].r ? (count == 2 || count == 3 ? white : black) : (count == 3 ? white : black);
        }
    }

    memcpy(pixbuf, buf, width * height * sizeof(pixel_info));
}

int main(void) {
    pixel_info* buf;
    const pixel_info red = {255, 0, 0, 255};
    int mouseX, mouseY;
    const int width = WIDTH * SCALE, height = HEIGHT * SCALE;

    if (!rbxeStart("Game Of Life", WIDTH, HEIGHT, SCALE, FULLSCREEN)) return EXIT_FAILURE;

    srand(time(NULL));

    buf = (pixel_info*)malloc(width * height * sizeof(pixel_info));
    pxInit(width * height);

    while (rbxeRun()) {
        rbxeMousePos(&mouseX, &mouseY);

        if (rbxeKeyPressed(KEY_ESCAPE) || rbxeKeyPressed(KEY_Q)) {
            break;
        }

        if (rbxeKeyPressed(KEY_R)) {
            pxInit(WIDTH / SCALE * HEIGHT / SCALE);
        }
        
        pxUpdate(buf, WIDTH/SCALE, HEIGHT/SCALE);
        
        if (mouseX >= 0 && mouseX < width && mouseY >= 0 && mouseY < height) {
            rbxeSetPixel(mouseX, mouseY, red);
        }
    }
    
    free(buf);
    return rbxeEnd();
}

