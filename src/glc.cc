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

SDL_GLContext mkglc(SDL_Window *win)
{
	SDL_GLContext ctx = SDL_GL_CreateContext(win);
	if(ctx)
		print("Created OpenGL context\n");
	return ctx;
}

void rmglc(SDL_GLContext glc)
{
	SDL_GL_DeleteContext(glc);
	print("Destroyed OpenGL context\n");
}
