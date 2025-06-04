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
#include <rbxe-gif.h>

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define WIDTH               640
#define HEIGHT              400
#define SCREEN_WIDTH        320
#define SCREEN_HEIGHT       200
#define SCALE               2
#define FULLSCREEN          FALSE
 
#define MAP_N               1024
#define SCALE_FACTOR        70.0
#define LIGHT               2

uint8_t* heightmap = NULL;   // buffer/array to hold height values (1024*1024)
uint8_t* colormap  = NULL;   // buffer/array to hold color values  (1024*1024)

uint8_t palette[256 * 3];    // palette (RGB)
int palsize;
int night = FALSE;

// Camera struct type declaration
typedef struct {
    float x;         // x position on the map
    float y;         // y position on the map
    float height;    // height of the camera
    float horizon;   // offset of the horizon position (looking up-down)
    float zfar;      // distance of the camera looking forward
    float angle;     // camera angle (radians, clockwise)
} camera_t;

// Camera initialization
camera_t camera = {
    .x       = 512.0,
    .y       = 512.0,
    .height  = 70.0,
    .horizon = 60.0,
    .zfar    = 600.0,
    .angle   = 1.5 * 3.141592 // (= 270 deg)
};

void handle_input(void) {
    camera.angle -= 0.01;
    camera.x += cos(camera.angle);
    camera.y += sin(camera.angle);

    if (rbxeKeyDown(KEY_UP)) {
        camera.x += cos(camera.angle);
        camera.y += sin(camera.angle);
    }

    if (rbxeKeyDown(KEY_DOWN)) {
        camera.x -= cos(camera.angle);
        camera.y -= sin(camera.angle);
    }

    if (rbxeKeyDown(KEY_LEFT)) {
        camera.angle -= 0.01;
    }

    if (rbxeKeyDown(KEY_RIGHT)) {
        camera.angle += 0.01;
    }

    if (rbxeKeyDown(KEY_E)) {
        camera.height++;
    }

    if (rbxeKeyDown(KEY_D)) {
        camera.height--;
    }

    if (rbxeKeyDown(KEY_S)) {
        camera.horizon += 1.5;
    }

    if (rbxeKeyDown(KEY_W)) {
        camera.horizon -= 1.5;
    }

    if (rbxeKeyPressed(KEY_N)) {
        night = !night;
    }
}

void render_voxel(void) {
    pixel_info color = {0, 0, 0, 255};
    pixel_info* pixbuf = rbxeGetBuffer();

    float sinangle = sin(camera.angle);
    float cosangle = cos(camera.angle);

    // left-most point of the FOV
    float plx = cosangle * camera.zfar + sinangle * camera.zfar;
    float ply = sinangle * camera.zfar - cosangle * camera.zfar;

    // right-most point of the FOV
    float prx = cosangle * camera.zfar - sinangle * camera.zfar;
    float pry = sinangle * camera.zfar + cosangle * camera.zfar;

    // Loop rays from left to right
    for (int i = 0; i < SCREEN_WIDTH; i++) {
        float deltax = (plx + (prx - plx) / SCREEN_WIDTH * i) / camera.zfar;
        float deltay = (ply + (pry - ply) / SCREEN_WIDTH * i) / camera.zfar;

        // ray (x,y) coords
        float rx = camera.x;
        float ry = camera.y;

        // store the tallest projected height per-ray
        float tallestheight = SCREEN_HEIGHT;

        // loop all depth units until the zfar distance limit
        for (int z = 1; z < camera.zfar; z++) {
            rx += deltax;
            ry += deltay;

            // find the offset that we have to go and fetch values from the heightmap
            int mapoffset = ((MAP_N * ((int)(ry) & (MAP_N - 1))) + ((int)(rx) & (MAP_N - 1)));

            // project height values and find the height on-screen
            int projheight = (int)((camera.height - heightmap[mapoffset]) / z * SCALE_FACTOR + camera.horizon);

            // only draw pixels if the new projected height is taller than the previous tallest height
            if (projheight < tallestheight) {
                // draw pixels from previous max-height until the new projected height
                for (int y = projheight; y < tallestheight; y++) {
                    if (y >= 0) {
                        int pal = colormap[mapoffset];
                        
                        color.r = palette[pal * 3 + 0];
                        color.g = palette[pal * 3 + 1];
                        color.b = palette[pal * 3 + 2];
                        color.a = 255;
                        
                        color.r = (color.r & 63) << 2;
                        color.g = (color.g & 63) << 2;
                        color.b = (color.b & 63) << 2;
 
                        if (night) {
                            int mix = (color.r + color.g + color.b) / 3;
                            color.r = 0;
                            color.g = mix;
                            color.b = 0;
                        }

                        pixbuf[(SCREEN_WIDTH * (SCREEN_HEIGHT-y)) + i] = color; 
                    }
                }

                tallestheight = projheight;
            }
        }
    }
}

void render_hud(void) {
    int mid_width = SCREEN_WIDTH / 2;
    int mid_height = SCREEN_HEIGHT / 2;

    pixel_info green = {0, 255, 0, 255};

    rbxeSetPixel(mid_width, mid_height, green);
    rbxePlotLine(mid_width,      mid_height - 10, mid_width,      mid_height - 40, green);
    rbxePlotLine(mid_width,      mid_height + 10, mid_width,      mid_height + 40, green);
    rbxePlotLine(mid_width - 10, mid_height,      mid_width - 40, mid_height,      green);
    rbxePlotLine(mid_width + 10, mid_height,      mid_width + 40, mid_height,      green);
}

int main(void) {
    int width, height;

    colormap = rbxeLoadGIF("C00.gif", NULL, NULL, &palsize, palette);
    heightmap = rbxeLoadGIF("D00.gif", NULL, NULL, NULL, NULL);

    if (colormap == NULL || heightmap == NULL) {
        fprintf(stderr, "ERROR: Can't load colormap or heightmap file\n");
        return FALSE;
    }

    if (!rbxeStart("Voxel", WIDTH, HEIGHT, SCALE, FULLSCREEN)) return EXIT_FAILURE;
 
    rbxeScreenSize(&width, &height);
 
    srand(time(NULL));
 
    while (rbxeRun()) {
        rbxeClear(0);

        handle_input();
        render_voxel();
        
        if (night) {
            render_hud();
        }

        if (rbxeKeyPressed(KEY_ESCAPE)) {
            break;
        }
    }
     
    return rbxeEnd();
}
