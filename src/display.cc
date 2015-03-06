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

void display(ovrHmd hmd, unsigned img, unsigned vol)
{
	ovrVector3f offset[2] = {global::rdesc[0].HmdToEyeViewOffset,
	                         global::rdesc[1].HmdToEyeViewOffset};
	ovrPosef pose[2]; ovrHmd_GetEyePoses(hmd, 0, offset, pose, NULL);

	ovrFrameTiming t = ovrHmd_BeginFrame(hmd, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, global::fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for(int i = 0; i < 2; ++i) {
		ovrEyeType eye = hmd->EyeRenderOrder[i];
		glViewport(eye == ovrEye_Left ? 0 : global::bsz.w / 2, 0, global::bsz.w / 2, global::bsz.h);

		glMatrixMode(GL_PROJECTION);
		ovrFovPort  fov  = hmd->DefaultEyeFov[eye];
		ovrMatrix4f proj = ovrMatrix4f_Projection(fov, 0.01f, 1000.0f, true);
		glLoadTransposeMatrixf(proj.M[0]);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		float x = global::rdesc[eye].HmdToEyeViewOffset.x;
		float y = global::rdesc[eye].HmdToEyeViewOffset.y;
		float z = global::rdesc[eye].HmdToEyeViewOffset.z;
		glTranslatef(x, y, z);

		glasses(img, vol); // head-mounted models

		ovrMatrix4f Rij;
		quat_to_matrix(&pose[eye].Orientation.x, Rij.M[0]);
		glMultMatrixf(Rij.M[0]);

		x += pose[eye].Position.x;
		y += pose[eye].Position.y + ovrHmd_GetFloat(hmd, OVR_KEY_EYE_HEIGHT, 1.675);
		z += pose[eye].Position.z;
		glTranslatef(-x, -y, -z);

		objects(img, vol); // spatially fixed objects
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	ovrHmd_EndFrame(hmd, pose, &global::gltex[0].Texture);

	const char spinner[] = "-/|\\";
	static unsigned long count = 0; ++count;
	print("\r%lu %c", count, spinner[count % 4]);

	if(t.DeltaSeconds > 0.0)
		print("%6.2f fps, ", 1.0 / t.DeltaSeconds);

	static double longest = 0.0;
	if(longest < t.DeltaSeconds && count > 100)
		longest = t.DeltaSeconds;

	if(longest > 0.0)
		print("[slowest: %.2f fps]", 1.0 / longest);

	fflush(stdout);
}
