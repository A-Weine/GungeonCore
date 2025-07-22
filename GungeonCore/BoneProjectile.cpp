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

#include "BoneProjectile.h"
#include "GungeonCore.h"
#include "Hud.h"
#include "Platform.h"

// ------------------------------------------------------------------------------

// ------------------------------------------------------------------------------

BoneProjectile::BoneProjectile(Object* shooter, float angle)
{
    // inicializa sprite
    sprite = new TileSet("Resources/bone_projectile_sprite_sheet.png", 14.0f, 14.0f, 3.0f, 8.0f);
	animation = new Animation(sprite, 0.095f, true);

    // cria bounding box 
    BBox(new Circle(8));

    // inicializa velocidade
    speed.RotateTo(angle);
    speed.ScaleTo(200.0f);

    // move para posição
    MoveTo(shooter->X() + 35 * cos(speed.Radians()), shooter->Y() - 35 * sin(speed.Radians()));
    RotateTo(-speed.Angle());

    type = BONEPROJECTILE;
}

// ------------------------------------------------------------------------------

BoneProjectile::~BoneProjectile()
{
    delete sprite;
    delete animation;
}

// -------------------------------------------------------------------------------

void BoneProjectile::Update()
{
    animation->NextFrame();
    Translate(speed.XComponent() * 1.5f * gameTime, -speed.YComponent() * 1.5f * gameTime);
}

void BoneProjectile::OnCollision(Object* obj)
{
    layer = CollisionLayer::PROJECTILE;

    if (obj->Type() == SOLIDPLATFORM) {
        GungeonCore::audio->Play(FIRE_HIT_STONE);
        GungeonCore::level->GetScene()->Delete(this, MOVING);
    }

    if (obj->Type() == VILLAIN) {
        return;
	}
     

}
// -------------------------------------------------------------------------------
