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
#include <rbxe.h>
#include <rbxe-font.h>

#define WINDOW_CAPTION "CHIP-8 Machine"
#define LOG_FILE_NAME "chip8.log"

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
        rom_step();
    }

    rom_deinit();

    rlog("%s: Main loop stopped", WINDOW_CAPTION);
    rlog("%s","Application Done!\n");

    fclose(logfile);

    return 0;
}
