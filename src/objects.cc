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

namespace control {
	float theta = 90;
	float phi   = 0;
}

void objects(unsigned vol)
{
	float pos[][4] = {{-8.0f, 2.0f,10.0f, 1.0f}, {0.0f,15.0f, 0.0f, 1.0f}};
	float col[][4] = {{ 0.8f, 0.8f, 0.8f, 1.0f}, {0.4f, 0.3f, 0.3f, 1.0f}};
	for(int i = 0; i < 2; ++i) {
		glLightfv(GL_LIGHT0+i, GL_POSITION, pos[i]);
		glLightfv(GL_LIGHT0+i, GL_DIFFUSE,  col[i]);
	}

	if(control::sitting)
		glTranslatef(0.0f, 1.675f, -1.0f);
	else
		glTranslatef(0.0f, 2.0f, 0.0f);
	glScalef(0.5f, 0.5f, 0.5f);

	glBegin(GL_LINE_STRIP);
		glVertex3f( 1, 1, 1);
		glVertex3f(-1, 1, 1);
		glVertex3f(-1,-1, 1);
		glVertex3f(-1,-1,-1);
	glEnd();
	glBegin(GL_LINE_STRIP);
		glVertex3f(-1, 1, 1);
		glVertex3f(-1, 1,-1);
		glVertex3f(-1,-1,-1);
		glVertex3f( 1,-1,-1);
	glEnd();
	glBegin(GL_LINE_STRIP);
		glVertex3f(-1, 1,-1);
		glVertex3f( 1, 1,-1);
		glVertex3f( 1,-1,-1);
		glVertex3f( 1,-1, 1);
	glEnd();
	glBegin(GL_LINE_STRIP);
		glVertex3f( 1, 1,-1);
		glVertex3f( 1, 1, 1);
		glVertex3f( 1,-1, 1);
		glVertex3f(-1,-1, 1);
	glEnd();

	if(!vol)
		return;

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 1.0f/128);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_3D);
	glBindTexture(GL_TEXTURE_3D, vol);
	for(int i = 0; i < 128; ++i) {
		float g   = 2 * i / 127.0 - 1;
		float v[] = {1,  1, -1, -1,
		             1, -1, -1,  1,
		             g,  g,  g,  g,
		             1,  1,  1,  1};
		glPushMatrix();
		glLoadMatrixf(v);
		glScalef(0.25f, 0.25f, 0.25f);
		glRotatef(control::theta - 90, 0, 1, 0);
		glRotatef(control::phi,        1, 0, 0);
		glGetFloatv(GL_MODELVIEW_MATRIX, v);
		glPopMatrix();

		glBegin(GL_QUADS);
		glTexCoord3f(v[0]+0.5f, v[4]+0.5f, v[ 8]+0.5f); glVertex3f( 1, 1, g);
		glTexCoord3f(v[1]+0.5f, v[5]+0.5f, v[ 9]+0.5f); glVertex3f(-1, 1, g);
		glTexCoord3f(v[2]+0.5f, v[6]+0.5f, v[10]+0.5f); glVertex3f(-1,-1, g);
		glTexCoord3f(v[3]+0.5f, v[7]+0.5f, v[11]+0.5f); glVertex3f( 1,-1, g);
		glEnd();
	}
	glBindTexture(GL_TEXTURE_3D, 0);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_3D);
}