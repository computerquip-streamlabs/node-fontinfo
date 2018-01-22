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

#include <nan.h>
#include <locale>
#include <codecvt>
#include "fontinfo/fontinfo.h"
#include "fontinfo/endian.h"

namespace fontinfo {

namespace {

v8::Local<v8::String> StringFromFontString(font_info_string *name)
{
	int buffer_length = name->length / 2;
	uint16_t *buffer = new uint16_t[buffer_length];
	memcpy(buffer, name->buffer, name->length);
	
	/* Flip from BE to host order */
	for (int i = 0; i < name->length / 2; ++i) {
		buffer[i] = be16toh(((uint16_t*)name->buffer)[i]);
	}
	
	v8::Local<v8::String> result = 
		Nan::New<v8::String>(buffer, buffer_length).ToLocalChecked();
	
	delete[] buffer;
	
	return result;
}

void SetByString(v8::Local<v8::Object> object, const char* key, v8::Local<v8::Value> value)
{
	Nan::Set(object, Nan::New<v8::String>(key).ToLocalChecked(), value);
}

}

NAN_METHOD(getFontInfo) {
	/* I can't wait for N-API */
	if (!info[0]->IsString()) {
		Nan::ThrowTypeError("getFontInfo: Argument 1 - expected string");
		return;
	}
	
	Nan::Utf8String filepath(info[0]);
	
	font_info *f_info = font_info_create(*filepath);
	
	if (!f_info) {
		info.GetReturnValue().Set(Nan::Null());
		return;
	}
	
	/* We now have UTF16 and the size of the buffer, let V8 deal with the rest */
	v8::Local<v8::Object> result = Nan::New<v8::Object>();
	SetByString(result, "family_name", StringFromFontString(&f_info->family_name));
	SetByString(result, "subfamily_name", StringFromFontString(&f_info->subfamily_name));
	SetByString(result, "italic", Nan::New<v8::Boolean>(f_info->italic);
	SetByString(result, "bold", Nan::New<v8::Boolean>(f_info->bold);
	
	font_info_destroy(f_info);
	
	info.GetReturnValue().Set(result);
}

NAN_MODULE_INIT(init_module) {
	NAN_EXPORT(target, getFontInfo);
}

}

const char *module_name = "node-fontinfo";
NODE_MODULE(module_name, fontinfo::init_module)