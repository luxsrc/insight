// Copyright (C) 2015 Chi-kwan Chan
// Copyright (C) 2015 Steward Observatory
//
// This file is part of insight.
//
// Insight is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Insight is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with insight.  If not, see <http://www.gnu.org/licenses/>.

#include "insight.h"
#include "img.h"
#include <cstdio>
#include <cstdint>
#include <jpeglib.h>

unsigned mkimg(const char *name)
{
	FILE *file = fopen(name, "rb");
	if(!file)
		return 0;

	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr         jerr;
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, file);
	jpeg_read_header(&cinfo, false);
	jpeg_start_decompress(&cinfo);

	uint32_t w = cinfo.image_width;
	uint32_t h = cinfo.image_height;
	uint8_t  n = cinfo.num_components;
	uint8_t *d = (uint8_t *)malloc(w * h * n);
	while(cinfo.output_scanline < h) {
		uint8_t *p = d + cinfo.output_scanline * w * n;
		jpeg_read_scanlines(&cinfo, &p, 1);
	}

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	fclose(file);

	unsigned img;
	glGenTextures(1, &img);
	glBindTexture(GL_TEXTURE_2D, img);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0,
	             GL_RGB, GL_UNSIGNED_BYTE, d);

	free(d);

	return img;
}

void rmimg(unsigned img)
{
	glDeleteTextures(1, &img);
}
