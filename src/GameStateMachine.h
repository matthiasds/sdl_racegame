/*
 * GameStateMachine.h
 *
 *  Created on: 28-mrt.-2015
 *      Author: Matthias
 */

#ifndef GAMESTATEMACHINE_H_
#define GAMESTATEMACHINE_H_

enum GameState {BOOT, MENU, PLAY};
class GameStateMachine {
public:
	GameStateMachine();
	virtual ~GameStateMachine();
private:
	GameState gameState;
};

#endif /* GAMESTATEMACHINE_H_ */
