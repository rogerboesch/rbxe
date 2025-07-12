#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// Include main function if set
#define STANDALONE 1

#define RBXE_ENGINE
#include "rbxe.h"
#include "gameboy.h"

#define ROM_FILE "/Users/roger/Data/Projects-Retro/boulder.gb"
#define SCALE 1
#define FULLSCREEN FALSE

u8* rom;
u32 rom_size;
FILE* rom_f;
u8* save;
u32 save_size;
FILE* save_f;
char save_file[260];

pixel_info buf[LCD_HEIGHT][LCD_WIDTH];
int i, x, y;
u32 romread;

double old_ticks;
double delay;
int run;

void platform_delay(unsigned int ms) {
    usleep(ms * 1000); 
}

double platform_get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    double time = (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;

    return time;
}

// Color schemes
pixel_info pixel_map_y[4] = {{0xff, 0xff, 0xff, 0xff}, {0x99, 0x99, 0x99, 0x99}, {0x44, 0x44, 0x44, 0x44}, {0x00, 0x00, 0x00, 0x00}};
pixel_info pixel_map_r[4] = {{0xff, 0xff, 0xff, 0xff}, {0xff, 0x99, 0x99, 0xff}, {0x44, 0x44, 0x99, 0xff}, {0x00, 0x00, 0x00, 0x00}};
pixel_info pixel_map_g[4] = {{0xff, 0xff, 0xff, 0xff}, {0x99, 0xff, 0x99, 0xff}, {0x99, 0x44, 0x44, 0xff}, {0x00, 0x00, 0x00, 0x00}};
pixel_info pixel_map_b[4] = {{0xff, 0xff, 0xff, 0xff}, {0x99, 0x99, 0x99, 0xff}, {0x44, 0x99, 0x44, 0xff}, {0x00, 0x00, 0x00, 0x00}};
pixel_info pixel_map_o[4] = {{0xff, 0xff, 0xee, 0xff}, {0xff, 0xff, 0x66, 0xff}, {0x44, 0x44, 0x99, 0xff}, {0x00, 0x00, 0x00, 0x00}};

pixel_info ColorToPixel(u16 cgb) {
	u8 r = (cgb & 0x001F) << 3;         // * 0xFF / 0x1F;
	u8 g = ((cgb >>  5) & 0x001F) << 3; // * 0xFF / 0x1F;
	u8 b = ((cgb >> 10) & 0x001F) << 3; // * 0xFF / 0x1F;

    pixel_info p;
    p.r = r;
    p.g = g;
    p.b = b;
    p.a = 255;

    return p;
}

void game_init(void) {
    run = 0;

    if ((access(ROM_FILE, F_OK) != -1) && ((rom_f = fopen(ROM_FILE, "rb")) != NULL)) {
		printf("%s: Opening %s.\n", __func__, ROM_FILE);
		rom_f = fopen(ROM_FILE, "rb");
	}
	else {
		printf("%s: File \"%s\" not found.\n", __func__, ROM_FILE);
		return;
	}

	fseek(rom_f, 0, SEEK_END);
	rom_size = ftell(rom_f);
	rewind(rom_f);
	rom = (u8*)malloc(rom_size);
	
    for (i = 0; i < rom_size; i++) {
		rom[i] = 0xFF;
    }

	romread = fread(rom, sizeof(u8), rom_size, rom_f);
	fclose(rom_f);

	// Load SAVE file (if it exists)
	sprintf(save_file, "%s.sav", ROM_FILE);
	save_size = GetSaveSize(rom);
	save = (u8*)malloc(save_size);
	save_f = fopen(save_file, "rb");
	
    if (save_f) {
		fseek(save_f, 0, SEEK_SET);
		fread(save, sizeof(u8), save_size, save_f);
		fclose(save_f);
	}

	LoadROM(rom, rom_size, save, save_size);

    if (!rbxeStart("GB", LCD_WIDTH, LCD_HEIGHT, SCALE, FULLSCREEN)) {
        return;
    }

	old_ticks = platform_get_time();
    run = 1;
}

void game_frame(void) {
    run = rbxeRun();
   
    RunFrame();

    if (gb_framecount == 0) {
        if (cgb_enable) {
            for (y = 0; y < LCD_HEIGHT; y++) {
                for (x = 0; x < LCD_WIDTH; x++) {
                    buf[y][x] = ColorToPixel(cgb_fb[y][x]);
                }
            }
        }
        else {
            for (y = 0; y < LCD_HEIGHT; y++) {
                for (x = 0; x < LCD_WIDTH; x++) {
                    buf[y][x] = pixel_map_y[gb_fb[y][x] & 3];
                }
            }
        }

        for (y = 0; y < LCD_HEIGHT; y++) {
            for (x = 0; x < LCD_WIDTH; x++) {
                rbxeSetPixel(x, LCD_HEIGHT-y, buf[y][x]);
            }
        }
        
        delay = 0.016 - (platform_get_time() - old_ticks);
        if  (delay > 0) {
            platform_delay(delay*1000);
        }

        old_ticks = platform_get_time();
    }
}

void game_exit(void) {
 	free(rom);
	free(save);
}

#ifdef STANDALONE

int main(void) {
    game_init();

	while (run) {
        game_frame();
	}

    rbxeEnd();
    game_exit();

	return 0;
}

#endif // STANDALONE