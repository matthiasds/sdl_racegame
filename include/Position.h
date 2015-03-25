/*
 * Position.h
 *
 *  Created on: 14-mrt.-2015
 *      Author: Matthias
 */

#ifndef POSITION_H_
#define POSITION_H_

class Position {
public:
	Position();
	Position(int x, int y);
	virtual ~Position();
	int getCenterX() const;
	void setCenterX(int centerX);
	int getCenterY() const;
	void setCenterY(int centerY);
private:
	int center_x;
	int center_y;
};

#endif /* POSITION_H_ */
