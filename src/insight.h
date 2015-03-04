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

#ifndef INSIGHT_H
#define INSIGHT_H

#include <cstddef> // must include before OVR_CAPI_GL.h to avoid C++ error

#include <SDL2/SDL.h>
#include <OVR_CAPI.h>
#include <OVR_CAPI_GL.h>

extern unsigned         fbo;
extern ovrSizei         bsz;
extern ovrEyeRenderDesc rdesc[];
extern ovrGLTexture     gltex[];

extern ovrHmd        mkhmd();
extern void          rmhmd(ovrHmd);
extern SDL_Window   *mkwin(ovrHmd);
extern void          rmwin(SDL_Window *);
extern SDL_GLContext mkglc(ovrHmd, SDL_Window *);
extern void          rmglc(SDL_GLContext);

extern ovrHmd setup();

extern void error(const char *, ...);
extern void print(const char *, ...);

extern bool handle(SDL_Event &);
extern void display(ovrHmd);

#endif // INSIGHT_H
