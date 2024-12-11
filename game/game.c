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
#define FULLSCREEN TRUE

#define OUTER_ROOM_SIZE 160
#define INNER_ROOM_SIZE OUTER_ROOM_SIZE/3*2

void drawRoom(void);

int main(void) {
    if (!rbxeStart("Game", WIDTH, HEIGHT, SCALE, FULLSCREEN)) return EXIT_FAILURE;

    while (rbxeRun()) {
        if (rbxeKeyPressed(KEY_ESCAPE)) {
            break;
        }

        rbxeClear(0);

        drawRoom();
    }

    return rbxeEnd();
}

void drawRoom(void) {
    int x1,y1,x2,y2;
    int space=(OUTER_ROOM_SIZE-INNER_ROOM_SIZE)/2;
    int width, height;
    rbxeScreenSize(&width, &height);

    /* Outer rectancle */
    x1 = 10;
    y1 = (height - OUTER_ROOM_SIZE) / 2;
    rbxePlotLine(x1, y1, x1 + OUTER_ROOM_SIZE, y1, gameGetPalette(PAL_COLOR_MAGENTA));
    rbxePlotLine(x1, y1+OUTER_ROOM_SIZE, x1 + OUTER_ROOM_SIZE, y1+OUTER_ROOM_SIZE, gameGetPalette(PAL_COLOR_MAGENTA));
    rbxePlotLine(x1, y1, x1, y1+OUTER_ROOM_SIZE, gameGetPalette(PAL_COLOR_MAGENTA));
    rbxePlotLine(x1+OUTER_ROOM_SIZE, y1, x1+OUTER_ROOM_SIZE, y1+OUTER_ROOM_SIZE, gameGetPalette(PAL_COLOR_MAGENTA));

    /* Inner rectancle */
    x2 = x1 + space; 
    y2 = y1 + space; 
    rbxePlotLine(x2, y2, x2 + INNER_ROOM_SIZE, y2, gameGetPalette(PAL_COLOR_MAGENTA));
    rbxePlotLine(x2, y2+INNER_ROOM_SIZE, x2 + INNER_ROOM_SIZE, y2+INNER_ROOM_SIZE, gameGetPalette(PAL_COLOR_MAGENTA));
    rbxePlotLine(x2, y2, x2, y2+INNER_ROOM_SIZE, gameGetPalette(PAL_COLOR_MAGENTA));
    rbxePlotLine(x2+INNER_ROOM_SIZE, y2, x2+INNER_ROOM_SIZE, y2+INNER_ROOM_SIZE, gameGetPalette(PAL_COLOR_MAGENTA));

    /* Connection */
   rbxePlotLine(x1, y1, x2, y2, gameGetPalette(PAL_COLOR_MAGENTA));
   rbxePlotLine(x1, y1+OUTER_ROOM_SIZE, x2, y2+INNER_ROOM_SIZE, gameGetPalette(PAL_COLOR_MAGENTA));
   rbxePlotLine(x1+OUTER_ROOM_SIZE, y1+OUTER_ROOM_SIZE, x2+INNER_ROOM_SIZE, y2+INNER_ROOM_SIZE, gameGetPalette(PAL_COLOR_MAGENTA));
   rbxePlotLine(x1+OUTER_ROOM_SIZE, y1, x2+INNER_ROOM_SIZE, y2, gameGetPalette(PAL_COLOR_MAGENTA));
}
