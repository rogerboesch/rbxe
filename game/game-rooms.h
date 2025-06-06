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

#include "game-palette-zx.h"

#include <rbxe.h>
#include <rbxe-arraylist.h>

typedef struct roomInfo {
    int id;                                 /* Room ID */
    int index;                              /* Index in list */
    int layer;                              /* Atic, First Floor, Ground, Basement, Caverns */
    int type;                               /* Standard, Edge, Cavern, Atic */
    int color;                              /* Palette color */
    int scale;                              /* Room scale */
    int idleft, idright, idtop, idbottom;   /* IDs to next rooms */
    int idextra;                            /* Extra Id for rooms with 5 doors */
    /* ... More to come */
} roomInfo;

int gameRoomInitialize(void);
roomInfo* gameRoomSelect(int id);
void gameRoomDraw(void);
void gameRoomDump(roomInfo room);

#ifdef GAME_ROOMS

#define ROOM_SIZE               200             /* Size of a default size room */
#define ROOM_SPACE              40              /* Space between inner and ourter room lines */

#define LEVEL_ATIC              1
#define LEVEL_FIRST_FLOOR       2
#define LEVEL_GROUND            3
#define LEVEL_BASEMENT          4
#define LEVEL_CAVERNS           5

#define ROOM_SCALE_FULL         1
#define ROOM_SCALE_HORIZ        2
#define ROOM_SCALE_VERT         3

#define ROOM_TYPE_STD           1           /* Standard box room */
#define ROOM_TYPE_EDGE          2           /* Room width edges */
#define ROOM_TYPE_CAVERN        3           /* Cavern style */
#define ROOM_TYPE_ATIC          4           /* Atic style */
#define ROOM_TYPE_ROOF          5           /* Roof style (205,210) */
#define ROOM_TYPE_DRTWO_UP      6           /* Two doors, has two doors on top (33) */
#define ROOM_TYPE_DRFOUR_UPDWN  7           /* Two doors up, two doors down (311) */
#define ROOM_TYPE_DRFIVE_DWN    8           /* Five doors, 2 on bottom (308,309) */
#define ROOM_TYPE_DRFIVE_UP     9           /* Five doors, 2 onm top (313,314) */

#define ROOM_EXTRA_START        900         /* Marks start/end room */

#define NUMBERS_PER_LINE        10

/* Used to store start index of each floor level */
int startIndex100=0, startIndex200=0, startIndex300=0, startIndex400=0;
int selected_room=0, selected_index=0;
int start_room=0;

/* Sprites used for rooms */
sprite_info* sprite_doors = NULL;
array_list *room_list = NULL;

void _gameRoomAdd(roomInfo* room) {
    /* Room id's are not continuing, must therefore save index */
    switch (room->id) {
        case 100:
            startIndex100 = room_list->len;
            printf("Add room %d (start=%d)\n", room->id, startIndex100);
            break;
        case 200:
            startIndex200 = room_list->len;
            printf("Add room %d (start=%d)\n", room->id, startIndex200);
            break;
        case 300:
            startIndex300 = room_list->len;
            printf("Add room %d (start=%d)\n", room->id, startIndex300);
            break;
        case 400:
            startIndex400 = room_list->len;
            printf("Add room %d (start=%d)\n", room->id, startIndex400);
            break;
        default:
            printf("Add room %d (index=%d)\n", room->id, room_list->len);
            break;
    }

    if (room->idextra == ROOM_EXTRA_START) {
        start_room = room->id;
        printf("> Room %d is the start room\n", room->id);
    }

    room->index = room_list->len;
    array_list_append(room_list, room);
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
        room.type     = numbers[2];
        room.color    = numbers[3];
        room.scale    = numbers[4];
        room.idleft   = numbers[5];
        room.idright  = numbers[6];
        room.idtop    = numbers[7];
        room.idbottom = numbers[8];
        room.idextra = numbers[9];

        _gameRoomAdd(&room);

        return TRUE;
    }

    return FALSE;
}

int _gameRoomLoadSprites(void) {
    sprite_doors = rbxeSpriteLoad("assets/door.png", 48, 36);
    if (!sprite_doors) return FALSE;

    return TRUE;
}

int gameRoomInitialize(void) {
    char str[256];
    int line = 0;

    /* Create list */
    room_list = array_list_create(sizeof(roomInfo), 200);        

    /* Load rooms file */
    int result;
    FILE* f = fopen("assets/rooms.txt", "r");

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

    fprintf(stdout, "Rooms file loaded %d rooms added\n", room_list->len);

    gameRoomSelect(start_room);
    
    return _gameRoomLoadSprites();
}

void gameRoomDump(roomInfo room) {
    fprintf(stdout, "Room %d (%d)\n", room.id, room.index);
    fprintf(stdout, "+-layer: %d\n", room.layer);
    fprintf(stdout, "+-type: %d\n", room.type);
    fprintf(stdout, "+-color: %d\n", room.layer);
    fprintf(stdout, "+-scale: %d\n", room.layer);
    fprintf(stdout, "+-doors: [%d,%d,%d,%d,(%d)]\n", room.idleft, room.idright, room.idtop, room.idbottom, room.idextra);
}

void _gameRoomDrawStandard(int x, int y, int width, int height, int thickness, roomInfo *room) {
    int x1,y1,x2,y2;
    int s_width, s_height;      /* Screen */
    int i_width, i_height;      /* Inner rect */
    pixel_info color;

    rbxeScreenSize(&s_width, &s_height);
    color = gameGetPalette(room->color);

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
    i_width = width - 2*thickness;
    i_height = height - 2*thickness;
    
    rbxePlotLine(x2, y2, x2 + i_width, y2, color);
    rbxePlotLine(x2, y2+i_height, x2 + i_width, y2+i_height, color);
    rbxePlotLine(x2, y2, x2, y2+i_height, color);
    rbxePlotLine(x2+i_width, y2, x2+i_width, y2+i_height, color);

    /* Connection */
    rbxePlotLine(x1, y1, x2, y2, color);
    rbxePlotLine(x1, y1+height, x2, y2+i_height, color);
    rbxePlotLine(x1+width, y1+height, x2+i_width, y2+i_height, color);
    rbxePlotLine(x1+width, y1, x2+i_width, y2, color);

    /* Draw doors */
    if (room->idtop != 0)    rbxeSpriteRenderEx(sprite_doors, s_width/2, y2+i_height+18-1, 0, 0, 48, 36);        /* top */
    if (room->idbottom != 0) rbxeSpriteRenderEx(sprite_doors, s_width/2, y2-18, 48, 0, 48, 36);                  /* bottom */
    if (room->idleft != 0)   rbxeSpriteRenderEx(sprite_doors, x2-18+1, y2+i_height/2, 132, 0, 36, 48);           /* left */
    if (room->idright != 0)  rbxeSpriteRenderEx(sprite_doors, x2+i_width+18-1, y2+i_height/2, 96, 0, 36, 48);    /* right */
}

void gameRoomDraw(void) {
    roomInfo* room;
    int s_width, s_height;
    int width, height, thickness;

    if (room_list == NULL || selected_index < 0 || selected_index >= room_list->len) {
        fprintf(stderr, "No room with id %d available (index=%d)\n", selected_room, selected_index);
        return;
    }

    rbxeScreenSize(&s_width, &s_height);

    room = array_list_get(room_list, selected_index);
 
    width = ROOM_SIZE;
    height = ROOM_SIZE;
    thickness = ROOM_SPACE;

    if (room->scale == ROOM_SCALE_HORIZ) {
        height /= 2;
        thickness /= 2;
    }

    if (room->scale == ROOM_SCALE_VERT) {
        width /= 2;
        thickness /= 2;
    }

    /* TODO: Draw different room types based on room.type */
    _gameRoomDrawStandard(s_width/2, s_height/2, width, height, thickness, room);
}

roomInfo* gameRoomSelect(int room_id) {
    selected_room = room_id;
    selected_index = room_id-1;
    roomInfo* room;

    if (room_id >= 400) {
       selected_index = room_id - 400 + startIndex400; 
    }
    else if (room_id >= 300) {
       selected_index = room_id - 300 + startIndex300; 
    }
    else if (room_id >= 200) {
       selected_index = room_id - 200 + startIndex200; 
    }
    else if (room_id >= 100) {
       selected_index = room_id - 100 + startIndex100; 
    }

    fprintf(stdout, "gameRoomSelect: room with id %d (index=%d)\n", selected_room, selected_index);

    if (room_list == NULL || selected_index < 0 || selected_index >= room_list->len) {
        fprintf(stderr, "No room with id %d available (index=%d)\n", selected_room, selected_index);
        return NULL;
    }

    room = array_list_get(room_list, selected_index);

    gameRoomDump(*room);

    return room;
}

#endif /* GAME_ROOMS */
#endif /* RB_GAME_ROOMS_H */
