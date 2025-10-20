/* Miscellaneous glue code for iACE, copyright (C) 2012 Edward Patel.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <string.h>
#include <unistd.h>

#define RBXE_ENGINE
#include "rbxe.h"

//#include "SoundGenerator.h"

#include "ace.rom.h"
#include "frogger.h"

#include "z80.h"

extern void z80_init(void);
extern void z80_frame(void);

static BYTE video_ram_old[32*24];

unsigned char mem[65536];
unsigned char *memptr[8] = {
    mem,
    mem+0x2000,
    mem+0x4000,
    mem+0x6000,
    mem+0x8000,
    mem+0xa000,
    mem+0xc000,
    mem+0xe000
};

int spooler_read_char() {
    return 0;
}

BYTE keyboard_get_keyport(int port) { return 0; }
void keyboard_keypress(int aceKey) {}
void keyboard_keyrelease(int aceKey);
void keyboard_clear() {}
void get_z80_internal_state(char **ptr, int *len);
void set_z80_internal_state(const char *ptr);

unsigned long tstates=0, tsmax=65000, tsmaxfreq=50;

int memattr[8] = {0,1,1,1,1,1,1,1}; /* 8K RAM Banks */

volatile int interrupted = 0;
int reset_ace = 0;

void save_p(int _de, int _hl) {
    // TODO: Implement
}

BYTE empty_bytes[799] = {       /* a small screen       */
    0x1a,0x00,0x20,0x6f,        /* will be loaded if    */
    0x74,0x68,0x65,0x72,        /* wanted file can't be */
    0x20,0x20,0x20,0x20,        /* opened               */
    0x20,0x00,0x03,0x00,
    0x24,0x20,0x20,0x20,
    0x20,0x20,0x20,0x20,
    0x20,0x20,0x20,0x20,
    0x01,0x03,0x43,0x6f,
    0x75,0x6c,0x64,0x6e,
    0x27,0x74,0x20,0x6c,
    0x6f,0x61,0x64,0x20,
    0x79,0x6f,0x75,0x72,
    0x20,0x66,0x69,0x6c,
    0x65,0x21,0x20,
};

BYTE empty_dict[] = {           /* a small forth program */
    0x1a,0x00,0x00,0x6f,        /* will be loaded if     */
    0x74,0x68,0x65,0x72,        /* wanted file can't be  */
    0x20,0x20,0x20,0x20,        /* opened                */
    0x20,0x2a,0x00,0x51,
    0x3c,0x58,0x3c,0x4c,
    0x3c,0x4c,0x3c,0x4f,
    0x3c,0x7b,0x3c,0x20,
    0x2b,0x00,0x52,0x55,
    0xce,0x27,0x00,0x49,
    0x3c,0x03,0xc3,0x0e,
    0x1d,0x0a,0x96,0x13,
    0x18,0x00,0x43,0x6f,
    0x75,0x6c,0x64,0x6e,
    0x27,0x74,0x20,0x6c,
    0x6f,0x61,0x64,0x20,
    0x79,0x6f,0x75,0x72,
    0x20,0x66,0x69,0x6c,
    0x65,0x21,0xb6,0x04,
    0xff,0x00
};

void load_p(int _de, int _hl) {
    // TODO: Implement
}

void patch_rom(BYTE *mem)
{
    /* patch the ROM here */
    mem[0x18a7]=0xed; /* for load_p */
    mem[0x18a8]=0xfc;
    mem[0x18a9]=0xc9;
    
    mem[0x1820]=0xed; /* for save_p */
    mem[0x1821]=0xfd;
    mem[0x1822]=0xc9;
}

void load_state() {
    // TODO: Implement
}

void save_state(){
    // TODO: Implement
}

void setup() {
    memcpy(mem, ace_rom, ace_rom_len);
    patch_rom(mem);
    memset(mem+8192, 0xff, sizeof(mem)-8192);
    keyboard_clear();

    memset(video_ram_old, 0xff, 768);

    // TODO: Implement tape list
}

static BOOL soundStarted = FALSE;
static long sound_tsstate = 0;

unsigned int in(int h, int l) {
    static int scans_left_before_next = 0;
    
    if (l==0xfe && h==0xfe && !scans_left_before_next) {
        static int x = 0;
        
        if (x) {
            keyboard_clear();
            if (x==0x0a)
                scans_left_before_next = 4;
            x = 0;
        }
        else {
            x = spooler_read_char();
            if (x) {
                keyboard_keypress(x);
                scans_left_before_next = 4;
            }
        }
        
    }
    else if (l==0xfe && h==0xfd && scans_left_before_next) {
        scans_left_before_next--;
    }

    if (l==0xfe) {        
        if (!soundStarted) {
            // TODO: Implement
        }
        soundStarted = FALSE;
        switch (h) {
            case 0xfe:
                return keyboard_get_keyport(0);
            case 0xfd:
                return keyboard_get_keyport(1);
            case 0xfb:
                return keyboard_get_keyport(2);
            case 0xf7:
                return keyboard_get_keyport(3);
            case 0xef:
                return keyboard_get_keyport(4);
            case 0xdf:
                return keyboard_get_keyport(5);
            case 0xbf:
                return keyboard_get_keyport(6);
            case 0x7f:
                return keyboard_get_keyport(7);
            default:
                return 255;
        }
    }
    return 255;
}

unsigned int out(int h, int l, int a) {
    if (l==0xfe) {
        // TODO: Implement
        soundStarted = TRUE;
        long dt = (long)tstates-sound_tsstate;
        if (dt < 0)
            dt += tsmax;
        int n = (float)dt/(tsmax*tsmaxfreq/22050);
        sound_tsstate = tstates;
    }

    return 0;
}

void fix_tstates() {
    static int first_time = 1;

    if (first_time) {
        first_time = 0;
        load_state();
        keyboard_clear();
    }

    static double t0 = 0.0;
    double t1 = rbxeTime();
    tstates -= tsmax;
    interrupted = 1;
    double delay = 0.020 - (t1 - t0);

    t0 = t1;

    if (delay > 0.0) {
        // TODO: Sleep
        //usleep(delay*1000);
    }
}

BOOL refresh(void);
int scrn_freq = 2;

void do_interrupt() {
    static int count=0;

    if (interrupted == 1) {
        interrupted = 2;

        /* only do refresh() every 1/Nth */
        count++;
        if (count >= scrn_freq) {
            count=0;
            //spooler_read();
            if (refresh())
                rbxeRun();
        }
    }
}

#define HSIZE 256
#define VSIZE 192
#define BYTES_PER_PIXEL 4

const pixel_info BLACK = {0, 0, 0, 255}, WHITE = {255, 255, 255, 255};

void set_pixel(int x, int y, unsigned int color) {
    if (color == 0) {
        rbxeSetPixel(x, y, BLACK);
    }
    else {
        //printf("%d,%d=%d\n", x, y, color);
        rbxeSetPixel(x, y, WHITE);
    }

    //memcpy(&image[(y*HSIZE+x)*BYTES_PER_PIXEL], &color, BYTES_PER_PIXEL);
}

void set_image_character(int x, int y, int inv, BYTE *charbmap) {
    int color;
    int charbmap_x, charbmap_y;
    BYTE charbmap_row;
    BYTE charbmap_row_mask;
    
    for (charbmap_y = 0; charbmap_y < 8; charbmap_y++) {
        charbmap_row = charbmap[charbmap_y];
        
        if (inv) {
            charbmap_row ^= 255;
        }

        charbmap_row_mask = 0x80;
        
        for (charbmap_x = 0; charbmap_x < 8; charbmap_x++) {
            color = !(charbmap_row & charbmap_row_mask) ? 0x00000000 : 0xffffffff;
            set_pixel(x*8+charbmap_x, y*8+charbmap_y, color);
            charbmap_row_mask >>= 1;
        }
    }
}

BOOL refresh() {
    BYTE *video_ram, *charset;
    int x, y, c;
    
    charset = mem+0x2c00;
    video_ram = mem+0x2400;
    
    if (!memcmp(video_ram, video_ram_old, sizeof(video_ram_old))) {
        return FALSE;
    }

    for (y=0; y<24; y++) {
        for (x=0; x<32; x++) {
            c = video_ram[x+y*32];
            video_ram_old[x+y*32] = c;
            set_image_character(x, y, c&0x80, charset + (c&0x7f)*8);
        }
    }
    
    return TRUE;
}

#define SCALE 2
#define FULLSCREEN FALSE
static int count = 0;

int main(void) {
    if (!rbxeStart("Jupiter Ace", HSIZE, VSIZE, SCALE, FULLSCREEN)) {
        return EXIT_FAILURE;
    }

    // Initialize jupiter
    setup();
    z80_init();
    refresh();
    
    while (1) {
        if (rbxeKeyPressed(KEY_ESCAPE)) {
            break;
        }

        //rbxeClear(255);

        //rbxePlotLine(0, 0, 100, 100, BLACK);

        // Emulator step
        z80_frame();

        // Drawing (TODO: Check timing)
        //refresh();
    }

    return rbxeEnd();
}

