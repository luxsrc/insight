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
#include "vol.h"
#include <cstdio>
#include <cstdint>

unsigned mkvol(const char *name)
{
	FILE *file = fopen(name, "rb");
	if(!file)
		return 0;

	int n;
	fread(&n, sizeof(int), 1, file);

        uint8_t *data = (uint8_t *)malloc(n * n * n * 4);
        fread(data, 1, n * n * n * 4, file);
        fclose(file);

        unsigned vol;
	glGenTextures(1, &vol);
	glBindTexture(GL_TEXTURE_3D, vol);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, n, n, n, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, data);
        free(data);

        return vol;
}

void rmvol(unsigned vol)
{
	glDeleteTextures(1, &vol);
}
