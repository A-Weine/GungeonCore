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

#include "Fire.h"
#include "GungeonCore.h"
#include "Hud.h"
#include "Platform.h"
#include "JarShrap.h"
#include "WallHit.h"
#include "RandomMovementVillain.h"

// ------------------------------------------------------------------------------

// ------------------------------------------------------------------------------

Fire::Fire(Object* shooter, float angle, int typeShot) : Projectile(shooter)
{
    sprite = new Sprite(Projectile::bullet);

    BBox(new Circle(6));

    speed.RotateTo(angle);
    
    if (typeShot == FIRE) {
        speed.ScaleTo(320.0f);
    }
    else {
        speed.ScaleTo(150.0f);
    }

    MoveTo(shooter->X() + 15 * cos(speed.Radians()), shooter->Y() - 15 * sin(speed.Radians()));
    RotateTo(-speed.Angle());

    type = typeShot;
}

// ------------------------------------------------------------------------------

Fire::~Fire()
{
}

// -------------------------------------------------------------------------------

void Fire::OnCollision(Object* obj)
{
    Projectile::OnCollision(obj);

    if (obj->Type() == PLATFORM) {
        GungeonCore::level->GetScene()->Add(new WallHit(X(), Y(), speed.Angle()), STATIC);
        GungeonCore::audio->Play(FIRE_HIT_STONE);
        GungeonCore::level->GetScene()->Delete(this, MOVING);
    }
    else if (obj->Type() == CHASEVILLAIN && type == FIRE) {
        ChaseVillain* villain = static_cast<ChaseVillain*>(obj);
        villain->TakeDamage(20);
        GungeonCore::audio->Play(FIRE_HIT_VILLAIN);
        GungeonCore::level->GetScene()->Delete(this, MOVING);
    }
    else if (obj->Type() == RUNAWAYVILLAIN && type == FIRE) {
        RunAwayVillain* villain = static_cast<RunAwayVillain*>(obj);
        villain->TakeDamage(20);
        GungeonCore::audio->Play(FIRE_HIT_VILLAIN);
        GungeonCore::level->GetScene()->Delete(this, MOVING);
    }
    else if (obj->Type() == RANDOMMOVEMENTVILLAIN) {
        RandomMovementVillain* villain = static_cast<RandomMovementVillain*>(obj);
        villain->TakeDamage(20);
        GungeonCore::audio->Play(FIRE_HIT_VILLAIN);
        GungeonCore::level->GetScene()->Delete(this, MOVING);
    }
  /*  else if (obj->Type() == PLAYER) {
        GungeonCore::level->GetScene()->Delete(this, MOVING);
    }*/
}
// -------------------------------------------------------------------------------
