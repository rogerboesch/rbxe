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

#ifndef RB_PIXEL_ENGINE_MATH_H
#define RB_PIXEL_ENGINE_MATH_H

typedef struct vec2 {
    float x, y;
} vec2;

static vec2 vec2_create(float x, float y);

#ifdef RBXE_ENGINE

static vec2 vec2_create(float x, float y) {
    vec2 p;
    p.x = x;
    p.y = y;

    return p;
}

#endif /* RBXE_ENGINE */
#endif /* RB_PIXEL_ENGINE_MATH_H */
