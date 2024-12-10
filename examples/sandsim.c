#define RBXE_APPLICATION
#include <rbxe.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static const Pixel sand = {125, 125, 0, 255};

static Pixel pxAir(const int height, int y) {
    Pixel air = {100, 100, 130, 255};
    air.b += (uint8_t)(int)(125.0 * ((float)y / (float)height));

    return air;
}

static void pxInit(const int width, const int height) {
    int x, y, index = 0;
    Pixel* pixbuf = rbxeGetBuffer();

    for (y = 0; y < height; ++y) {
        for (x = 0; x < width; ++x, ++index) {
            pixbuf[index] = y < height / 3 - (rand() % 40) ? sand: pxAir(height, y);
        }
    }
}

static void pxUpdate(Pixel* buf, const int width, const int height) {
    int x, y, dy, index = 0;
    Pixel* pixbuf = rbxeGetBuffer();

    for (y = 0; y < height; ++y) {
        for (x = 0; x < width; ++x, ++index) { 
            if (pixbuf[index].r > 124) {
                if (y > 0) {
                    dy = (y - 1) * width;
                    if (pixbuf[dy + x].r < 125) {
                        buf[dy + x] = sand;
                        buf[index] = pxAir(height, y);
                    } else if (x + 1 < width && pixbuf[dy + x + 1].r < 125) {
                        buf[dy + x + 1] = sand;
                        buf[index] = pxAir(height, y);
                    } else if (x > 0 && pixbuf[dy + x - 1].r < 125) {
                        buf[dy + x - 1] = sand;
                        buf[index] = pxAir(height, y);
                    }
                }
            }
            else buf[index] = pxAir(height, y);
        }
    }

    memcpy(pixbuf, buf, width * height * sizeof(Pixel));
}

int main(const int argc, const char** argv) {
    Pixel* buf;
    const Pixel red = {255, 0, 0, 255};
    int mouseX, mouseY, width = 160, height = 120;
    
    if (argc > 1) {
        width = atoi(argv[1]);
        height = argc > 2 ? atoi(argv[2]) : width;
    }

    if (!rbxeStart("Sand Simulation", 800, 600, width, height)) return EXIT_FAILURE;

    srand(time(NULL));

    buf = (Pixel*)malloc(width * height * sizeof(Pixel));
    pxInit(width, height);

    while (rbxeRun()) {
        rbxeMousePos(&mouseX, &mouseY);

        if (rbxeKeyPressed(KEY_ESCAPE) || rbxeKeyPressed(KEY_Q)) {
            break;
        }
        
        if (rbxeKeyPressed(KEY_R)) {
            pxInit(width, height);
        }
        
        pxUpdate(buf, width, height);
        
        if (rbxeMouseDown(MOUSE_LEFT) && 
            mouseX >= 0 && mouseX < width && mouseY >= 0 && mouseY < height) {
            rbxeSetPixel(mouseX, mouseY, red);
        }
    }
    
    free(buf);
    return rbxeEnd();
}

