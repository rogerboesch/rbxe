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
#include <math.h>

#define ABS(n) ((n) > 0 ? (n) : -(n))

#define WIDTH 800
#define HEIGHT 600
#define SCALE 4
#define FULLSCREEN FALSE

typedef struct vec2 {
    float x, y;
} vec2;

typedef struct ivec2 {
    int x, y;
} ivec2;

static vec2 vec2_create(float x, float y) {
    vec2 p;
    p.x = x;
    p.y = y;

    return p;
}

static ivec2 ivec2_create(int x, int y) {
    ivec2 p;
    p.x = x;
    p.y = y;

    return p;
}

int main(void) {
    const Pixel red = {255, 0, 0, 255}, green = {0, 255, 0, 255};
    vec2 xy, dif, cross;
    ivec2 p, center, idif;
    float invDist, dT, T, t, v = 0.0f;
    int width=0, height=0;

    if (!rbxeStart("Pendulum", WIDTH, HEIGHT, SCALE, FULLSCREEN)) return EXIT_FAILURE;

    rbxeScreenSize(&width, &height);

    center = ivec2_create(width / 2, height / 2);
    p = ivec2_create(center.x + width / 8, center.y);
    xy = vec2_create((float)p.x, (float)p.y);

    idif = ivec2_create(p.x - center.x, p.y - center.y);
    invDist = 1.0F / sqrt(idif.x * idif.x + idif.y * idif.y);
    t = rbxeTime();


    while (rbxeRun()) {
        T = rbxeTime();
        dT = (T - t);
        t = T;

        if (rbxeKeyPressed(KEY_ESCAPE)) {
            break;
        }

        dif = vec2_create((float)(p.x - center.x), (float)(p.y - center.y));
        cross = vec2_create(dif.y * invDist, dif.x * invDist);
        
        v -= cross.y;
        xy.x -= cross.x * v * dT;
        xy.y += cross.y * v * dT;
        p.x = (int)xy.x;
        p.y = (int)xy.y;

        rbxeClear(155);

        if (p.x >= 0 && p.y >= 0 && p.x < width && p.y < height) {
            ivec2 d = ivec2_create((int)(xy.x - cross.x * v * 0.25F), (int)(xy.y + cross.y * v * 0.25F));

            rbxePlotLine(center.x, center.y, p.x, p.y, red);
            rbxePlotLine(p.x, p.y, d.x, d.y, green);
        }
    }

    return rbxeEnd();
}

