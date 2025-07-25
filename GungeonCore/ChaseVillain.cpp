

#include "ChaseVillain.h"
#include "GungeonCore.h"
#include "Random.h" 


ChaseVillain::ChaseVillain(float pX, float pY, Player* p)
{
    player = p;
    sprite = new TileSet("Resources/player_sprite_sheet.png", 17, 27, 5, 18);
    animation = new Animation(sprite, 0.095f, true);


    uint Seq3[6] = { 12, 13, 14, 15, 16, 17 };

    animation->Add(VILLAIN_RUNNING, Seq3, 6);
    animation->Select(VILLAIN_RUNNING);

    speed.RotateTo(0.0f);
    speed.ScaleTo(0.0f);

    // Acelera quando se aproxima
    distance = 100;

    MoveTo(pX, pY);

    type = CHASEVILLAIN;
}

ChaseVillain::~ChaseVillain()
{
    delete sprite;

}

void ChaseVillain::Update()
{
    animation->NextFrame();
    // a magnitude do vetor target controla quão 
    // rápido o objeto converge para a direção do alvo
    Vector target = Vector(Line::Angle(Point(x, y), Point(player->X(), player->Y())), 100.0f * gameTime);

    speed.Add(target);

    // limita a magnitude da velocidade para impedir 
    // seu crescimento indefinido na soma vetorial
    if (speed.Magnitude() > 8)
        speed.ScaleTo(8.0f);

    // Acelerar dependendo da distância do player
    if (Point::Distance(Point(x, y), Point(player->X(), player->Y())) < distance)
    {
        speed.ScaleTo(9.0f);
        target.ScaleTo(50.0f * gameTime);
    }



    // move o objeto pelo seu vetor velocidade
    Translate(speed.XComponent() * 15.0f * gameTime, -speed.YComponent() * 15.0f * gameTime);
    Rotate(50 * gameTime);

    // mantém o objeto dentro do mundo do jogo
    if (x < 0)
        MoveTo(0, y);
    if (y < 0)
        MoveTo(x, 0);
    if (x > game->Width())
        MoveTo(game->Width(), y);
    if (y > game->Height())
        MoveTo(x, game->Height());
}


// ---------------------------------------------------------------------------------

inline void ChaseVillain::Draw()
{
    animation->Draw(x, y, Layer::MIDDLE, 1.0f, 0);
}
