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

#if SCREEN_SCALE == 0
/* This has happened to me more than once */
#  error "You set SCREEN_SCALE to 0 again, dummy!"
#endif

#define VSCREEN_WIDTH    (SCREEN_WIDTH * (EPX_SCALE?2:1))
#define VSCREEN_HEIGHT   (SCREEN_HEIGHT * (EPX_SCALE?2:1))
#define WINDOW_WIDTH    (VSCREEN_WIDTH * SCREEN_SCALE)
#define WINDOW_HEIGHT   (VSCREEN_HEIGHT * SCREEN_SCALE)

#ifndef USE_LOG_STREAM
#  define USE_LOG_STREAM 0
#else
#  ifndef LOG_STREAM
#    define LOG_STREAM   stderr
#  endif
#endif

#ifndef LOG_FILE_NAME
#  define LOG_FILE_NAME "chip8.log"
#endif

Bitmap *screen;
Bitmap *vscreen;

static int pressed_key = 0;

int mouse_x, mouse_y;
static int mclick = 0, mdown = 0, mrelease = 0, mmove = 0;

static Bitmap *cursor = NULL;
static int cursor_hsx, cursor_hsy;
static Bitmap *cursor_back = NULL;

#if EPX_SCALE
static Bitmap *scale_epx_i(Bitmap *in, Bitmap *out);
static Bitmap *epx;
#endif

static int dodebug = 0;
static double frameTimes[256];
static uint32_t n_elapsed = 0;

int quit = 0;

/* This leaves a bit to be desired if I'm to
support multi-touch on mobile eventually */
int mouse_clicked() {
    return mclick;
}
int mouse_down() {
    return mdown;
}
int mouse_released() {
    return mrelease;
}
int mouse_moved() {
    return mmove;
}
int key_pressed() {
    return pressed_key;
}

int show_debug() {
// #ifdef NDEBUG
    // return 0;
// #else
    return dodebug;
// #endif
}

char *readfile(const char *fname) {
    FILEOBJ *f;
    long len,r;
    char *str;

    if(!(f = FOPEN(fname, "rb")))
        return NULL;

    FSEEK(f, 0, SEEK_END);
    len = (long)FTELL(f);
    REWIND(f);

    if(!(str = malloc(len+2)))
        return NULL;
    r = FREAD(str, 1, len, f);

    if(r != len) {
        free(str);
        return NULL;
    }

    FCLOSE(f);
    str[len] = '\0';
    return str;
}

void set_cursor(Bitmap *b, int hsx, int hsy) {
    cursor_hsx = hsx;
    cursor_hsy = hsy;
    cursor = b;
    int w = bm_width(b), h= bm_height(b);
    if(b) {
        if(!cursor_back)
            cursor_back = bm_create(w, h);
        else if(bm_width(cursor_back) != w || bm_height(cursor_back) != h) {
            bm_free(cursor_back);
            cursor_back = bm_create(w, h);
        }
    } else {
        bm_free(cursor_back);
        cursor_back = NULL;
    }
}

static const char *lastEvent = "---";
static int finger_id = -1;

static void handle_events() {
}

Bitmap *get_bmp(const char *filename) {
    Bitmap *bmp = bm_load(filename);
    return bmp;
}

static uint32_t get_ticks(void) {
    return 0;
}

static void draw_frame() {
    static uint32_t start = 0;
    static uint32_t elapsed = 0;

    elapsed = get_ticks() - start;

    /* It is technically possible for the game to run
       too fast, rendering the deltaTime useless */
    if(elapsed < 10)
        return;

    double deltaTime = elapsed / 1000.0;
    if(!render(deltaTime)) {
        quit = 1;
    }

    start = get_ticks();

    if(dodebug && n_elapsed > 0) {
        double sum = 0;
        int i, n = n_elapsed > 0xFF ? 0xFF : n_elapsed;
        for(i = 0; i < n; i++) sum += frameTimes[i];
        double avg = sum / n;
        double fps = 1.0 / avg;
        BmFont *save = bm_get_font(screen);
        bm_reset_font(screen);
        bm_set_color(screen, bm_atoi("red"));
        bm_fillrect(screen, 0, 0, 50, 10);
        bm_set_color(screen, bm_atoi("yellow"));
        bm_printf(screen, 1, 1, "%3.2f", fps);
        bm_set_font(screen, save);
    }
    frameTimes[(n_elapsed++) & 0xFF] = deltaTime;

    mclick = 0;
    mrelease = 0;
    mmove = 0;
    pressed_key = 0;
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
    if(fmt) {
        va_list arg;
        va_start (arg, fmt);

        fputs("ERROR: ", logfile);
        vfprintf (logfile, fmt, arg);

        va_end (arg);
    }
    if(logfile != stdout && logfile != stderr)
        fclose(logfile);
    else {
        fflush(stdout);
        fflush(stderr);
    }
    exit(1);
}

static void do_iteration() {
    int cx = 0, cy = 0;

    handle_events();

    draw_frame();

    if(cursor) {
        cx = mouse_x - cursor_hsx;
        cy = mouse_y - cursor_hsy;
        int cw = bm_width(cursor), ch = bm_height(cursor);
        bm_blit(cursor_back, 0, 0, screen, cx, cy, cw, ch);
        bm_maskedblit(screen, cx, cy, cursor, 0, 0, cw, ch);
    }

    if(cursor) {
        bm_maskedblit(screen, cx, cy, cursor_back, 0, 0, bm_width(cursor_back), bm_height(cursor_back));
    }
}

int main(int argc, char *argv[]) {
    logfile = fopen(LOG_FILE_NAME, "w");

    if (!logfile)
        return 1;

    rlog("%s: Application Running", WINDOW_CAPTION);

    srand((unsigned int)time(NULL));

    screen = bm_create(SCREEN_WIDTH, SCREEN_HEIGHT);

    init_game(argc, argv);

    rlog("%s: Entering main loop", WINDOW_CAPTION);

    while(!quit) {
        do_iteration();
    }

    deinit_game();

    rlog("%s: Main loop stopped", WINDOW_CAPTION);

    bm_unbind(vscreen);
    bm_free(screen);

    rlog("%s","Application Done!\n");

    fclose(logfile);
    return 0;
}
