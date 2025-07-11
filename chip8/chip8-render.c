#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#include "chip8.h"
#include "chip8-platform.h"

#define RBXE_ENGINE
#include <rbxe.h>
#include <rbxe-font.h>

#define BORDER 16
#define ZOOM 10
#define WIDTH 64*ZOOM+2*BORDER
#define HEIGHT 32*ZOOM+2*BORDER
#define SCALE 1
#define FULLSCREEN FALSE

// Mac
//const char *romfile1 = "/Users/roger/Data/Projects-Retro/PixelEngine/bin/TETRIS.ch8";
//const char *romfile2 = "/Users/roger/Data/Projects-Retro/PixelEngine/bin/BLINKY.ch8";

// Linux
const char *romfile1 = "/home/roger/Data/rbxe/chip8/roms/BLINKY.ch8";
const char *romfile2 = "/home/roger/Data/rbxe/chip8/roms/TETRIS.ch8";

/* number of instructions to execute per second */
static int speed = 1200;

/* Colors */
const pixel_info fg_color = {255, 192, 0, 255};
const pixel_info bg_color = {0, 0, 0, 255};
const pixel_info fg_color_dbg = {255, 255, 255, 255};
const pixel_info bg_color_dbg = {255, 0, 0, 255};

/* Is the interpreter running? Set to 0 to enter "debug" mode */
static int running = 1;
static int showfps = 0;

static double frameTimes[256];
static uint32_t n_elapsed = 0;

static void draw_screen();

static void usage() {
    exit_error("Use these command line variables:\n"
                "  -f fg        : Foreground color\n"
                "  -b bg        : Background color\n"
                "  -s spd       : Specify the speed\n"
                "  -d           : Debug mode\n"
                "  -v           : increase verbosity\n"
                "  -q quirks    : sets the quirks mode\n"
                "      `quirks` can be a comma separated combination\n"
                "      of `none`, `vf`, `mem`, `disp`, `clip`, `shift`,\n"
                "      `jump`, `default`, `chip8` and `schip`\n"
                "  -a addr=val  : Sets the byte in RAM at `addr` to\n"
                "                 the value `val` before executing.\n"
                "  -h           : Displays this help\n"
                );
}

/*
 * This is an example of how you can write a CHIP8 program that can access the
 *  world outside by invoking the `SYS nnn` (`0nnn`) instruction
 *
 * ```
 * CLS
 * LD V0, #AB
 * SYS 2
 * LD V0, #CD
 * SYS 2
 * LD V0, #EF
 * SYS 2
 * LD I, string
 * SYS 1
 * EXIT
 * string: text "Hello World!"
 * ```
 */
int example_sys_hook(unsigned int nnn) {
    switch (nnn) {
        case 1:
            {
                char *str = (char*)&C8.RAM[C8.I];
                rlog("console: %s", str);
            }
            break;
        case 2:
            {
                rlog("console: V0: %02X", C8.V[0]);
            }
            break;
        case 3:
            {
                rlog("console: Halting interpreter; VF: %02X", C8.V[0xF]);
                return 0;
            }
            break;
        default:
            break;
    }
    
    return 1;
}

int rom_init(int argc, char *argv[]) {
    rlog("Initializing...");

    srand((uint32_t)time(NULL));

    c8_reset();

    if (!rbxeStart("Chip8", WIDTH, HEIGHT, SCALE, FULLSCREEN)) {
        rerror("Can't initialize RBXE");
        return 0;
    }

    rbxeFontInit();
    
    int opt;

    while ((opt = getopt(argc, argv, "f:b:s:dvhq:m:")) != -1) {
        switch(opt) {
            case 'v':
                c8_verbose++;
                break;
            case 's':
                speed = atoi(optarg);
                if (speed < 1) speed = 10;
                break;
            case 'd':
                running = 0;
                break;
            case 'q':
                {
                    unsigned int quirks = 0;
                    
                    char *token = strtok(optarg, ",");
                    while (token) {
                        if (!strcmp(token, "none")) quirks = 0;
                        else if (!strcmp(token, "vf")) quirks |= QUIRKS_VF_RESET;
                        else if (!strcmp(token, "mem")) quirks |= QUIRKS_MEM_CHIP8;
                        else if (!strcmp(token, "disp")) quirks |= QUIRKS_DISP_WAIT;
                        else if (!strcmp(token, "clip")) quirks |= QUIRKS_CLIPPING;
                        else if (!strcmp(token, "shift")) quirks |= QUIRKS_SHIFT;
                        else if (!strcmp(token, "jump")) quirks |= QUIRKS_JUMP;
                        else if (!strcmp(token, "default")) quirks |= QUIRKS_DEFAULT;
                        else if (!strcmp(token, "chip8")) quirks |= QUIRKS_CHIP8;
                        else if (!strcmp(token, "schip")) quirks |= QUIRKS_SCHIP;
                        else rerror("warning: unknown quirk '%s'", token);
                        
                        token = strtok(NULL, ",");
                    }
                    c8_set_quirks(quirks);
                }
                break;
            case 'm':
                {
                    int addr, val;
                    char *token = strtok(optarg, ",");
                    while (token) {
                        char *delim = strchr(token, '=');
                        
                        if (!delim) {
                            rerror("error: bad field for -m; expected `addr=value`, got `%s`", token);
                            return 0;
                        }

                        *delim = '\0';
                        delim++;
                        addr = (int)strtol(token, NULL, 0);
                        val = (int)strtol(delim, NULL, 0);
                        
                        if (addr < 0 || addr >= TOTAL_RAM)
                            exit_error("error: bad address for -m: 0 <= addr < %d", TOTAL_RAM);
                        if (val < 0 || val >= 256)
                            exit_error("error: bad address for -m: 0 <= val < 256");

                        c8_set(addr, val);

                        token = strtok(NULL, ",");
                    }
                }
                break;
            case 'h':
                usage();
                break;
        }
    }

    c8_sys_hook = example_sys_hook;

    rlog("Loading %s...", romfile1);

    if (!c8_load_file(romfile1)) {
        rerror("Unable to load '%s': %s\n", romfile1, strerror(errno));
        return 0;
    }

    draw_screen();

    rlog("Initialized.");

    return 1;
}

int rom_deinit() {
    rbxeEnd();
    rlog("Done.");

    return 1;
}

int rom_load(const char* filename) {
    rlog("Loading %s...", filename);

    c8_reset();
    
    if (!c8_load_file(filename)) {
        rerror("Unable to load '%s': %s\n", filename, strerror(errno));
        return 0;
    }
}

static void draw_screen() {
    int w, h;

    c8_resolution(&w, &h);

    int xx, yy;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            pixel_info c = c8_get_pixel(x,y) ? fg_color : bg_color;
            
            xx = x * ZOOM + BORDER;
            yy = (h-y) * ZOOM + BORDER;
            rbxePlotRectangle(xx, yy, xx+ZOOM, yy+ZOOM, c);
        }
    }
}

static int test_key1(int code, int index) {
    if (rbxeKeyDown(code)) {
        c8_key_down(index);
        return 1;
    }
    else {
        c8_key_up(index);
        return 0;
    }
}

static int test_key2(int code1, int code2, int index) {
    if (rbxeKeyDown(code1) || rbxeKeyDown(code2)) {
        c8_key_down(index);
        return 1;
    }
    else {
        c8_key_up(index);
        return 0;
    }
}

int rom_render(double elapsedSeconds) {
    int i;
    static double timer = 0.0;
    int key_pressed = 0;
    
    rbxeRun();
    
    key_pressed = test_key1(KEY_0, 0);
    key_pressed = test_key1(KEY_1, 1);
    key_pressed = test_key2(KEY_2, KEY_UP, 2);
    key_pressed = test_key1(KEY_3, 3);
    key_pressed = test_key2(KEY_4, KEY_LEFT, 4);
    key_pressed = test_key1(KEY_5, 5);
    key_pressed = test_key2(KEY_6, KEY_RIGHT, 6);
    key_pressed = test_key1(KEY_7, 7);
    key_pressed = test_key2(KEY_8, KEY_DOWN, 8);
    key_pressed = test_key1(KEY_9, 9);
    key_pressed = test_key1(KEY_A, 10);
    key_pressed = test_key1(KEY_B, 11);
    key_pressed = test_key1(KEY_C, 12);
    key_pressed = test_key1(KEY_D, 13);
    key_pressed = test_key1(KEY_E, 14);
    key_pressed = test_key1(KEY_F, 15);

    timer += elapsedSeconds;

    while (timer > 1.0/60.0) {
        c8_60hz_tick();
        timer -= 1.0/60.0;
    }
    
    if (rbxeKeyPressed(KEY_F10)) {
        showfps = !showfps;
    }

    if (rbxeKeyPressed(KEY_F11)) {
        c8_disasm();
    }

    if (rbxeKeyPressed(KEY_F12)) {
        rom_load(romfile2);
    }

    if (running) {
        if (rbxeKeyPressed(KEY_F5)) {
            running = 0;
        }
        
        /* instructions per second * elapsed seconds = number of instructions to execute */
        int count = speed * elapsedSeconds;

        for (i = 0; i < count; i++) {
            if (c8_ended())
                return 0;
            else if(c8_waitkey() && !key_pressed)
                return 1;

            c8_step();

            if (c8_screen_updated())
                draw_screen();
        }
    }
    else {
        /* Debugging mode: F6 steps through the program, F8 resumes */

        if (rbxeKeyPressed(KEY_F8)) {
            running = 1;
            return 1;
        }
        
        if (rbxeKeyPressed(KEY_F6)) {
            if (c8_ended())
                return 0;
            else if(c8_waitkey() && !key_pressed)
                return 1;
            
            c8_step();
            
            if (c8_screen_updated()) {
                draw_screen();
            }
            
            /* TODO: Relese key F6 ? */
        }

        draw_screen();
    }

    return 1;
}

static uint32_t get_ticks(void) {
    clock_t uptime = clock() / (CLOCKS_PER_SEC / 1000);
    return (uint32_t)uptime;
}

static void draw_frame() {
    static uint32_t start = 0;
    static uint32_t elapsed = 0;

    elapsed = get_ticks() - start;

    /* It is technically possible for the game to run too fast, rendering the deltaTime useless */
    if (elapsed < 16) {
        return;
    }
    
    double deltaTime = elapsed / 1000.0;
    rom_render(deltaTime);

    start = get_ticks();

    /* Debug screen */
    if (showfps && n_elapsed > 0) {
         double sum = 0;
         int i, n = n_elapsed > 0xFF ? 0xFF : n_elapsed;
         
         for (i = 0; i < n; i++)
             sum += frameTimes[i];
        
         double avg = sum / n;
         double fps = 1.0 / avg;
        
        char str[16];
        snprintf(str, 16, "%3.2f", fps);
        rbxeFontDrawString(2*BORDER, 3*BORDER, str, fg_color_dbg, bg_color_dbg);
     }

    frameTimes[(n_elapsed++) & 0xFF] = deltaTime;
}

int rom_step() {
    draw_frame();
    
    /* TODO: Cursor handling */

    return 1;
}
