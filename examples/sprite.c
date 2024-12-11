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

#define WIDTH 800
#define HEIGHT 600
#define SCALE 2
#define FULLSCREEN FALSE
#define NUMBER_OF_SPRITES 5

int main(void) {
   int mouseX, mouseY;
    const Pixel black = {0, 0, 0, 255}, white = {255, 255, 255, 255};
    char str[255];
    Sprite* sprite;
    int i=0;

    if (!rbxeStart("Sprite", WIDTH, HEIGHT, SCALE, FULLSCREEN)) return EXIT_FAILURE;

    rbxeFontInit();

    sprite = rbxeSpriteLoad("player.png", 24, 24);
    if (!sprite) return EXIT_FAILURE;

    rbxeSpriteSetPosition(sprite, 0, 100);
    rbxeSpriteSetVelocity(sprite, 1, 0);

    while (rbxeRun()) {
        if (rbxeKeyPressed(KEY_ESCAPE)) {
            break;
        }

        rbxeClear(0);

        rbxeMousePos(&mouseX, &mouseY);
 
        if (mouseX >= 0 && mouseX < WIDTH && mouseY >= 0 && mouseY < HEIGHT) {
            sprintf(str, "%d,%d", mouseX, mouseY);
            rbxeFontDrawString(10, 10, str, white, black);
        }

        rbxeSpriteUpdate(sprite);
        rbxeSpriteRender(sprite);
    }

    return rbxeEnd();
}

