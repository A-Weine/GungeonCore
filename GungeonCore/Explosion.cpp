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

#include "GungeonCore.h"
#include "Explosion.h"

// ------------------------------------------------------------------------------

// ------------------------------------------------------------------------------

Explosion::Explosion(float X, float Y)
{
    sprite = new TileSet("Resources/explosion_sprite_sheet.png", 34, 34, 3, 5);
    animation = new Animation(sprite, 0.025f, false);

    BBox(new Circle(25));

    uint seqExplosion[5] = { 0, 1, 2 , 3, 4 };

    animation->Add(static_cast<uint>(Explosions::EXPLOSION), seqExplosion, 5);

    animation->Select(static_cast<uint>(state));

    timer.Start();
    TTL = 0.15;

    MoveTo(X, Y);
}

// ------------------------------------------------------------------------------

Explosion::~Explosion()
{
    delete sprite;

}

// -------------------------------------------------------------------------------

void Explosion::Update()
{
    // move míssel com vetor resultante
    animation->NextFrame();
    
    if (timer.Elapsed(TTL))
    {
        GungeonCore::level->GetScene()->Delete(this, STATIC);
    }
 

}
void Explosion::OnCollision(Object* obj)
{

}
// -------------------------------------------------------------------------------

void Explosion::Draw()
{
    animation->Draw(x, y, Layer::FRONT, 2.0f, 0);
}