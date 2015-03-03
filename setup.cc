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

#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <cstring>

#include <OVR_CAPI.h>
#include <SDL2/SDL.h>

static SDL_Window    *win = NULL;
static SDL_GLContext  ctx = 0;

static void cleanup()
{
	if(ctx)
		SDL_GL_DeleteContext(ctx);

	if(win)
		SDL_DestroyWindow(win);

	SDL_Quit();
	ovr_Shutdown();
}

void setup()
{
	if(atexit(cleanup))
		error("ERROR: failed to register cleanup function [%s]\n",
		      strerror(errno));

	ovr_Initialize();
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

	win = SDL_CreateWindow("insight",
	                       SDL_WINDOWPOS_UNDEFINED,
	                       SDL_WINDOWPOS_UNDEFINED,
	                       960, 540,
	                       SDL_WINDOW_OPENGL);
	if(!win)
		error("ERROR: failed to create window\n");

	ctx = SDL_GL_CreateContext(win);
	if(!ctx)
		error("ERROR: failed to create OpenGL context\n");
}
