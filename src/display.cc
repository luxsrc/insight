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

void display()
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

	// TODO: OpenGL rendering

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	ovrHmd_EndFrame(hmd, pose, &gltex[0].Texture);
}
