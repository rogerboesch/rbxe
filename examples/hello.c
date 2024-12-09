#define RBXE_APPLICATION
#include <rbxe.h>

int main(void) {
    Pixel* pixbuf = rbxeStart("Hello", 800, 600, 100, 75);

    while (rbxeRun(pixbuf)) {
        if (rbxeKeyPressed(KEY_ESCAPE)) {
            break;
        }
    }

    return rbxeEnd(pixbuf);
}

