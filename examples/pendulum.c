#define RBXE_APPLICATION
#include <rbxe.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ABS(n) ((n) > 0 ? (n) : -(n))

typedef struct vec2 {
    float x, y;
} vec2;

typedef struct ivec2 {
    int x, y;
} ivec2;

typedef struct bmp4 {
    Pixel* pixbuf;
    int width, height;
} bmp4;

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

int main(const int argc, const char** argv) {
    const Pixel red = {255, 0, 0, 255}, green = {0, 255, 0, 255};
    
    vec2 xy, dif, cross;
    ivec2 p, center, idif;
    bmp4 fb = {NULL, 200, 150};
    float invDist, dT, T, t, v = 0.0f;

    if (argc > 1) {
        fb.width = atoi(argv[1]);
        fb.height = argc > 2 ? atoi(argv[2]) : fb.width;
    }

    fb.pixbuf = rbxeStart("Pendulum", 800, 600, fb.width, fb.height);

    center = ivec2_create(fb.width / 2, fb.height / 2);
    p = ivec2_create(center.x + fb.width / 8, center.y);
    xy = vec2_create((float)p.x, (float)p.y);

    idif = ivec2_create(p.x - center.x, p.y - center.y);
    invDist = 1.0F / sqrt(idif.x * idif.x + idif.y * idif.y);
    t = rbxeTime();

    while (rbxeRun(fb.pixbuf)) {
        T = rbxeTime();
        dT = (T - t);
        t = T;

        if (rbxeKeyPressed(KEY_ESCAPE)) {
            break;
        }

        dif = vec2_create((float)(p.x - center.x), (float)(p.y - center.y));
        cross = vec2_create(dif.y * invDist, dif.x * invDist);
        
        v -= cross.y;
        xy.x -= cross.x * v * dT;
        xy.y += cross.y * v * dT;
        p.x = (int)xy.x;
        p.y = (int)xy.y;

        rbxeClear(155);

        if (p.x >= 0 && p.y >= 0 && p.x < fb.width && p.y < fb.height) {
            ivec2 d = ivec2_create(
                (int)(xy.x - cross.x * v * 0.25F),
                (int)(xy.y + cross.y * v * 0.25F)
            );

            rbxePlotLine(center.x, center.y, p.x, p.y, red);
            rbxePlotLine(p.x, p.y, d.x, d.y, green);
        }
    }

    return rbxeEnd(fb.pixbuf);
}

