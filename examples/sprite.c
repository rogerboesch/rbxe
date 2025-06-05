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
    const pixel_info black = {0, 0, 0, 255}, white = {255, 255, 255, 255};
    char str[255];
    sprite_info* sprite;
    sprite_info* clone[NUMBER_OF_SPRITES];
    int i=0;

    if (!rbxeStart("Sprite", WIDTH, HEIGHT, SCALE, FULLSCREEN)) return EXIT_FAILURE;

    rbxeFontInit();

    /* Create main sprite */
    sprite = rbxeSpriteLoad("assets/player.png", 24, 24);
    if (!sprite) return EXIT_FAILURE;

    rbxeSpriteSetPosition(sprite, 0, 40);
    rbxeSpriteSetVelocity(sprite, 10, 0);

    /* Create sprite clones */
    for (i=0; i<NUMBER_OF_SPRITES; i++) {
        clone[i] = rbxeSpriteClone(sprite);

        rbxeSpriteSetPosition(clone[i], 0, (i+2)*40);
        rbxeSpriteSetVelocity(clone[i], (i+1)*10, 0);
    }

    while (rbxeRun()) {
        if (rbxeKeyPressed(KEY_ESCAPE)) {
            break;
        }

        rbxeClear(0);

        sprintf(str, "Delta: %f", rbxeDeltaTime());
        rbxeFontDrawString(10, 10, str, white, black);

        rbxeSpriteUpdate(sprite);
        rbxeSpriteRender(sprite);

        rbxeFontDrawString((int)sprite->position.x, (int)sprite->position.y+10, "original", white, black);

        for (i=0; i<NUMBER_OF_SPRITES; i++) {
            rbxeSpriteUpdate(clone[i]);
            rbxeSpriteRender(clone[i]);

            rbxeFontDrawString((int)clone[i]->position.x, (int)clone[i]->position.y+10, "clone", white, black);
        }
    }

    return rbxeEnd();
}

