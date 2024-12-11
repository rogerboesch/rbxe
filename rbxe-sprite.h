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

#ifndef RB_PIXEL_ENGINE_SPRITE_H
#define RB_PIXEL_ENGINE_SPRITE_H

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <rbxe-math.h>

typedef struct Sprite {
	int id;
    int is_clone;
	int is_active;

    /* Transform related */
    vec2 position;
    vec2 velocity;

    /* Sprite atlas information*/
    unsigned char* data;
    int width;
    int height;
    int cell_width;
    int cell_height;

    /* Animation */
	int animation_nr;

    /* Life management */
    int health;
} Sprite;

Sprite* rbxeSpriteLoad(char* filename, int cell_width, int cell_height);
Sprite* rbxeSpriteClone(Sprite* sprite);
void rbxeSpriteRender(Sprite* sprite);
void rbxeSpriteUpdate(Sprite* sprite);
void rbxeSpriteSetPosition(Sprite* sprite, float x, float y);
void rbxeSpriteSetVelocity(Sprite* sprite, float x, float y);
void rbxeSpriteSetActive(Sprite* sprite, int active);
void rbxeSpriteSetHealth(Sprite* sprite, int health);

#ifdef RBXE_ENGINE

static int unique_id = 0;

void rbxeSpriteDump(Sprite* sprite) {
    fprintf(stdout, "Sprite %d (clone: %d)\n", sprite->id, sprite->is_clone);
    fprintf(stdout, "+-position: %0.3f, %0.3f\n", sprite->position.x, sprite->position.y);
    fprintf(stdout, "+-velocity: %0.3f, %0.3f\n", sprite->velocity.x, sprite->velocity.y);
    fprintf(stdout, "+-w/h: %d, %d\n", sprite->width, sprite->height);
    fprintf(stdout, "+-cell: %d, %d\n", sprite->cell_width, sprite->cell_height);
    fprintf(stdout, "+-animation: %d\n", sprite->animation_nr);
}

Sprite* rbxeSpriteLoad(char* path, int cell_width, int cell_height) {
	int width, height, channel_count;
    Sprite *sprite;

    sprite = malloc(sizeof(Sprite));
    sprite->id = ++unique_id;
    sprite->is_clone = FALSE;
    sprite->animation_nr = 0;
    sprite->position = vec2_create(100, 100);
    sprite->is_active = TRUE;
    sprite->health = 0;

	sprite->data = stbi_load(path, &width, &height, &channel_count, 0);
	
    if (!sprite->data) {
        fprintf(stderr, "RBXE: Failed to load sprite image: '%s'\n", path);
        return NULL;
	}
    else {
        fprintf(stdout, "RBXE: Loaded sprite image: '%s' (w:%d, h:%d, bits:%d)\n", path, width, height, channel_count);
    }

	sprite->width = width;
	sprite->height = height;
	sprite->cell_width = cell_width;
	sprite->cell_height = cell_height;

    fprintf(stdout, "RBXE: Sprite %d created\n", sprite->id);

    rbxeSpriteDump(sprite);

    return sprite;
}

Sprite* rbxeSpriteClone(Sprite* sprite) {
    Sprite *clone;

    clone = malloc(sizeof(Sprite));
    clone->id = ++unique_id;
    clone->is_clone = TRUE;
    clone->animation_nr = 0;
    clone->position = vec2_create(100, 100);
    clone->is_active = TRUE;
    clone->health = 0;
	clone->data = sprite->data;

	clone->width = sprite->width;
	clone->height = sprite->height;
	clone->cell_width = sprite->cell_width;
	clone->cell_height = sprite->cell_height;

    fprintf(stdout, "RBXE: Sprite %d created\n", clone->id);

    rbxeSpriteDump(clone);

    return clone;
}

void rbxeSpriteRender(Sprite* sprite) {
    int y, width, height;
    unsigned char* source, *dest;

    rbxeScreenSize(&width, &height);

    /* 
    TODO:
    - Use correct frame (see animation_id)
    - Check buffer limitations!!
    */
    source = sprite->data;    
    dest = (unsigned char*)rbxeGetBuffer();  
    dest += ((int)sprite->position.y * width + (int)sprite->position.x)  * 4; 

    for (y=0; y<sprite->cell_height; y++) {
        memcpy(dest, source, sprite->cell_height*4);
        
        source += sprite->width * 4;
        dest -= (width * 4);
    }
}

void rbxeSpriteUpdate(Sprite* sprite) {
    sprite->position.x += sprite->velocity.x * rbxeDeltaTime();
    sprite->position.y += sprite->velocity.y * rbxeDeltaTime();
}

void rbxeSpriteSetPosition(Sprite* sprite, float x, float y) {
    sprite->position = vec2_create(x, y);
}

void rbxeSpriteSetVelocity(Sprite* sprite, float x, float y) {
    sprite->velocity = vec2_create(x, y);
}

void rbxeSpriteSetActive(Sprite* sprite, int active) {
    sprite->is_active = active;
}

void rbxeSpriteSetHealth(Sprite* sprite, int health) {
    sprite->health = health;
}

#endif /* RBXE_ENGINE */
#endif /* RB_PIXEL_ENGINE_SPRITE_H */
