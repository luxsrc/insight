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
#include <cerrno>
#include <cstring>

ovrHmd hmd = 0;

static SDL_Window    *win = NULL;
static SDL_GLContext  glc = 0;

static void cleanup()
{
	if(glc)	rmglc(glc);
	if(win) rmwin(win);
	if(hmd) rmhmd(hmd);

	SDL_Quit();
	ovr_Shutdown();
}

void setup()
{
	if(atexit(cleanup))
		error("Failed to register cleanup function [%s]\n",
		      strerror(errno));

	ovr_Initialize();
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

	if(!(hmd = mkhmd()))
		error("Failed to initialize head mounted display [%s]\n",
		      ovrHmd_GetLastError(NULL));

	if(!(win = mkwin(hmd)))
		error("Failed to create SDL window\n");

	if(!(glc = mkglc(hmd, win)))
		error("Failed to create OpenGL context\n");
}
