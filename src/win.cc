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

SDL_Window *mkwin(ovrHmd hmd)
{
	SDL_Window *win = SDL_CreateWindow("insight",
	                                   hmd->WindowsPos.x,
	                                   hmd->WindowsPos.y,
	                                   hmd->Resolution.w,
	                                   hmd->Resolution.h,
	                                   SDL_WINDOW_OPENGL);
	if(!win)
		return win; // NULL with correct type

	SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN_DESKTOP);
	print("Created SDL window of size %d x %d\n",
	      hmd->Resolution.w,
	      hmd->Resolution.h);
	return win;
}

void rmwin(SDL_Window *win)
{
	SDL_DestroyWindow(win);
	print("Destroyed SDL window\n");
}
