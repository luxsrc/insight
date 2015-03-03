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

#include <cstdio>
#include <cstdlib>

#include <OVR_CAPI.h>

int main(int argc, char *argv[])
{
	ovr_Initialize();

	ovrHmd hmd = ovrHmd_Create(0);
	if(!hmd)
		fprintf(stderr, "ERROR: %s\n", ovrHmd_GetLastError(NULL));
	else {
		printf("Initialized HMD: %s - %s\n",
		       hmd->Manufacturer, hmd->ProductName);
		ovrHmd_Destroy(hmd);
	}

	ovr_Shutdown();
	return EXIT_SUCCESS;
}
