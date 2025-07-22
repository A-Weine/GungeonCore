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

// ------------------------------------------------------------------------------

// ------------------------------------------------------------------------------

Fire::Fire(Object * shooter,float angle, Image* img, int types)
{
    // inicializa sprite
    sprite = new Sprite(img);

    // cria bounding box
    BBox(new Circle(6));

    // inicializa velocidade
    speed.RotateTo(angle);
    speed.ScaleTo(300.0f);

    // move para posição
    MoveTo(shooter->X() + 25 * cos(speed.Radians()), shooter->Y() - 25 * sin(speed.Radians()));
    RotateTo(-speed.Angle());

    // define tipo
    if (types == PLAYER) {
       type = FIRE;    

    }
    else {
        type = ENEMYFIRE;
    }


    // incrementa contagem
    //++Hud::missiles;
}

// ------------------------------------------------------------------------------

Fire::~Fire()
{
    delete sprite;

    // decrementa contagem
    //--Hud::missiles;
}

// -------------------------------------------------------------------------------

void Fire::Update()
{
    // move míssel com vetor resultante
    Translate(speed.XComponent() * 1.5f * gameTime, -speed.YComponent() * 1.5f * gameTime);

    // remove míssil da cena se ele sair da área de jogo
    //if (x > game->Width() - 50 || x < 50 || y > game->Height() - 50 || y < 50)
    //{
    //    // volume do som de destruição depende da distância para o jogador
    //    const float MaxDistance = 4406;
    //    const float BaseVolume = 0.8f;
    //    float distance = Point::Distance(Point(x, y), Point(player->X(), player->Y()));
    //    float level = (MaxDistance - distance) / MaxDistance;
    //    //BasicAI::audio->Volume(HITWALL, level * BaseVolume);
    //    //BasicAI::audio->Play(HITWALL);

    //    // adiciona explosão na cena
    //    //BasicAI::scene->Add(new WallHit(x, y), STATIC);

    //    // remove míssil da cena
    //    GungeonCore::scene->Delete();
    //}
}

void Fire::OnCollision(Object* obj)
{
    layer = CollisionLayer::PROJECTILE;

    if (obj->Type() == SOLIDPLATFORM) {
        GungeonCore::audio->Play(FIRE_HIT_STONE);
        GungeonCore::level->GetScene()->Delete(this, MOVING);
    }

}
// -------------------------------------------------------------------------------
