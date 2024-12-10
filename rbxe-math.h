#ifndef RB_PIXEL_ENGINE_MATH_H
#define RB_PIXEL_ENGINE_MATH_H

typedef struct vec2 {
    float x, y;
} vec2;

typedef struct ivec2 {
    int x, y;
} ivec2;

static vec2 vec2_create(float x, float y);
static ivec2 ivec2_create(int x, int y);

#ifdef RBXE_ENGINE

static vec2 vec2_create(float x, float y) {
    vec2 p;
    p.x = x;
    p.y = y;

    return p;
}

static ivec2 ivec2_create(int x, int y) {
    ivec2 p;
    p.x = x;
    p.y = y;

    return p;
}

#endif /* RBXE_ENGINE */
#endif /* RB_PIXEL_ENGINE_MATH_H */
