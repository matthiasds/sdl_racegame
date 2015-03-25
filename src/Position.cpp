/*
 * Position.cpp
 *
 *  Created on: 14-mrt.-2015
 *      Author: Matthias
 */

#include <Position.h>

Position::Position() {
	center_x=0;
	center_y=0;
}

Position::Position(int x, int y) {
	center_x=x;
	center_y=y;
}

Position::~Position() {

}

int Position::getCenterX() const {
	return center_x;
}

void Position::setCenterX(int centerX) {
	center_x = centerX;
}

int Position::getCenterY() const {
	return center_y;
}

void Position::setCenterY(int centerY) {
	center_y = centerY;
}
