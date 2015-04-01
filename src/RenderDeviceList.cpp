
#include <stdio.h>
#include <string>

#include "RenderDeviceList.h"

#include "SdlHardwareRenderer.h"
#include "SdlSoftwareRenderer.h"



Renderer* getRenderDevice(std::string name) {
	// "sdl" is the default
	if (name != "") {
		if (name == "sdl") return new SdlSoftwareRenderer();
#if SDL_VERSION_ATLEAST(2,0,0)
		else if (name == "sdl_hardware") return new SdlHardwareRenderer();
#endif
		else {
			logError("RenderDeviceList: Render device '%s' not found. Falling back to the default.", name.c_str());
			return new SdlSoftwareRenderer();
		}
	}
	else {
		return new SdlSoftwareRenderer();
	}
}
