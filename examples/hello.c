#define RBXE_APPLICATION
#include <rbxe.h>

int main(void) {
    if (!rbxeStart("Hello", 800, 600, 100, 75)) return EXIT_FAILURE;

    while (rbxeRun()) {
        if (rbxeKeyPressed(KEY_ESCAPE)) {
            break;
        }
    }

    return rbxeEnd();
}

