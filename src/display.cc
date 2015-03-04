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

void display(ovrHmd hmd, void (*fixed)(), void (*mounted)())
{
	static unsigned long count = 0;
	printf("%lu\n", count++);

	ovrVector3f offset[2] = {global::rdesc[0].HmdToEyeViewOffset,
	                         global::rdesc[1].HmdToEyeViewOffset};
	ovrPosef pose[2];
	ovrHmd_GetEyePoses(hmd, 0, offset, pose, NULL);

	ovrHmd_BeginFrame(hmd, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, global::fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for(int i = 0; i < 2; ++i) {
		ovrEyeType eye = hmd->EyeRenderOrder[i];

		glViewport(eye == ovrEye_Left ? 0 : global::bsz.w / 2, 0,
		           global::bsz.w / 2, global::bsz.h);

		ovrMatrix4f proj = ovrMatrix4f_Projection(hmd->DefaultEyeFov[eye], 0.5, 500.0, 1);
		glMatrixMode(GL_PROJECTION);
		glLoadTransposeMatrixf(proj.M[0]);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(global::rdesc[eye].HmdToEyeViewOffset.x,
		             global::rdesc[eye].HmdToEyeViewOffset.y,
		             global::rdesc[eye].HmdToEyeViewOffset.z);

		glPushMatrix();
		mounted();
		glPopMatrix();

		float Rij[16];
		quat_to_matrix(&pose[eye].Orientation.x, Rij);
		glMultMatrixf(Rij);
		glTranslatef(-pose[eye].Position.x, -pose[eye].Position.y, -pose[eye].Position.z);
		glTranslatef(0, -ovrHmd_GetFloat(hmd, OVR_KEY_EYE_HEIGHT, 1.65), 0);

		glPushMatrix();
		fixed();
		glPopMatrix();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	ovrHmd_EndFrame(hmd, pose, &global::gltex[0].Texture);
}
