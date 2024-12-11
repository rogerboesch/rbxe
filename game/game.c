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

#define GAME_PALETTE
#include "game-palette-zx.h"

#define WIDTH 800
#define HEIGHT 600
#define SCALE 2
#define FULLSCREEN FALSE

#define ROOM_SIZE 200

void drawStandardRoom(int x, int y, int width, int height, int thickness, Pixel color);

int main(void) {
    int swidth, sheight;

    if (!rbxeStart("Game", WIDTH, HEIGHT, SCALE, FULLSCREEN)) return EXIT_FAILURE;

    rbxeScreenSize(&swidth, &sheight);

    while (rbxeRun()) {
        if (rbxeKeyPressed(KEY_ESCAPE)) {
            break;
        }

        rbxeClear(0);

        drawStandardRoom(swidth/2, sheight/2, ROOM_SIZE, ROOM_SIZE, 40, gameGetPalette(PAL_COLOR_CYAN));
    }

    return rbxeEnd();
}

void drawStandardRoom(int x, int y, int width, int height, int thickness, Pixel color) {
    int x1,y1,x2,y2;
    int swidth, sheight;
    int iwidth, iheight;

    rbxeScreenSize(&swidth, &sheight);

    /* Outer rectancle */
    x1 = x-width/2;
    y1 = y-height/2;
    rbxePlotLine(x1, y1, x1 + width, y1, color);
    rbxePlotLine(x1, y1+height, x1 + width, y1+height, color);
    rbxePlotLine(x1, y1, x1, y1+height, color);
    rbxePlotLine(x1+width, y1, x1+width, y1+height, color);

    /* Inner rectancle */
    x2 = x1 + thickness; 
    y2 = y1 + thickness; 
    iwidth = width - 2*thickness;
    iheight = height - 2*thickness;
    
    rbxePlotLine(x2, y2, x2 + iwidth, y2, color);
    rbxePlotLine(x2, y2+iheight, x2 + iwidth, y2+iheight, color);
    rbxePlotLine(x2, y2, x2, y2+iheight, color);
    rbxePlotLine(x2+iwidth, y2, x2+iwidth, y2+iheight, color);

    /* Connection */
   rbxePlotLine(x1, y1, x2, y2, color);
   rbxePlotLine(x1, y1+height, x2, y2+iheight, color);
   rbxePlotLine(x1+width, y1+height, x2+iwidth, y2+iheight, color);
   rbxePlotLine(x1+width, y1, x2+iwidth, y2, color);
}
