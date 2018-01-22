/*
    Copyright (C) 2018  Zachary Lund

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fontinfo/fontinfo.h>
#include <fontinfo/endian.h>

void print_name(struct font_info_string *name, const char *message)
{
	/* The freetype string is UTF-16BE encoded */
	
	/* We're not guaranteed to control the memory 
	 * so make a copy instead. */
	wchar_t *buffer = malloc(name->length);
	int buffer_length = name->length / 2;
	memcpy(buffer, name->buffer, name->length);
	
	/* Flip from BE to host order */
	for (int i = 0; i < name->length / 2; ++i) {
		buffer[i] = be16toh(((unsigned short*)name->buffer)[i]);
	}
	
	printf("%s: %.*ls\n", message, buffer_length, buffer);
	
	free(buffer);
}

int main(void)
{
	const char *font_path = "./ZillaSlab Highlight-Regular.ttf";
	
	struct font_info *info =
		font_info_create(font_path);

	if (!info) {
		fprintf(stderr, "Failed to fetch font info for %s\n", font_path);
		return;
	}
	
	print_name(&info->family_name, "Font Family Name");
	print_name(&info->subfamily_name, "Font Subfamily Name");
	
	font_info_destroy(info);
}