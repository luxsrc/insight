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

namespace control {
	bool sitting  = true;
	bool pressing = false;
	int  key      = 0;
};

static bool hsw = true;

bool handle(ovrHmd hmd, SDL_Event &e)
{
	switch(e.type) {
	case SDL_QUIT:
		return 1;
	case SDL_KEYDOWN:
		control::pressing = true;
		control::key = e.key.keysym.sym;
		if(hsw && ovrHmd_DismissHSWDisplay(hmd))
			hsw = false;
		break;
	case SDL_KEYUP:
		control::pressing = false;
		switch(control::key) {
		case 27:
			return 1;
		case 'a':
			control::glasses = !control::glasses;
			break;
		case 'b':
			control::objects = !control::objects;
			break;
		case 's':
			control::sitting = !control::sitting;
			break;
		}
	}
	return 0;
}
