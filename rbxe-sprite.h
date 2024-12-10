#ifndef RB_PIXEL_ENGINE_SPRITE_H
#define RB_PIXEL_ENGINE_SPRITE_H

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <rbxe-math.h>

typedef struct Sprite {
	int id;
	int animation_nr;
    vec2 position;
	int is_active;
    int health;

    /* Sprite atlas information*/
    unsigned char* data;
    int width;
    int height;
    int cell_width;
    int cell_height;
} Sprite;

Sprite* rbxeSpriteLoad(char* filename, int cell_width, int cell_height);
void rbxeSpriteRender(Sprite* sprite);
void rbxeSpriteSetPosition(Sprite* sprite, vec2 position);
void rbxeSpriteSetActive(Sprite* sprite, int active);
void rbxeSpriteSetHealth(Sprite* sprite, int health);

#ifdef RBXE_APPLICATION

static int unique_id = 0;

void rbxeSpriteDump(Sprite* sprite) {
    fprintf(stdout, "Sprite %d\n", sprite->id);
    fprintf(stdout, "+-animation: %d\n", sprite->animation_nr);
    fprintf(stdout, "+-position: %0.2f, %0.2f\n", sprite->position.x, sprite->position.y);
    fprintf(stdout, "+-w/h: %d, %d\n", sprite->width, sprite->height);
    fprintf(stdout, "+-cell: %d, %d\n", sprite->cell_width, sprite->cell_height);
}

Sprite* rbxeSpriteLoad(char* path, int cell_width, int cell_height) {
	int width, height, channel_count;
    Sprite *sprite;

    sprite = malloc(sizeof(Sprite));
    sprite->id = ++unique_id;
    sprite->animation_nr = 0;
    sprite->position = vec2_create(0,0);
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

void rbxeSpriteRender(Sprite* sprite) {
    int y, width, height;
    unsigned char* source, *dest;

    rbxeScreenSize(&width, &height);

    source = sprite->data;    
    dest = (unsigned char*)rbxeGetBuffer();   

    for (y=0; y<sprite->cell_height; y++) {
        memcpy(dest, source, sprite->cell_height*4);
        
        source += sprite->width * 4;
        dest += (width * 4);
    }
}

void rbxeSpriteSetPosition(Sprite* sprite, vec2 position) {
    sprite->position = position;
}

void rbxeSpriteSetActive(Sprite* sprite, int active) {
    sprite->is_active = active;
}

void rbxeSpriteSetHealth(Sprite* sprite, int health) {
    sprite->health = health;
}

#endif /* RBXE_APPLICATION */
#endif /* RB_PIXEL_ENGINE_SPRITE_H */
