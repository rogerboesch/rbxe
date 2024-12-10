#define RBXE_APPLICATION
#include <rbxe.h>

#include <rbxe-sprite.h>

int main(void) {
    Sprite* sprite;
    
    if (!rbxeStart("Sprite", 800, 600, 400, 300)) return EXIT_FAILURE;

    sprite = rbxeSpriteLoad("player.png", 24, 24);
    if (!sprite) return EXIT_FAILURE;
    
    while (rbxeRun()) {
        if (rbxeKeyPressed(KEY_ESCAPE)) {
            break;
        }

        rbxeSpriteRender(sprite);
    }

    return rbxeEnd();
}

