
#include <stdio.h>
#include <string>

#include "RenderDeviceList.h"

#include "SdlHardwareRenderer.h"
#include <iostream>



Renderer* getRenderDevice(std::string name) {
	// "sdl" is the default
	if (name != "") {
		if (name == "sdl") {
			std::cerr << "no software renderer support";
			return NULL;
		}
#if SDL_VERSION_ATLEAST(2,0,0)
		else if (name == "sdl_hardware") return new SdlHardwareRenderer();
#endif
		else {
			std::cerr << "RenderDeviceList: Render device " << name.c_str() <<" not found. Falling back to the default.";
			std::cerr << "no software renderer support";
		}
	}
	else {
		std::cerr << "no software renderer support";
		return NULL;
	}
}
