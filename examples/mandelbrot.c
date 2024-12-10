#define RBXE_APPLICATION
#include <rbxe.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ITERS 50
#define CLAMP(f) (double)(((f) > 1.0) + f * ((f) >= 0.0 && (f) <= 1.0))

typedef struct vec2 {
    double x, y;
} vec2;

static double zoom = 1.0;

static double dlog2(double x) {
    return log(x) / log(2);
}

static vec2 vec2_create(double x, double y) {
    vec2 p;
    p.x = x;
    p.y = y;
    return p;
}

static vec2 vec2_mult(const vec2 a, const vec2 b) {
    return vec2_create(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x);
}

static vec2 vec2_add(const vec2 a, const vec2 b) {
    return vec2_create(a.x + b.x, a.y + b.y);
}

static double vec2_dot(const vec2 a, const vec2 b) {
    return a.x * b.x + a.y * b.y;
}

static void pxUpdate(const int width, const int height, vec2 pos, double t) {
    int i, x, y;
    Pixel color;

    pos.x = (pos.x + pos.x * zoom) * 0.5;
    pos.y = (pos.y + pos.y * zoom) * 0.5;

    for (y = 0; y < height; ++y) {
        for (x = 0; x < width; ++x) {   
            
            double dot, n = 0.0;
            vec2 z = {0.0, 0.0};
            
            const vec2 p = vec2_create(((double)x + pos.x) / ((double)width * zoom), ((double)y + pos.y) / ((double)height * zoom)
            );

            for (i = 0; i < ITERS; ++i) {
                z = vec2_add(p, vec2_mult(z, z));
                dot = vec2_dot(z, z);
                
                if (dot > 45678.0) {
                    n = ((double)i - (dlog2(dot)) + 4.0) / (double)ITERS;
                    break;
                }
            }

            color.r = (unsigned)(CLAMP(sin(t) * n * 0.8 + 0.2) * 255.0);
            color.g = (unsigned)(CLAMP(sin(t * 0.333333) * n) * 255.0);
            color.b = (unsigned)(CLAMP(cos(t * 0.7) * n * 0.8 + 0.2) * 255.0);
            color.a = 255;

            rbxeSetPixel(x, y, color);
        }
    }
}

int main(const int argc, const char** argv) {
    double t;
    vec2 pos = {0.0, 0.0};
    int width = 64, height = 64;

    if (argc > 1) {
        width = atoi(argv[1]);
        height = argc > 2 ? atoi(argv[2]) : width;
    }
  
    if (!rbxeStart("Mandelbrot", 800, 600, width, height)) return EXIT_FAILURE;

    rbxeClear(255);
    t = rbxeTime();

    while (rbxeRun()) { 
        double dT, T = rbxeTime();
        dT = (T - t) * 50.0;
        t = T;

        if (rbxeKeyPressed(KEY_ESCAPE) || rbxeKeyPressed(KEY_Q)) {
            break;
        }
        if (rbxeKeyDown(KEY_D)) {
            pos.x += dT / zoom;
        }
        if (rbxeKeyDown(KEY_A)) {
            pos.x -= dT / zoom;
        }
        if (rbxeKeyDown(KEY_W)) {
            pos.y += dT / zoom;
        }
        if (rbxeKeyDown(KEY_S)) {
            pos.y -= dT / zoom;
        }
        if (rbxeKeyDown(KEY_Z)) {
            zoom += dT * 0.01 * zoom;
        }
        if (rbxeKeyDown(KEY_X)) {
            zoom -= dT * 0.01 * zoom;
        }
        
        pxUpdate(width, height, pos, t);
    }

    return rbxeEnd();
}

