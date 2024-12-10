#define RBXE_ENGINE
#include <rbxe.h>
#include <rbxe-sprite.h>
#include <rbxe-font.h>

#define WIDTH 800
#define HEIGHT 600
#define SCALE 1 / 2

int main(void) {
   int mouseX, mouseY;
    const int width = WIDTH * SCALE, height = HEIGHT * SCALE;
    const Pixel black = {0, 0, 0, 255}, white = {255, 255, 255, 255};
    char str[255];
    Sprite* sprite;
    
    if (!rbxeStart("Sprite", WIDTH, HEIGHT, width, height)) return EXIT_FAILURE;

    rbxeFontInit();

    sprite = rbxeSpriteLoad("player.png", 24, 24);
    if (!sprite) return EXIT_FAILURE;

    while (rbxeRun()) {
        if (rbxeKeyPressed(KEY_ESCAPE)) {
            break;
        }

        rbxeClear(0);

        rbxeMousePos(&mouseX, &mouseY);
 
        if (mouseX >= 0 && mouseX < width && mouseY >= 0 && mouseY < height) {
            sprintf(str, "%d,%d", mouseX, mouseY);
            rbxeFontDrawString(10, 10, str, white, black);
        }

        rbxeSpriteRender(sprite);
    }

    return rbxeEnd();
}

