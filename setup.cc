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

static ovrHmd         hmd = 0;
static SDL_Window    *win = NULL;
static SDL_GLContext  ctx = 0;

static void cleanup()
{
	if(ctx) {
		SDL_GL_DeleteContext(ctx);
		print("Destroyed OpenGL context\n");
	}

	if(win) {
		SDL_DestroyWindow(win);
		print("Destroyed SDL window\n");
	}

	if(hmd) {
		ovrHmd_Destroy(hmd);
		print("Finalized head mounted display\n");
	}

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

	hmd = ovrHmd_Create(0);
	if(hmd)
		print("Initialized head mounted display: %s - %s: %s\n",
		      hmd->Manufacturer, hmd->ProductName,
		      hmd->HmdCaps & ovrHmdCap_ExtendDesktop ?
		      "extended display" : "direct HMD");
	else
		error("Failed to initialize head mounted display [%s]\n",
		      ovrHmd_GetLastError(NULL));

	win = SDL_CreateWindow("insight",
	                       SDL_WINDOWPOS_UNDEFINED,
	                       SDL_WINDOWPOS_UNDEFINED,
	                       hmd->Resolution.w / 2,
	                       hmd->Resolution.h / 2,
	                       SDL_WINDOW_OPENGL);
	if(win)
		print("Created SDL window of size %d x %d\n",
		      hmd->Resolution.w / 2,
		      hmd->Resolution.h / 2);
	else
		error("Failed to create SDL window\n");

	ctx = SDL_GL_CreateContext(win);
	if(ctx)
		print("Created OpenGL context\n");
	else
		error("Failed to create OpenGL context\n");
}
