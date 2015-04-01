
/*
 * Game.cpp
 *
 *  Created on: 9-mrt.-2015
 *      Author: Matthias
 */

#include "Game.h"

#include "Settings.h"
//#include "SharedResources.h"
#include "Car.h"
#include "EntityFactory.h"
#include "SdlFactory.h"
#include <vector>
#include "GameRenderer.h"
using namespace std;

/**
 * Game initialization.
 */

Game::Game(std::string &render_device_name) {
	// SDL Inits
			if ( SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0 ) {
				logError("main: Could not initialize SDL: %s", SDL_GetError());
				exit(1);
			}

			// Create render Device and Rendering Context.
			renderDevice = getRenderDevice(render_device_name);
			int status = renderDevice->createContext(800, 600);
			factory = new SdlFactory();

			/*create Game entitys and add them to the list of all entities */
			Entity * entity = factory->createCar(renderDevice);
			renderList.push_back(entity);
			road = factory->createRoad(renderDevice);
			renderList.push_back(road);
			gameRenderer = new GameRenderer(renderDevice);

			if (status == -1) {

				logError("main: Error during SDL_SetVideoMode: %s", SDL_GetError());
				SDL_Quit();
			}
}

void Game::start() {
	bool done = false;
	int delay = int(floor((1000.f/MAX_FRAMES_PER_SEC)+0.5f));
	int logic_ticks = SDL_GetTicks();

	while ( !done ) {
		int loops = 0;
		int now_ticks = SDL_GetTicks();
		int prev_ticks = now_ticks;

		/*while (now_ticks > logic_ticks && loops < MAX_FRAMES_PER_SEC) {
			// Frames where data loading happens (GameState switching and map loading)
			// take a long time, so our loop here will think that the game "lagged" and
			// try to compensate. To prevent this compensation, we mark those frames as
			// "loading frames" and update the logic ticker without actually executing logic.
			if (gswitch->isLoadingFrame()) {
				logic_ticks = now_ticks;
				break;
			}*/

			/*SDL_PumpEvents();
			inpt->handle(debug_event);

			// Skip game logic when minimized on Android
			if (inpt->window_minimized && !inpt->window_restored)
				break;

			gswitch->logic();
			inpt->resetScroll();

			// Engine done means the user escapes the main game menu.
			// Input done means the user closes the window.
			done = gswitch->done || inpt->done;

			logic_ticks += delay;
			loops++;

			// Android only
			// When the app is minimized on Android, no logic gets processed.
			// As a result, the delta time when restoring the app is large, so the game will skip frames and appear to be running fast.
			// To counter this, we reset our delta time here when restoring the app
			if (inpt->window_minimized && inpt->window_restored) {
				logic_ticks = now_ticks = SDL_GetTicks();
				inpt->window_minimized = inpt->window_restored = false;
				break;
			}

			// don't skip frames if the game is paused
			if (gswitch->isPaused()) {
				logic_ticks = now_ticks;
				break;
			}
		}*/

		renderDevice->blankScreen();
		gameRenderer->render(renderList);
		road->advance();

		// display the FPS counter
		// if the frame completed quickly, we estimate the delay here
		now_ticks = SDL_GetTicks();
		int delay_ticks = 0;
		if (now_ticks - prev_ticks < delay) {
			delay_ticks = delay - (now_ticks - prev_ticks);
		}
		if (now_ticks+delay_ticks - prev_ticks != 0) {
			//gswitch->showFPS(1000 / (now_ticks+delay_ticks - prev_ticks));
		}

		renderDevice->commitFrame();

		// delay quick frames
		now_ticks = SDL_GetTicks();
		if (now_ticks - prev_ticks < delay) {
			SDL_Delay(delay - (now_ticks - prev_ticks));
		}

	}
}



Game::~Game() {
	cout << "Game Destructor callled \n";
	/*delete gswitch;

	delete anim;
	delete comb;
	delete font;
	delete inpt;
	delete mods;
	delete msg;
	delete snd;
	delete curs;*/

	//Mix_CloseAudio();

	if (renderDevice)
		renderDevice->destroyContext();
	delete renderDevice;

	SDL_Quit();
}

/**
 * Render all graphics for a single frame
 */
////void Game::states() {
//
//	// Create a list of Renderables from all objects not already on the map.
//	// split the list into the beings alive (may move) and dead beings (must not move)
//	std::vector<Renderable> rens;
//	std::vector<Renderable> rens_dead;
//
////	pc->addRenders(rens, rens_dead);
////
////	enemies->addRenders(rens, rens_dead);
////
////	npcs->addRenders(rens); // npcs cannot be dead
////
////	loot->addRenders(rens, rens_dead);
////
////	hazards->addRenders(rens, rens_dead);
////
////
////	// render the static map layers plus the renderables
////	mapr->render(rens, rens_dead);
////
////	// mouseover tooltips
////	loot->renderTooltips(mapr->cam);
////
////	if (mapr->map_change) {
////		menu->mini->prerender(&mapr->collider, mapr->w, mapr->h);
////		mapr->map_change = false;
////	}
////	menu->mini->getMapTitle(mapr->title);
////	menu->mini->render(pc->stats.pos);
////	menu->render();
////
////	// render combat text last - this should make it obvious you're being
////	// attacked, even if you have menus open
////	comb->render();
////}



/**
 * Render all graphics for a single frame
 */
//void Game::render() {
//
//	// Create a list of Renderables from all objects not already on the map.
//	// split the list into the beings alive (may move) and dead beings (must not move)
//	std::vector<Renderable> rens;
//	std::vector<Renderable> rens_dead;
//
////	pc->addRenders(rens, rens_dead);
////
////	enemies->addRenders(rens, rens_dead);
////
////	npcs->addRenders(rens); // npcs cannot be dead
////
////	loot->addRenders(rens, rens_dead);
////
////	hazards->addRenders(rens, rens_dead);
////
////
////	// render the static map layers plus the renderables
////	mapr->render(rens, rens_dead);
////
////	// mouseover tooltips
////	loot->renderTooltips(mapr->cam);
////
////	if (mapr->map_change) {
////		menu->mini->prerender(&mapr->collider, mapr->w, mapr->h);
////		mapr->map_change = false;
////	}
////	menu->mini->getMapTitle(mapr->title);
////	menu->mini->render(pc->stats.pos);
////	menu->render();
////
////	// render combat text last - this should make it obvious you're being
////	// attacked, even if you have menus open
////	comb->render();
//}

Renderer* Game::getRenderer() const {
	return renderDevice;
}

/* misschien terugkeren naar renderable voor GameRenderer niet alleen entities moeten gerenderd worden.
 * Binnen Entity kunnen we lijst maken van cars, roads... en degene die getoond moeten worden toevoegen aan renderable.
 * Er zijn ook menu's... die niet behoren tot entity, vandaar dat renderable nog niet zo'n slecht idee is
 */

