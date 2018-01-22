#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_TRUETYPE_IDS_H
#include FT_SFNT_NAMES_H

#include "fontinfo.h"

#define container_of(ptr, type, member) ((type *)((char *)(1 ? (ptr) : &((type *)0)->member) - offsetof(type, member)))

/* We could use multiple backends at compile time
 * For now, we require freetype since it's the only
 * decent library for fetching font info that I know of */

struct freetype_font_info {
	FT_Library ft_library;
	FT_Face ft_face;
	struct font_info external;
};

struct font_info *font_info_create(const char *path)
{
	FT_Error ft_error;
	
	struct freetype_font_info *info = 
		calloc(1, sizeof(struct freetype_font_info));
		
	struct font_info *external = &info->external;

	ft_error = FT_Init_FreeType(&info->ft_library);
	
	if (ft_error) goto fail;
	
	ft_error = FT_New_Face(info->ft_library, path, 0, &info->ft_face);
	
	if (ft_error) goto fail;
	
	FT_UInt name_count = FT_Get_Sfnt_Name_Count(info->ft_face);
	
	#define ASSIGN_STRING(member) \
		external->member.buffer = name_info.string; \
		external->member.length = name_info.string_len;
	
	for (unsigned int i = 0; i < name_count; ++i) {
		FT_SfntName name_info;
		
		ft_error = FT_Get_Sfnt_Name(info->ft_face, i, &name_info);
		
		if (ft_error) continue;
		
		switch(name_info.name_id) {
		case TT_NAME_ID_FONT_FAMILY:
			ASSIGN_STRING(family_name)
			break;
		case TT_NAME_ID_FONT_SUBFAMILY:
			ASSIGN_STRING(subfamily_name)
			break;
		default:
			continue;
		}
	}
	
	#undef ASSIGN_STRING
	
	external->italic = info->ft_face->style_flags & FT_STYLE_FLAG_ITALIC;
	external->bold   = info->ft_face->style_flags & FT_STYLE_FLAG_BOLD;
	
	goto finish;
	
fail:
	free(info);
	return 0;
	
finish:
	return external;
}

void font_info_destroy(struct font_info *external_info)
{
	struct freetype_font_info *info = 
		container_of(external_info, struct freetype_font_info, external);
	
	FT_Done_Face(info->ft_face);
	FT_Done_FreeType(info->ft_library);
	free(info);
}