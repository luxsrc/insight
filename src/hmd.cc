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

ovrHmd mkhmd()
{
	ovrHmd hmd;
	if((hmd = ovrHmd_Create(0)))
		print("Initialized head mounted display: %s - %s: %s\n",
		      hmd->Manufacturer, hmd->ProductName,
		      hmd->HmdCaps & ovrHmdCap_ExtendDesktop ?
		      "extended display" : "direct HMD");
	else if((hmd = ovrHmd_CreateDebug(ovrHmd_DK2)))
		print("Initialized head mounted display: %s - %s: virtual\n",
		      hmd->Manufacturer, hmd->ProductName);
	if(!hmd)
		return hmd; // NULL with correct type

	ovrHmd_SetEnabledCaps(hmd, ovrHmdCap_LowPersistence |
	                           ovrHmdCap_DynamicPrediction);
	return hmd;
}

void rmhmd(ovrHmd hmd)
{
	ovrHmd_Destroy(hmd);
	print("Finalized head mounted display\n");
}
