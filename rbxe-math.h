#ifndef RB_PIXEL_ENGINE_MATH_H
#define RB_PIXEL_ENGINE_MATH_H

typedef struct vec2 {
    float x, y;
} vec2;

static vec2 vec2_create(float x, float y);

#ifdef RBXE_APPLICATION

static vec2 vec2_create(float x, float y) {
    vec2 p;
    p.x = x;
    p.y = y;

    return p;
}

#endif /* RBXE_APPLICATION */
#endif /* RB_PIXEL_ENGINE_MATH_H */
