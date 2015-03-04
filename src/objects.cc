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

void objects()
{
	float pos[][4] = {{-8.0f, 2.0f,10.0f, 1.0f}, {0.0f,15.0f, 0.0f, 1.0f}};
	float col[][4] = {{ 0.8f, 0.8f, 0.8f, 1.0f}, {0.4f, 0.3f, 0.3f, 1.0f}};
	for(int i = 0; i < 2; ++i) {
		glLightfv(GL_LIGHT0+i, GL_POSITION, pos[i]);
		glLightfv(GL_LIGHT0+i, GL_DIFFUSE,  col[i]);
	}

	glTranslatef(0.0f, 1.0f,-1.0f);

	glBegin(GL_LINE_STRIP);
		glVertex3f( 0.5f, 0.5f, 0.5f);
		glVertex3f(-0.5f, 0.5f, 0.5f);
		glVertex3f(-0.5f,-0.5f, 0.5f);
		glVertex3f(-0.5f,-0.5f,-0.5f);
	glEnd();
	glBegin(GL_LINE_STRIP);
		glVertex3f(-0.5f, 0.5f, 0.5f);
		glVertex3f(-0.5f, 0.5f,-0.5f);
		glVertex3f(-0.5f,-0.5f,-0.5f);
		glVertex3f( 0.5f,-0.5f,-0.5f);
	glEnd();
	glBegin(GL_LINE_STRIP);
		glVertex3f(-0.5f, 0.5f,-0.5f);
		glVertex3f( 0.5f, 0.5f,-0.5f);
		glVertex3f( 0.5f,-0.5f,-0.5f);
		glVertex3f( 0.5f,-0.5f, 0.5f);
	glEnd();
	glBegin(GL_LINE_STRIP);
		glVertex3f( 0.5f, 0.5f,-0.5f);
		glVertex3f( 0.5f, 0.5f, 0.5f);
		glVertex3f( 0.5f,-0.5f, 0.5f);
		glVertex3f(-0.5f,-0.5f, 0.5f);
	glEnd();
}
