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

#define WIDTH 800
#define HEIGHT 600
#define SCALE 2
#define FULLSCREEN FALSE
#define SPEED 50

int main(void) {
    int s_width, s_height;
    sprite_info* sprite;

    if (!rbxeStart("Game", WIDTH, HEIGHT, SCALE, FULLSCREEN)) return EXIT_FAILURE;

    rbxeScreenSize(&s_width, &s_height);

    /* Create rooms */
    if (!gameRoomInitialize()) return EXIT_FAILURE;

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

        rbxeClear(0);

        gameRoomDraw(1);

        rbxeSpriteUpdate(sprite);
        rbxeSpriteRender(sprite);
    }

    return rbxeEnd();
}
