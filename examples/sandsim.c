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
#define SCALE 5
#define FULLSCREEN FALSE

static const pixel_info sand = {125, 125, 0, 255};

static pixel_info pxAir(const int height, int y) {
    pixel_info air = {100, 100, 130, 255};
    air.b += (uint8_t)(int)(125.0 * ((float)y / (float)height));

    return air;
}

static void pxInit(const int width, const int height) {
    int x, y, index = 0;
    pixel_info* pixbuf = rbxeGetBuffer();

    for (y = 0; y < height; ++y) {
        for (x = 0; x < width; ++x, ++index) {
            pixbuf[index] = y < height / 3 - (rand() % 40) ? sand: pxAir(height, y);
        }
    }
}

static void pxUpdate(pixel_info* buf, const int width, const int height) {
    int x, y, dy, index = 0;
    pixel_info* pixbuf = rbxeGetBuffer();

    for (y = 0; y < height; ++y) {
        for (x = 0; x < width; ++x, ++index) { 
            if (pixbuf[index].r > 124) {
                if (y > 0) {
                    dy = (y - 1) * width;
                    if (pixbuf[dy + x].r < 125) {
                        buf[dy + x] = sand;
                        buf[index] = pxAir(height, y);
                    } else if (x + 1 < width && pixbuf[dy + x + 1].r < 125) {
                        buf[dy + x + 1] = sand;
                        buf[index] = pxAir(height, y);
                    } else if (x > 0 && pixbuf[dy + x - 1].r < 125) {
                        buf[dy + x - 1] = sand;
                        buf[index] = pxAir(height, y);
                    }
                }
            }
            else buf[index] = pxAir(height, y);
        }
    }

    memcpy(pixbuf, buf, width * height * sizeof(pixel_info));
}

int main(void) {
    pixel_info* buf;
    const pixel_info red = {255, 0, 0, 255};
    int mouseX, mouseY, width = 0, height = 0;

    if (!rbxeStart("Sand Simulation", WIDTH, HEIGHT, SCALE, FULLSCREEN)) return EXIT_FAILURE;

    rbxeScreenSize(&width, &height);

    srand(time(NULL));

    buf = (pixel_info*)malloc(width * height * sizeof(pixel_info));
    pxInit(width, height);

    while (rbxeRun()) {
        rbxeMousePos(&mouseX, &mouseY);

        if (rbxeKeyPressed(KEY_ESCAPE)) {
            break;
        }
        
        if (rbxeKeyPressed(KEY_R)) {
            pxInit(width, height);
        }
        
        pxUpdate(buf, width, height);
        
        if (rbxeMouseDown(MOUSE_LEFT) && 
            mouseX >= 0 && mouseX < width && mouseY >= 0 && mouseY < height) {
            rbxeSetPixel(mouseX, mouseY, red);
        }
    }
    
    free(buf);
    return rbxeEnd();
}

