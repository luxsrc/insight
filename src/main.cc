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
#include "img.h"
#include "vol.h"
#include <cstdlib>

namespace global {
	bool fixed   = true;
	bool mounted = true;
}

int main(int argc, char *argv[])
{
	ovrHmd   hmd = setup();
	unsigned vol = 0;
	unsigned img = 0;
	if(argc > 1) {
		char name[1024];
		sprintf(name, "%s.raw", argv[1]);
		vol = mkvol(name);
		sprintf(name, "%s.jpg", argv[1]);
		img = mkimg(name);
	}

	for(bool done = 0; !done; ) {
		SDL_Event event;
		if(SDL_PollEvent(&event))
			done = handle(event);
		else
			display(hmd,
			        global::fixed   ? vol : 0,
			        global::mounted ? img : 0);
	}
	putchar('\n');

	if(img) rmimg(img);
	if(vol) rmvol(vol);
	return EXIT_SUCCESS;
}
