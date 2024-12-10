#define RBXE_APPLICATION
#include <rbxe.h>

#define WIDTH 256
#define HEIGHT 256

static Pixel rbxeGradientAt(int x, int y) {
    Pixel px = {0, 0, 0, 255};
    px.r = x;
    px.b = y;

    return px;
}

static void rbxeDrawGradient(void) {
    int x, y;

    for (y = 0; y < HEIGHT; ++y) {
        for (x = 0; x < WIDTH; ++x) {
            rbxeSetPixel(x, y, rbxeGradientAt(x, y));
        }
    }
}

int main(void) {
    if (!rbxeStart("Gradient", 800, 600, WIDTH, HEIGHT)) return EXIT_FAILURE;

    rbxeDrawGradient();

    while (rbxeRun()) {
        if(rbxeKeyPressed(KEY_ESCAPE)) {
            break;
        }
    }

    return rbxeEnd();
}

