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

#include <GL/glew.h> // must be included before gl.h in OVR_CAPI_GL.h
#include "insight.h"

unsigned         fbo;
ovrSizei         bsz;
ovrEyeRenderDesc rdesc[2];
ovrGLTexture     gltex[2];

static inline unsigned clp2(unsigned x)
{
	x -= 1;
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
	return x + 1;
}

SDL_GLContext mkglc(ovrHmd hmd, SDL_Window *win)
{
	SDL_GLContext ctx = SDL_GL_CreateContext(win);
	if(!ctx)
		return ctx; // NULL with correct type

	ovrSizei lsz = ovrHmd_GetFovTextureSize(hmd, ovrEye_Left,  hmd->DefaultEyeFov[ovrEye_Left],  1.0);
	ovrSizei rsz = ovrHmd_GetFovTextureSize(hmd, ovrEye_Right, hmd->DefaultEyeFov[ovrEye_Right], 1.0);
	ovrSizei tsz = {clp2(bsz.w = lsz.w + rsz.w), clp2(bsz.h = lsz.h > rsz.h ? lsz.h : rsz.h)};

	glewInit();

	// Generate framebuffer object name
	// unsigned fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// Generate renderbuffer object name
	unsigned rbf;
	glGenRenderbuffers(1, &rbf);
	glBindRenderbuffer(GL_RENDERBUFFER, rbf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, tsz.w, tsz.h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbf);

	// Generate texture name
	unsigned tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tsz.w, tsz.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

	// Check framebuffer completion; unbind it if done
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	else
		error("Failed to complete framebuffer\n");

	// Fill out gltex as an output, will be used in ovrHmd_EndFrame()
	for(int i = 0; i < 2; ++i) {
		gltex[i].OGL.Header.API = ovrRenderAPI_OpenGL;
		gltex[i].OGL.Header.TextureSize.w = tsz.w;
		gltex[i].OGL.Header.TextureSize.h = tsz.h;
		gltex[i].OGL.Header.RenderViewport.Pos.x = !i ? 0 : bsz.w / 2.0;
		gltex[i].OGL.Header.RenderViewport.Pos.y = 0;
		gltex[i].OGL.Header.RenderViewport.Size.w = bsz.w / 2.0;
		gltex[i].OGL.Header.RenderViewport.Size.h = bsz.h;
		gltex[i].OGL.TexId = tex;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);

	glClearColor(0.1, 0.1, 0.1, 1);

	print("Created OpenGL context with window size %d x %d and texture size %d x %d\n",
	      bsz.w, bsz.h, tsz.w, tsz.h);
	return ctx;
}

void rmglc(SDL_GLContext glc)
{
	SDL_GL_DeleteContext(glc);
	print("Destroyed OpenGL context\n");
}
