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

#ifndef RB_GAME_ROOMS_H
#define RB_GAME_ROOMS_H

#include <rbxe.h>

int gameRoomInitialize(void);
void gameRoomDrawStandard(int x, int y, int width, int height, int thickness, Pixel color);

#ifdef GAME_ROOMS

#define ROOM_SIZE 200

#define LEVEL_ATIC          1
#define LEVEL_FIRST_FLOOR   2
#define LEVEL_GROUND        3
#define LEVEL_BASEMENT      4
#define LEVEL_CAVERNS       5

#define ROOM_SCALE_FULL     1
#define ROOM_SCALE_H        2
#define ROOM_SCALE_V        3

#define NUMBERS_PER_LINE    8

typedef struct roomInfo {
    int id;                                 /* Room ID */
    int layer;                              /* Atic, First Floor, Ground, Basement, Caverns */
    int color;                              /* Palette color */
    int scale;                              /* Room scale */
    int idleft, idright, idtop, idbottom;   /* IDs to next rooms */
    /* ... More to come */
} roomInfo;

void _gameRoomAdd(roomInfo* room) {
    printf("Add room %d\n", room->id);
}

int _gameRoomParse(char* str) {
    char *comma, *position;
    char number[128];
    int numbers[NUMBERS_PER_LINE];
    int index=0, i=0;
    roomInfo room;

    comma = strchr(str, ',');
    position = str;
    
    while (comma) {
        i = 0;

        while (position < comma && i <= 128) {
            number[i] = *position;
            i++;
            position++;
        }

        // Add a NULL to the end of the string
        number[i] = '\0';

        // We need one last number at the end of the function
        if (index+1 >= NUMBERS_PER_LINE) {
            fprintf(stderr, "Line has too much infos. Maximum number of lines per line is %d\n", NUMBERS_PER_LINE);
            return FALSE;
        }

        numbers[index] = atoi(number);
        index++;

        // Position is now the comma, skip it past
        position++;
        comma = strchr(position, ',');
    }

    // Now there's no more commas in the string so the final value is simply the rest of the string
    numbers[index] = atoi(position);

    /* Process loaded values if index is correct */
    if (index+1 == NUMBERS_PER_LINE) {
        room.id       = numbers[0];
        room.layer    = numbers[1];
        room.color    = numbers[2];
        room.scale    = numbers[3];
        room.idleft   = numbers[4];
        room.idright  = numbers[5];
        room.idtop    = numbers[6];
        room.idbottom = numbers[7];

        _gameRoomAdd(&room);

        return TRUE;
    }

    return FALSE;
}

int gameRoomInitialize(void) {
    char str[256];
    int line = 0;

    // Load rooms file
    int result;
    FILE* f = fopen("rooms.txt", "r");

    if (!f) {
        fprintf(stderr, "Can't load rooms file\n");
        return FALSE;
    }

    do {
        result = fscanf(f, "%127[^;\n]", str);

        if (result == 0) {
            result = fscanf(f, "%*c");
        }
        else {
            line++;

            /* Remove lines starting with comment token '#' */
            if (strlen(str) > 0 && str[0] != '#') {
                if (!_gameRoomParse(str)) {
                    fprintf(stderr, "Error parsing rooms file at line %d > '%s'\n", line, str);

                    return FALSE;
                }
            }
        }

    } while(result != EOF);

    fprintf(stdout, "Rooms file loaded\n");

    return TRUE;
}

void gameRoomDrawStandard(int x, int y, int width, int height, int thickness, Pixel color) {
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

#endif /* GAME_ROOMS */
#endif /* RB_GAME_ROOMS_H */
