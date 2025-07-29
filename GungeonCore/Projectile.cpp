/**********************************************************************************
// Projectile (Código Fonte)
//
// Criação:     23 Jul 2025
// Atualização: 23 Jul 2025
// Compilador:  Visual C++ 2022
//
// Descrição:   Classe base para todos os projéteis.
//
**********************************************************************************/

#include "Projectile.h"
#include "GungeonCore.h" // Ou o nome da sua classe principal
#include "Engine.h"

// ------------------------------------------------------------------------------

Image* Projectile::bullet = nullptr;

// ------------------------------------------------------------------------------

Projectile::Projectile(Object* owner)
{
    anim = nullptr;
    tileset = nullptr;
    sprite = nullptr;
    shooter = owner;
}

// ------------------------------------------------------------------------------

Projectile::~Projectile()
{
    delete anim;
    delete tileset;
    delete sprite;
}

// -------------------------------------------------------------------------------

void Projectile::Update()
{
    Translate(speed.XComponent() * 1.5f * gameTime, -speed.YComponent() * 1.5f * gameTime);

    if (x > game->Width() + 100 || x < -100 || y > game->Height() + 100 || y < -100)
    {
        GungeonCore::level->GetScene()->Delete(this, MOVING);
    }
}

void Projectile::Draw()
{
    if (anim)
    {
        anim->Draw(x, y, Layer::MIDDLE, 1.0f, rotation);
    }

    if (sprite)
    {
        sprite->Draw(x, y, Layer::MIDDLE, 1.0f, rotation);
    }
}

void Projectile::OnCollision(Object* obj)
{
    layer = CollisionLayer::PROJECTILE;

    if (obj == shooter)
        return;

    if (obj->Type() == PLATFORM)
    {
         GungeonCore::level->GetScene()->Delete(this, MOVING);
    }
}