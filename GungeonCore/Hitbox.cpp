/**********************************************************************************
// Missile (Código Fonte)
//
// Criação:     23 Nov 2011
// Atualização: 11 Nov 2021
// Compilador:  Visual C++ 2019
//
// Descrição:   Define uma classe para um míssil
//
**********************************************************************************/

#include "Hitbox.h"
#include "GungeonCore.h"

// ------------------------------------------------------------------------------

// ------------------------------------------------------------------------------

Hitbox::Hitbox(float sizeX, float sizeY, float posX, float posY, float lifeTime, float activationDelay)
{
	type = HITBOX;
    layer = CollisionLayer::HITBOX;

	this->lifeTime = lifeTime;
	this->activationDelay = activationDelay;

    BBox(new Rect(-sizeX/2.0f, -sizeY/2.0f, sizeX/2.0f, sizeY/2.0f));
	MoveTo(posX, posY);

    hitboxTimer.Start();
}

// ------------------------------------------------------------------------------

Hitbox::~Hitbox()
{
}

// -------------------------------------------------------------------------------

void Hitbox::Update()
{
    if (hitboxTimer.Elapsed() >= activationDelay && !isActive) {
		isActive = true;
	}

    if (hitboxTimer.Elapsed() >= lifeTime) {
        GungeonCore::level->GetScene()->Delete(this, MOVING);
    }
}

void Hitbox::OnCollision(Object* obj)
{

}
// -------------------------------------------------------------------------------
