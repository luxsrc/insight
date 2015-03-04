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

/* Convert a quaternion to a rotation matrix, see the example by
   John Tsiombikas <nuclear@member.fsf.org> */
static inline void quat_to_matrix(const float *q, float *M)
{
	M[ 0] = 1.0f - 2.0f * (q[1] * q[1] + q[2] * q[2]);
	M[ 1] =        2.0f * (q[0] * q[1] - q[3] * q[2]);
	M[ 2] =        2.0f * (q[2] * q[0] + q[3] * q[1]);
	M[ 3] = 0.0f;

	M[ 4] =        2.0f * (q[0] * q[1] + q[3] * q[2]);
	M[ 5] = 1.0f - 2.0f * (q[0] * q[0] + q[2] * q[2]);
	M[ 6] =        2.0f * (q[1] * q[2] - q[3] * q[0]);
	M[ 7] = 0.0f;

	M[ 8] =        2.0f * (q[2] * q[0] - q[3] * q[1]);
	M[ 9] =        2.0f * (q[1] * q[2] + q[3] * q[0]);
	M[10] = 1.0f - 2.0f * (q[0] * q[0] + q[1] * q[1]);
	M[11] = 0.0f;

	M[12] = 0.0f;
	M[13] = 0.0f;
	M[14] = 0.0f;
	M[15] = 1.0f;
}

static void draw()
{
	float lpos[][4] = {{-8.0f, 2.0f,10.0f, 1.0f}, {0.0f,15.0f, 0.0f, 1.0f}};
	float lcol[][4] = {{ 0.8f, 0.8f, 0.8f, 1.0f}, {0.4f, 0.3f, 0.3f, 1.0f}};
	for(int i = 0; i < 2; ++i) {
		glLightfv(GL_LIGHT0+i, GL_POSITION, lpos[i]);
		glLightfv(GL_LIGHT0+i, GL_DIFFUSE,  lcol[i]);
	}

	glTranslatef(0.0f,1.0f,-1.0f);

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

void display(ovrHmd hmd)
{
	static unsigned long count = 0;
	printf("%lu\n", count++);

	ovrVector3f offset[2] = {rdesc[0].HmdToEyeViewOffset,
	                         rdesc[1].HmdToEyeViewOffset};
	ovrPosef pose[2];
	ovrHmd_GetEyePoses(hmd, 0, offset, pose, NULL);

	ovrHmd_BeginFrame(hmd, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for(int i = 0; i < 2; ++i) {
		ovrEyeType eye = hmd->EyeRenderOrder[i];

		glViewport(eye == ovrEye_Left ? 0 : bsz.w / 2, 0, bsz.w / 2, bsz.h);

		ovrMatrix4f proj = ovrMatrix4f_Projection(hmd->DefaultEyeFov[eye], 0.5, 500.0, 1);
		glMatrixMode(GL_PROJECTION);
		glLoadTransposeMatrixf(proj.M[0]);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(rdesc[eye].HmdToEyeViewOffset.x,
		             rdesc[eye].HmdToEyeViewOffset.y,
		             rdesc[eye].HmdToEyeViewOffset.z);

		float Rij[16];
		quat_to_matrix(&pose[eye].Orientation.x, Rij);
		glMultMatrixf(Rij);
		glTranslatef(-pose[eye].Position.x, -pose[eye].Position.y, -pose[eye].Position.z);
		glTranslatef(0, -ovrHmd_GetFloat(hmd, OVR_KEY_EYE_HEIGHT, 1.65), 0);

		draw();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	ovrHmd_EndFrame(hmd, pose, &gltex[0].Texture);
}
