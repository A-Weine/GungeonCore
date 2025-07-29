#ifndef _GungeonCore_SHADOW_H_
#define _GungeonCore_SHADOW_H_

#include "Object.h"
#include "Sprite.h"
#include "Player.h"
#include "Scene.h"

class Shadow : public Object
{
private:
	Sprite* sprite;
	
	//Sprite* debugRayStartSprite;
	//Sprite* debugHitPointSprite;
	//float debugStartX;
	//float debugStartY;
	//float debugHitX;
	//float debugHitY;
	
	Scene* parentScene;
	Object* owner;
	float originalScale;
	float scaleFactor;

	int distance;

public:
	Shadow(Object* followTarget, Scene* scene, int Distance);
	~Shadow();

	void Update() override;
	void Draw() override;
};

#endif