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

#ifndef RB_PIXEL_ENGINE_ARRAYLIST_H
#define RB_PIXEL_ENGINE_ARRAYLIST_H

typedef struct array_list {
	int len;
	int capacity;
	int item_size;
	void *items;
} array_list;

array_list* array_list_create(int item_size, int initial_capacity);
int array_list_append(array_list *list, void *item);
void* array_list_get(array_list *list, int index);
int array_list_remove(array_list *list, int index);

#ifdef RBXE_ENGINE

array_list *array_list_create(int item_size, int initial_capacity) {
    array_list *list = malloc(sizeof(array_list));

    if (!list) {
	    fprintf(stderr, "Could not allocate memory for array_list\n");
        return NULL;
    }

    list->item_size = item_size;
    list->capacity = initial_capacity;
    list->len = 0;
    list->items = malloc(item_size * initial_capacity);

    if (!list->items) {
	    fprintf(stderr, "Could not allocate memory for array_list\n");
        return NULL;
    }

    return list;
}

int array_list_append(array_list *list, void *item) {
	if (list->len == list->capacity) {
		list->capacity = list->capacity > 0 ? list->capacity * 2 : 1;
		void *items = realloc(list->items, list->item_size * list->capacity);

		if (!items) {
			fprintf(stderr, "Could not allocate memory for array_list\n");
            return -1;
        }

		list->items = items;
	}

	int index = list->len++;

	memcpy((unsigned char*)list->items + index * list->item_size, item, list->item_size);

	return index;
}

void* array_list_get(array_list *list, int index) {
	if (index >= list->len) {
		fprintf(stderr, "Index out of bounds\n");
    }

	return (unsigned char*)list->items + index * list->item_size;
}

int array_list_remove(array_list *list, int index) {
	if (list->len == 0) {
		fprintf(stderr, "List is empty\n");
        return 0;
    }
	if (index >= list->len) {
		fprintf(stderr, "Index out of bounds\n");
        return 0;
    }

	if (list->len == 1) {
		list->len = 0;

		return 0;
	}

	--list->len;

	unsigned char *item_ptr = (unsigned char*)list->items + index * list->item_size;
	unsigned char *end_ptr = (unsigned char*)list->items + list->len * list->item_size;
	memcpy(item_ptr, end_ptr, list->item_size);

	return 1;
}

#endif /* RBXE_ENGINE */
#endif /* RB_PIXEL_ENGINE_ARRAYLIST_H */
