/*
 * RBXE | The Pixel Engine by Roger Boesch
 *
 * Copyright (C) 2024 Roger Boesch
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef RB_PIXEL_ENGINE_GIF_H
#define RB_PIXEL_ENGINE_GIF_H

unsigned char* rbxeLoadGIF(char const* filename, int* width, int* height, int* palcount, unsigned char palette[768]);

#ifdef RBXE_ENGINE

#include <gif_load.h>

struct rbxe_gif_load_context_t {
    int width;
    int height;
    uint8_t* pixels;
    int palcount;
    uint8_t palette[768];
};

static void rbxeLoadGIFFrame(void* data, struct GIF_WHDR* whdr) {
    struct rbxe_gif_load_context_t* context = (struct rbxe_gif_load_context_t*) data;

    if (context->pixels) {
        return;
    }
   
    context->width = whdr->xdim;
    context->height = whdr->ydim;
    context->palcount = whdr->clrs;
    memcpy(context->palette, whdr->cpal, whdr->clrs * 3);

    context->pixels = (uint8_t*)malloc( whdr->xdim * whdr->ydim);
    memset(context->pixels, 0, whdr->xdim * whdr->ydim);

    uint32_t ddst = (uint32_t)(whdr->xdim * whdr->fryo + whdr->frxo);
    uint32_t iter = (whdr->intr) ? 0 : 4;
    uint32_t ifin = (!iter) ? 4 : 5; 

    for (uint32_t dsrc = (uint32_t)-1; iter < ifin; ++iter) {
        for (uint32_t yoff = 16U >> ((iter > 1)? iter : 1), y = (8 >> iter) & 7; y < (uint32_t)whdr->fryd; y += yoff) {
            for( uint32_t x = 0; x < (uint32_t)whdr->frxd; x++) {
                if (whdr->tran != (long)whdr->bptr[++dsrc]) {
                    context->pixels[(uint32_t)whdr->xdim * y + x + ddst] = whdr->bptr[dsrc];
                }
            }
        }
    }
}

unsigned char* rbxeLoadGIF( char const* filename, int* width, int* height, int* palcount, unsigned char palette[768]) {
    FILE* fp = fopen(filename, "rb");
    if (!fp) return NULL;

    fseek(fp, 0, SEEK_END);
    size_t sz = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    uint8_t* data = (uint8_t*) malloc(sz);
    fread(data, 1, sz, fp);
    fclose(fp);

    struct rbxe_gif_load_context_t context;
    memset(&context, 0, sizeof(context));

    GIF_Load(data, (long)sz, rbxeLoadGIFFrame, NULL, &context, 0);
    free(data);

    if (width) {
        *width = context.width;
    }

    if (height) {
        *height = context.height;
    }

    if (palcount) {
        *palcount = context.palcount;
    }

    if (palette) {
        for( int i = 0; i < context.palcount * 3; ++i) {
            palette[ i ] = context.palette[ i ] >> 2;
        }
    }

    return context.pixels;
}

#endif /* RBXE_GIF */
#endif /* RB_PIXEL_ENGINE_MATH_H */
