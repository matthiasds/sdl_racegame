/*
Copyright © 2011-2012 Clint Bellanger
Copyright © 2012 Igor Paliychuk
Copyright © 2012 Stefan Beller

This file is part of FLARE.

FLARE is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

FLARE is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
FLARE.  If not, see http://www.gnu.org/licenses/
*/

/**
 * Settings
 */

#include <cstring>
#include <typeinfo>
#include <cmath>
#include <iomanip>

#include "CommonIncludes.h"
//#include "FileParser.h"
#include "Settings.h"
#include "Utils.h"
//#include "UtilsParsing.h"
//#include "UtilsFileSystem.h"

#ifdef _MSC_VER
#define log2(x)	logf(x)/logf(2)
#endif

#ifdef __ANDROID__
#include <SDL.h>
#endif

class ConfigEntry {
public:
	const char * name;
	const std::type_info * type;
	const char * default_val;
	void * storage;
	const char * comment;
};

ConfigEntry config[] = {
	{ "fullscreen",       &typeid(FULLSCREEN),      "0",   &FULLSCREEN,      "fullscreen mode. 1 enable, 0 disable."},
	{ "resolution_w",     &typeid(VIEW_W),          "640", &VIEW_W,          "display resolution. 640x480 minimum."},
	{ "resolution_h",     &typeid(VIEW_H),          "480", &VIEW_H,          NULL},
	{ "audio",            &typeid(AUDIO),           "1",   &AUDIO,           "Enable music and sound subsystem."},
	{ "music_volume",     &typeid(MUSIC_VOLUME),    "96",  &MUSIC_VOLUME,    "music and sound volume (0 = silent, 128 = max)"},
	{ "sound_volume",     &typeid(SOUND_VOLUME),    "128", &SOUND_VOLUME,    NULL},
	{ "combat_text",      &typeid(COMBAT_TEXT),     "0",   &COMBAT_TEXT,     "display floating damage text. 1 enable, 0 disable."},
	{ "mouse_move",       &typeid(MOUSE_MOVE),      "0",   &MOUSE_MOVE,      "use mouse to move (experimental). 1 enable, 0 disable."},
	{ "hwsurface",        &typeid(HWSURFACE),       "1",   &HWSURFACE,       "hardware surfaces, double buffering. Try disabling for performance. 1 enable, 0 disable."},
	{ "doublebuf",        &typeid(DOUBLEBUF),       "1",   &DOUBLEBUF,       NULL},
	{ "enable_joystick",  &typeid(ENABLE_JOYSTICK), "0",   &ENABLE_JOYSTICK, "joystick settings."},
	{ "joystick_device",  &typeid(JOYSTICK_DEVICE), "0",   &JOYSTICK_DEVICE, NULL},
	{ "joystick_deadzone",&typeid(JOY_DEADZONE),    "100", &JOY_DEADZONE,    NULL},
	{ "language",         &typeid(LANGUAGE),        "en",  &LANGUAGE,        "2-letter language code."},
	{ "change_gamma",     &typeid(CHANGE_GAMMA),    "0",   &CHANGE_GAMMA,    "allow changing gamma (experimental). 1 enable, 0 disable."},
	{ "gamma",            &typeid(GAMMA),           "1.0", &GAMMA,           "screen gamma (0.5 = darkest, 2.0 = lightest)"},
	{ "mouse_aim",        &typeid(MOUSE_AIM),       "1",   &MOUSE_AIM,       "use mouse to aim. 1 enable, 0 disable."},
	{ "no_mouse",         &typeid(NO_MOUSE),        "0",   &NO_MOUSE,        "make using mouse secondary, give full control to keyboard. 1 enable, 0 disable."},
	{ "show_fps",         &typeid(SHOW_FPS),        "0",   &SHOW_FPS,        "show frames per second. 1 enable, 0 disable."},
	{ "show_hotkeys",     &typeid(SHOW_HOTKEYS),    "1",   &SHOW_HOTKEYS,    "show hotkeys names on power bar. 1 enable, 0 disable."},
	{ "colorblind",       &typeid(COLORBLIND),      "0",   &COLORBLIND,      "enable colorblind tooltips. 1 enable, 0 disable"},
	{ "hardware_cursor",  &typeid(HARDWARE_CURSOR), "0",   &HARDWARE_CURSOR, "use the system mouse cursor. 1 enable, 0 disable"},
	{ "dev_mode",         &typeid(DEV_MODE),        "0",   &DEV_MODE,        "allow opening the developer console. 1 enable, 0 disable"},
	{ "show_target",      &typeid(SHOW_TARGET),     "0",   &SHOW_TARGET,     "show the targeting reticle on the ground when attacking. 1 enable, 0 disable"},
	{ "loot_tooltips",    &typeid(LOOT_TOOLTIPS),   "1",   &LOOT_TOOLTIPS,   "always show loot tooltips. 1 enable, 0 disable"}
};
const int config_size = sizeof(config) / sizeof(ConfigEntry);

// Paths
std::string PATH_CONF = "";
std::string PATH_USER = "";
std::string PATH_DATA = "";
std::string CUSTOM_PATH_DATA = "";

// Filenames
std::string FILE_SETTINGS	= "settings.txt";
std::string FILE_KEYBINDINGS = "keybindings.txt";

// Tile Settings
float UNITS_PER_PIXEL_X;
float UNITS_PER_PIXEL_Y;
unsigned short TILE_W;
unsigned short TILE_H;
unsigned short TILE_W_HALF;
unsigned short TILE_H_HALF;
unsigned short TILESET_ISOMETRIC;
unsigned short TILESET_ORTHOGONAL;
unsigned short TILESET_ORIENTATION;

// Main Menu frame size
unsigned short FRAME_W;
unsigned short FRAME_H;

unsigned short ICON_SIZE;

// Video Settings
bool FULLSCREEN;
unsigned short MAX_FRAMES_PER_SEC = 60;
unsigned char BITS_PER_PIXEL = 32;
unsigned short VIEW_W;
unsigned short VIEW_H;
unsigned short VIEW_W_HALF = VIEW_W/2;
unsigned short VIEW_H_HALF = VIEW_H/2;
short MIN_VIEW_W = -1;
short MIN_VIEW_H = -1;
bool DOUBLEBUF;
bool HWSURFACE;
bool CHANGE_GAMMA;
float GAMMA;

// Audio Settings
bool AUDIO;
unsigned short MUSIC_VOLUME;
unsigned short SOUND_VOLUME;

// Interface Settings
bool COMBAT_TEXT;
bool SHOW_FPS;
bool SHOW_HOTKEYS;
bool COLORBLIND;
bool HARDWARE_CURSOR;
bool DEV_MODE;
bool DEV_HUD = false;
bool SHOW_TARGET;
bool LOOT_TOOLTIPS;

// Input Settings
bool MOUSE_MOVE;
bool ENABLE_JOYSTICK;
int JOYSTICK_DEVICE;
bool MOUSE_AIM;
bool NO_MOUSE;
int JOY_DEADZONE;
bool TOUCHSCREEN = false;

// Language Settings
std::string LANGUAGE = "en";

// Autopickup Settings
bool AUTOPICKUP_CURRENCY;

// Combat calculation caps (percentage)
short MAX_ABSORB;
short MAX_RESIST;
short MAX_BLOCK;
short MAX_AVOIDANCE;
short MIN_ABSORB;
short MIN_RESIST;
short MIN_BLOCK;
short MIN_AVOIDANCE;

// Elemental types
std::vector<Element> ELEMENTS;

// Equipment flags
std::map<std::string,std::string> EQUIP_FLAGS;

// Hero classes
std::vector<HeroClass> HERO_CLASSES;

// Currency settings
std::string CURRENCY;
float VENDOR_RATIO;

// Death penalty settings
bool DEATH_PENALTY;
bool DEATH_PENALTY_PERMADEATH;
int DEATH_PENALTY_CURRENCY;
int DEATH_PENALTY_XP;
int DEATH_PENALTY_XP_CURRENT;
bool DEATH_PENALTY_ITEM;

// Other Settings
bool MENUS_PAUSE;
bool SAVE_HPMP;
bool ENABLE_PLAYGAME;
int CORPSE_TIMEOUT;
bool SELL_WITHOUT_VENDOR;
int AIM_ASSIST;
std::string SAVE_PREFIX = "";
std::string WINDOW_TITLE;
int SOUND_FALLOFF;
int PARTY_EXP_PERCENTAGE;
bool ENABLE_ALLY_COLLISION_AI;
bool ENABLE_ALLY_COLLISION;
int CURRENCY_ID;
float INTERACT_RANGE;
bool SAVE_ONLOAD = true;
bool SAVE_ONEXIT = true;


/**
 * Return a string of version name + version number
 */
std::string getVersionString() {
	std::stringstream ss;
	ss << VERSION_NAME << " v" << VERSION_MAJOR << "." << std::setfill('0') << std::setw(2) << VERSION_MINOR;
	return ss.str();
}

/**
 * Compare version numbers. Returns true if the first number is larger than the second
 */
bool compareVersions(int maj0, int min0, int maj1, int min1) {
	if (maj0 == maj1)
		return min0 > min1;
	else
		return maj0 > maj1;
}


