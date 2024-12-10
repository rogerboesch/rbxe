#define RBXE_ENGINE
#include <rbxe.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define pxAt(px, w, x, y) (px[(((y) * (w)) + (x))].r)

static const Pixel black = {0, 0, 0, 255};
static const Pixel white = {255, 255, 255, 255};

static void pxInit(const size_t size) {
    Pixel* pixbuf = rbxeGetBuffer();
    size_t i;

    for (i = 0; i < size; ++i) {
        memcpy(pixbuf + i, rand() % 2 ? &white : &black, sizeof(Pixel));
    }
}

static void pxUpdate(Pixel* buf, const int width, const int height) {
    int x, y, count, index = 0;
    Pixel* pixbuf = rbxeGetBuffer();

    for (y = 0; y < height; ++y) {
        for (x = 0; x < width; ++x, ++index) {
            count = 0;
            count += (x + 1 < width) && pxAt(pixbuf, width, x + 1, y);
            count += (x + 1 < width) && (y > 0) && pxAt(pixbuf, width, x + 1, y - 1);
            count += (y > 0) && pxAt(pixbuf, width, x, y - 1);
            count += (x > 0) && (y > 0) && pxAt(pixbuf, width, x - 1, y - 1);
            count += (x > 0) && pxAt(pixbuf, width, x - 1, y);
            count += (x > 0) && (y + 1 < height) && pxAt(pixbuf, width, x - 1, y + 1);
            count += (y + 1 < height) && pxAt(pixbuf, width, x, y + 1);
            count += (x + 1 < width) && (y + 1 < height) && pxAt(pixbuf, width, x + 1, y + 1);

            buf[index] = pixbuf[index].r ? (count == 2 || count == 3 ? white : black) : (count == 3 ? white : black);
        }
    }

    memcpy(pixbuf, buf, width * height * sizeof(Pixel));
}

int main(const int argc, char** argv) {
    Pixel* buf;
    const Pixel red = {255, 0, 0, 255};
    int mouseX, mouseY, width = 320, height = 240;
    
    if (argc > 1) {
        width = atoi(argv[1]);
        height = argc > 2 ? atoi(argv[2]) : width;
    }

    if (!rbxeStart("Game Of Life", 800, 600, width, height)) return EXIT_FAILURE;

    srand(time(NULL));

    buf = (Pixel*)malloc(width * height * sizeof(Pixel));
    pxInit(width * height);

    while (rbxeRun()) {
        rbxeMousePos(&mouseX, &mouseY);

        if (rbxeKeyPressed(KEY_ESCAPE) || rbxeKeyPressed(KEY_Q)) {
            break;
        }

        if (rbxeKeyPressed(KEY_R)) {
            pxInit(width * height);
        }
        
        pxUpdate(buf, width, height);
        
        if (mouseX >= 0 && mouseX < width && mouseY >= 0 && mouseY < height) {
            rbxeSetPixel(mouseX, mouseY, red);
        }
    }
    
    free(buf);
    return rbxeEnd();
}

