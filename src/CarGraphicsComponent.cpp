#include "CarGraphicsComponent.h"

#include <stddef.h>
#include <SDL2/SDL_rect.h>

#include "Entity.h"
#include "Image.h"
#include "Renderer.h"

CarGraphicsComponent::CarGraphicsComponent(Renderer& renderer) {

	Image* carImage=renderer.loadImage(PATH_CAR2);
	int originalImagesizeW = carImage->getWidth();
	int originalImagesizeH = carImage->getHeight();
	SubArea screenSize=renderer.getContextSize();
	int windowsizeW = screenSize.w;
	int windowsizeH = screenSize.h;
	int scaledImageSizeW = windowsizeW/5;
	int scaledImageSizeH = originalImagesizeH *((float) scaledImageSizeW/originalImagesizeW);//scale related to windowsize
	carImage = carImage->resize(scaledImageSizeW,scaledImageSizeH); //keep aspect ratio
	Point offset=Point(-(windowsizeW-scaledImageSizeW)/2, -(windowsizeH - scaledImageSizeH)); //center
	Sprite* carSprite = carImage->createSprite(true);
	carSprite->setOffset(offset);
	carSprites.push_back(carSprite);
}



CarGraphicsComponent::~CarGraphicsComponent() {
	// TODO Auto-generated destructor stub
}

void CarGraphicsComponent::update(Entity& entity, Renderer& renderer) {
	if (carSprites[0] != NULL) {
		renderer.render(carSprites[0]);
	}
}

