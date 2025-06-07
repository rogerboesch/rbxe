#  define FILEOBJ   FILE
#  define FOPEN     fopen
#  define FCLOSE    fclose
#  define FSEEK     fseek
#  define FTELL     ftell
#  define FREAD     fread
#  define REWIND    rewind

#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))

extern void rlog(const char *fmt, ...);
extern void rerror(const char *fmt, ...);
extern void exit_error(const char *msg, ...);
extern char *readfile(const char *fname);

extern void init_game(int argc, char *argv[]);
extern void deinit_game();
extern void render(double deltaTime);
