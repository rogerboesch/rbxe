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

#ifndef RB_GAME_PALETTE_ZX_H
#define RB_GAME_PALETTE_ZX_H

#include <rbxe.h>

#define PAL_COLOR_BLACK        0
#define PAL_COLOR_BLUE         1
#define PAL_COLOR_RED          2
#define PAL_COLOR_MAGENTA      3
#define PAL_COLOR_GREEN        4
#define PAL_COLOR_CYAN         5
#define PAL_COLOR_YELLOW       6
#define PAL_COLOR_WHITE        7
#define PAL_COLOR_BLACK_LT     8
#define PAL_COLOR_BLUE_LT      9
#define PAL_COLOR_RED_LT      10
#define PAL_COLOR_MAGENTA_LT  11
#define PAL_COLOR_GREEN_LT    12
#define PAL_COLOR_CYAN_LT     13
#define PAL_COLOR_YELLOW_LT   14
#define PAL_COLOR_WHITE_LT    15

Pixel gameGetPalette(int number);

#ifdef GAME_PALETTE

#define MAX_PALETTE_ENTRY 16

Pixel game_palette[] = {
	// Bright 0
	{0x00, 0x00, 0x00, 0xff}, // Black
	{0x00, 0x22, 0xd7, 0xff}, // Blue
	{0xd6, 0x28, 0x16, 0xff}, // Red
	{0xd4, 0x33, 0xc7, 0xff}, // Magenta
	{0x00, 0xc5, 0x25, 0xff}, // Green
	{0x00, 0xc7, 0xc9, 0xff}, // Cyan
	{0xcc, 0xc8, 0x2a, 0xff}, // Yellow
	{0xca, 0xca, 0xca, 0xff}, // White

	// Bright 1
	{0x00, 0x00, 0x00, 0xff}, // Black
	{0x00, 0x2b, 0xfb, 0xff}, // Blue
	{0xff, 0x33, 0x1c, 0xff}, // Red
	{0xff, 0x40, 0xfc, 0xff}, // Magenta
	{0x00, 0xf9, 0x2f, 0xff}, // Green
	{0x00, 0xfb, 0xfe, 0xff}, // Cyan
	{0xff, 0xfc, 0x36, 0xff}, // Yellow
	{0xff, 0xff, 0xff, 0xff}  // White
};

Pixel gameGetPalette(int number) {
    if (number < 0 || number >= MAX_PALETTE_ENTRY) {
        fprintf(stderr, "Palette color %d is not available. (%d-%d)", number, 0, MAX_PALETTE_ENTRY);
        return game_palette[0];
    }

    return game_palette[number];
}

#endif /* GAME_PALETTE */
#endif /* RB_GAME_PALETTE_ZX_H */
