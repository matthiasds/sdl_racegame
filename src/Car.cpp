/*
 * Car.cpp
 *
 *  Created on: 9-mrt.-2015
 *      Author: Matthias
 */

#include "Car.h"
#include "Entity.h"


Car::Car()
	: speed(0) {
	pos=FPoint(Point(10,10));

	prio=2;
	}


bool Car::move() {
//	if (!allow_movement) {
//		return false;
//	}
//	else if (MOUSE_MOVE) {
//		return inpt->pressing[MAIN1];
//	}
//	else {
//		return (inpt->pressing[UP] && !inpt->lock[UP]) ||
//			   (inpt->pressing[DOWN] && !inpt->lock[DOWN]) ||
//			   (inpt->pressing[LEFT] && !inpt->lock[LEFT]) ||
//			   (inpt->pressing[RIGHT] && !inpt->lock[RIGHT]);
//	}
}

void Car::set_direction() {
//	// handle direction changes
//	if (MOUSE_MOVE) {
//		FPoint target = screen_to_map(inpt->mouse.x, inpt->mouse.y, stats.pos.x, stats.pos.y);
//		stats.direction = calcDirection(stats.pos, target);
//	}
//	else {
//		if (inpt->pressing[UP] && !inpt->lock[UP] && inpt->pressing[LEFT] && !inpt->lock[LEFT]) stats.direction = 1;
//		else if (inpt->pressing[UP] && !inpt->lock[UP] && inpt->pressing[RIGHT] && !inpt->lock[RIGHT]) stats.direction = 3;
//		else if (inpt->pressing[DOWN] && !inpt->lock[DOWN] && inpt->pressing[RIGHT] && !inpt->lock[RIGHT]) stats.direction = 5;
//		else if (inpt->pressing[DOWN] && !inpt->lock[DOWN] && inpt->pressing[LEFT] && !inpt->lock[LEFT]) stats.direction = 7;
//		else if (inpt->pressing[LEFT] && !inpt->lock[LEFT]) stats.direction = 0;
//		else if (inpt->pressing[UP] && !inpt->lock[UP]) stats.direction = 2;
//		else if (inpt->pressing[RIGHT] && !inpt->lock[RIGHT]) stats.direction = 4;
//		else if (inpt->pressing[DOWN] && !inpt->lock[DOWN]) stats.direction = 6;
//		// Adjust for ORTHO tilesets
//		if (TILESET_ORIENTATION == TILESET_ORTHOGONAL &&
//				((inpt->pressing[UP] && !inpt->lock[UP]) || (inpt->pressing[DOWN] && !inpt->lock[UP]) ||
//				 (inpt->pressing[LEFT] && !inpt->lock[LEFT]) || (inpt->pressing[RIGHT] && !inpt->lock[RIGHT])))
//			stats.direction = stats.direction == 7 ? 0 : stats.direction + 1;
//	}
}



Car::~Car() {
//	if (SHOW_TARGET) {
//		anim->decreaseCount("animations/target.txt");
//		delete target_anim;
//	}
//
//	if (stats.transformed && charmed_stats && charmed_stats->animations != "") {
//		anim->decreaseCount(charmed_stats->animations);
//	}
//	else {
//		anim->decreaseCount("animations/hero.txt");
//	}
//
//	for (unsigned int i=0; i<animsets.size(); i++) {
//		if (animsets[i])
//			anim->decreaseCount(animsets[i]->getName());
//		delete anims[i];
//	}
//	anim->cleanUp();
//
//	delete charmed_stats;
//	delete hero_stats;
//
//	unloadSounds();
//
//	for (unsigned i=0; i<sound_steps.size(); i++)
//		snd->unload(sound_steps[i]);
//
//	delete haz;
}
