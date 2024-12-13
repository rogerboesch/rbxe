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
#include <rbxe-sprite.h>
#include <rbxe-font.h>

#define GAME_PALETTE
#include "game-palette-zx.h"

#define GAME_ROOMS
#include "game-rooms.h"

#define WIDTH       800
#define HEIGHT      600
#define SCALE       2
#define FULLSCREEN  FALSE

#define SPEED               50

#define ROOM_START_ID       18

int main(void) {
    int s_width = 0, s_height = 0;
    sprite_info* sprite = NULL;
    int room_id = ROOM_START_ID;
    roomInfo* room = NULL;

    const pixel_info black = {0, 0, 0, 255}, white = {255, 255, 255, 255};
    char str[255];

    if (!rbxeStart("Game", WIDTH, HEIGHT, SCALE, FULLSCREEN)) return EXIT_FAILURE;

    rbxeScreenSize(&s_width, &s_height);

    rbxeFontInit();

    /* Create rooms */
    if (!gameRoomInitialize()) return EXIT_FAILURE;

    room = gameRoomSelect(room_id);

    /* Create main sprite */
    sprite = rbxeSpriteLoad("player.png", 24, 24);
    if (!sprite) return EXIT_FAILURE;

    rbxeSpriteSetPosition(sprite, s_width/2, s_height/2);

    while (rbxeRun()) {
        if (rbxeKeyPressed(KEY_ESCAPE)) {
            break;
        }

        if (rbxeKeyDown(KEY_LEFT)) {
            rbxeSpriteSetVelocityX(sprite, -SPEED);
        }
        else if (rbxeKeyDown(KEY_RIGHT)) {
            rbxeSpriteSetVelocityX(sprite, SPEED);
        }
        else {
            rbxeSpriteSetVelocityX(sprite, 0);
        }

        if (rbxeKeyDown(KEY_UP)) {
            rbxeSpriteSetVelocityY(sprite, SPEED);
        }
        else if (rbxeKeyDown(KEY_DOWN)) {
            rbxeSpriteSetVelocityY(sprite, -SPEED);
        }
        else {
            rbxeSpriteSetVelocityY(sprite, 0);
        }

        if (rbxeKeyPressed(KEY_Q)) {
            room_id--;
            room = gameRoomSelect(room_id);
        }
        else if (rbxeKeyPressed(KEY_W)) {
            room_id++;
            room = gameRoomSelect(room_id);
        }
        else if (rbxeKeyPressed(KEY_0)) {
            room_id = 1;
            room = gameRoomSelect(room_id);
        }
        else if (rbxeKeyPressed(KEY_1)) {
            room_id = 100;
            room = gameRoomSelect(room_id);
        }
        else if (rbxeKeyPressed(KEY_2)) {
            room_id = 200;
            room = gameRoomSelect(room_id);
        }
        else if (rbxeKeyPressed(KEY_3)) {
            room_id = 300;
            room = gameRoomSelect(room_id);
        }
        else if (rbxeKeyPressed(KEY_4)) {
            room_id = 400;
            room = gameRoomSelect(room_id);
        }

        rbxeClear(0);

        gameRoomDraw();

        if (room != NULL) {
            sprintf(str, "Room: %d", room->id);
            rbxeFontDrawString(s_width-80, s_height-20, str, white, black);

            sprintf(str, "Index: %d", room->index);
            rbxeFontDrawString(s_width-80, s_height-36, str, white, black);
        }

        rbxeSpriteUpdate(sprite);
        rbxeSpriteRender(sprite);
    }

    return rbxeEnd();
}
