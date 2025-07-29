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

// ------------------------------------------------------------------------------

// ------------------------------------------------------------------------------

Fire::Fire(Object* shooter, float angle, int typeShot) : Projectile(shooter)
{
    sprite = new Sprite(Projectile::bullet);

    BBox(new Circle(6));

    speed.RotateTo(angle);
    speed.ScaleTo(300.0f);

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
        GungeonCore::audio->Play(FIRE_HIT_STONE);
    }
    else if (obj->Type() == CHASEVILLAIN) {
        ChaseVillain* villain = static_cast<ChaseVillain*>(obj);
        villain->TakeDamage(20);
    }
    else if (obj->Type() == RUNAWAYVILLAIN) {
        RunAwayVillain* villain = static_cast<RunAwayVillain*>(obj);
        villain->TakeDamage(20);
    }

    if (obj->Type() == PLAYER && type == FIRE) {
        return;
    }

    GungeonCore::level->GetScene()->Delete(this, MOVING);
}
// -------------------------------------------------------------------------------
