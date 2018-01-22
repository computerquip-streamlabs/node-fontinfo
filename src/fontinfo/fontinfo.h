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

#include <stdbool.h>

struct font_info_string {
	/* A UTF-16BE encoded byte string
	 * It is not null-terminated */
	char *buffer;
	int length;
};

struct font_info {
	struct font_info_string family_name;
	struct font_info_string subfamily_name;
	bool italic;
	bool bold;
};

#ifdef __cplusplus
extern "C" {
#endif

/* Fetches the structure given the path */
struct font_info *font_info_create(const char* path);
void font_info_destroy(struct font_info* info);

#ifdef __cplusplus
}
#endif