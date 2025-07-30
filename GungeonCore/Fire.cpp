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
#include "Gun.h"

// ------------------------------------------------------------------------------

// ------------------------------------------------------------------------------

Fire::Fire(Object* shooter, Gun* gun, float angle, int typeShot) : Projectile(shooter)
{
    sprite = new Sprite(Projectile::bullet);

    BBox(new Circle(6));

    speed.RotateTo(angle);
    
    if (typeShot == FIRE) {
        speed.ScaleTo(gun->shootVelocity);
        damage = gun->shootDamage;
    }
    else {
        speed.ScaleTo(150.0f);
        damage = 10;
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
<<<<<<< HEAD
        villain->TakeDamage(damage);
=======
        villain->TakeDamage(20);
        GungeonCore::audio->Play(FIRE_HIT_VILLAIN);
>>>>>>> 15a938659cef2e2137a61fbb6733969efdc4efd1
        GungeonCore::level->GetScene()->Delete(this, MOVING);
    }
    else if (obj->Type() == RUNAWAYVILLAIN && type == FIRE) {
        RunAwayVillain* villain = static_cast<RunAwayVillain*>(obj);
<<<<<<< HEAD
        villain->TakeDamage(damage);
=======
        villain->TakeDamage(20);
        GungeonCore::audio->Play(FIRE_HIT_VILLAIN);
>>>>>>> 15a938659cef2e2137a61fbb6733969efdc4efd1
        GungeonCore::level->GetScene()->Delete(this, MOVING);
    }
    else if (obj->Type() == RANDOMMOVEMENTVILLAIN) {
        RandomMovementVillain* villain = static_cast<RandomMovementVillain*>(obj);
<<<<<<< HEAD
        villain->TakeDamage(damage);
=======
        villain->TakeDamage(20);
        GungeonCore::audio->Play(FIRE_HIT_VILLAIN);
>>>>>>> 15a938659cef2e2137a61fbb6733969efdc4efd1
        GungeonCore::level->GetScene()->Delete(this, MOVING);
    }
    else if (obj->Type() == PLAYER && type == ENEMYFIRE) {
        GungeonCore::level->GetScene()->Delete(this, MOVING);
    }
}
// -------------------------------------------------------------------------------
