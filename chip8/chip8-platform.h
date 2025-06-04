#  define FILEOBJ   FILE
#  define FOPEN     fopen
#  define FCLOSE    fclose
#  define FSEEK     fseek
#  define FTELL     ftell
#  define FREAD     fread
#  define REWIND    rewind


#include "chip8-constants.h"
#include "chip8-bmp.h"

#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))

extern Bitmap *screen;

extern int quit;

extern int show_debug();

extern int key_pressed();

extern int mouse_clicked();
extern int mouse_released();
extern int mouse_down();
extern int mouse_moved();
extern int mouse_x, mouse_y;

extern void set_cursor(Bitmap *b, int hsx, int hsy);

extern void rlog(const char *fmt, ...);

extern void rerror(const char *fmt, ...);

extern void exit_error(const char *msg, ...);

extern char *readfile(const char *fname);

extern Bitmap *get_bmp(const char *filename);

/* These functions should be provided elsewhere */
extern void init_game(int argc, char *argv[]);
extern void deinit_game();
extern int render(double deltaTime);
