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

void screen()
{
	float grey[] = {0.5f, 0.5f, 0.5f, 1.0f};

	glTranslatef(0.125f, 0.125f,-0.5f);
	glScalef(0.05f, 0.05f, 0.05f);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, grey);
	glBegin(GL_QUADS);
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glVertex3f( 1.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f,-1.0f, 0.0f);
		glVertex3f( 1.0f,-1.0f, 0.0f);
	glEnd();
}
