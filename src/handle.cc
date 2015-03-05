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

bool handle(SDL_Event &e)
{
	switch(e.type) {
	case SDL_QUIT:
		return 1;
	case SDL_KEYUP:
		switch(e.key.keysym.sym) {
		case 27:
			return 1;
		case 'f':
			control::fixed = !control::fixed;
			break;
		case 'm':
			control::mounted = !control::mounted;
			break;
		}
	case SDL_KEYDOWN:
		switch(e.key.keysym.sym) {
		case SDLK_LEFT:
			control::phi += 1.0f;
			break;
		case SDLK_RIGHT:
			control::phi -= 1.0f;
			break;
		case SDLK_UP:
			control::theta += 1.0f;
			if(control::theta > 180) control::theta = 180;
			break;
		case SDLK_DOWN:
			control::theta -= 1.0f;
			if(control::theta < 0) control::theta = 0;
			break;
		}
	}
	return 0;
}
