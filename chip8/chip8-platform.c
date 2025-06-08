#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include <stdarg.h>
#include <assert.h>

#include "chip8-platform.h"
#include "rbxe.h"

#define WINDOW_CAPTION "CHIP-8 Machine"
#define LOG_FILE_NAME "chip8.log"

static double frameTimes[256];
static uint32_t n_elapsed = 0;

char *readfile(const char *fname) {
    FILEOBJ *f;
    long len,r;
    char *str;

    if (!(f = FOPEN(fname, "rb")))
        return NULL;

    FSEEK(f, 0, SEEK_END);
    len = (long)FTELL(f);
    REWIND(f);

    if (!(str = malloc(len+2)))
        return NULL;
    
    r = FREAD(str, 1, len, f);

    if (r != len) {
        free(str);
        return NULL;
    }

    FCLOSE(f);
    str[len] = '\0';
    return str;
}

static FILE *logfile = NULL;

void rlog(const char *fmt, ...) {
    va_list arg;
    va_start(arg, fmt);

    fputs("INFO: ", logfile);
    vfprintf(logfile, fmt, arg);
    fputc('\n', logfile);

    va_end(arg);
}

void rerror(const char *fmt, ...) {
    va_list arg;
    va_start(arg, fmt);

    fputs("ERROR: ", logfile);
    vfprintf(logfile, fmt, arg);
    fputc('\n', logfile);

    va_end(arg);
}

void exit_error(const char *fmt, ...) {
    if (fmt) {
        va_list arg;
        va_start (arg, fmt);

        fputs("ERROR: ", logfile);
        vfprintf (logfile, fmt, arg);

        va_end (arg);
    }
    if (logfile != stdout && logfile != stderr)
        fclose(logfile);
    else {
        fflush(stdout);
        fflush(stderr);
    }
    exit(1);
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
    if (elapsed < 10) {
        return;
    }
    
    double deltaTime = elapsed / 1000.0;
    rom_render(deltaTime);

    start = get_ticks();

    frameTimes[(n_elapsed++) & 0xFF] = deltaTime;
}

static void do_iteration() {
    draw_frame();

    /* TODO: Cursor handling */
}

int main(int argc, char *argv[]) {
    logfile = fopen(LOG_FILE_NAME, "w");

    if (!logfile) {
        return 1;
    }

    rlog("%s: Application Running", WINDOW_CAPTION);

    srand((unsigned int)time(NULL));

    if (!rom_init(argc, argv)) {
        return 1;
    }

    rlog("%s: Entering main loop", WINDOW_CAPTION);

    while (!rbxeKeyDown(KEY_ESCAPE)) {
        do_iteration();
    }

    rom_deinit();

    rlog("%s: Main loop stopped", WINDOW_CAPTION);
    rlog("%s","Application Done!\n");

    fclose(logfile);

    return 0;
}
