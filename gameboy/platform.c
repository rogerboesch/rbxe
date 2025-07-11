#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define RBXE_ENGINE
#include "rbxe.h"
#include "gameboy.h"

#define ROM_FILE "/Users/roger/Data/Projects-Retro/boulder.gb"
#define SCALE 1
#define FULLSCREEN FALSE

u8 frames;
u32 f0_ticks;
u32 f1_ticks;
u16 fps;

u8* rom;
u32 rom_size;
FILE* rom_f;
u8* save;
u32 save_size;
FILE* save_f;
char save_file[260];

// TODO: Color schemes
u32 COLORS_Y[4] = {0xFFFFFFFF, 0x99999999, 0x44444444, 0x00000000};
u32 COLORS_R[4] = {0xFFFFFFFF, 0xFFFF9999, 0xFF444499, 0x00000000};
u32 COLORS_G[4] = {0xFFFFFFFF, 0xFF99FF99, 0xFF994444, 0x00000000};
u32 COLORS_B[4] = {0xFFFFFFFF, 0xFF9999FF, 0xFF449944, 0x00000000};
u32 COLORS_O[4] = {0xFFFFFFEE, 0xFFFFFF66, 0xFF444499, 0x00000000};

pixel_info pixel_map_y[4] = {{0xff, 0xff, 0xff, 0xff}, {0x99, 0x99, 0x99, 0x99}, {0x44, 0x44, 0x44, 0x44}, {0x00, 0x00, 0x00, 0x00}};

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

int main(void) {
	int i, x, y;
	u8 j;
	u32 romread;
	u32 old_ticks;
	u32 new_ticks;
	int delay;
	u32* s;
	int quit_seq;
	pixel_info buf[LCD_HEIGHT][LCD_WIDTH];

	if ((access(ROM_FILE, F_OK) != -1) && ((rom_f = fopen(ROM_FILE, "rb")) != NULL)) {
		printf("%s: Opening %s.\n", __func__, ROM_FILE);
		rom_f = fopen(ROM_FILE, "rb");
	}
	else {
		printf("%s: File \"%s\" not found.\n", __func__, ROM_FILE);
		return -1;
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

	new_ticks= rbxeTime();
	f1_ticks = new_ticks;

    if (!rbxeStart("GB", LCD_WIDTH, LCD_HEIGHT, SCALE, FULLSCREEN)) {
        return EXIT_FAILURE;
    }

	while (rbxeRun()) {
		old_ticks = rbxeTime();
		usleep(16 - (new_ticks - old_ticks));

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

 			new_ticks = rbxeTime();
			frames++;
			
            if (frames % 0x80 == 0) {
				f0_ticks = f1_ticks;
				f1_ticks = new_ticks;
				fps = (128*1000) / (f1_ticks - f0_ticks);
			}

			// Cap at 60FPS
            delay = 16 - (new_ticks - old_ticks);
		    // TODO: Add delay function
		}
	}

    rbxeEnd();

	free(rom);
	free(save);

	return 0;
}

