/**********************************************************************************
// Green (Código Fonte)
//
// Criação:     15 Out 2012
// Atualização: 05 Nov 2021
// Compilador:  Visual C++ 2022
//
// Descrição:   Objeto faz uma fuga suavizada
//
**********************************************************************************/

#include "RunAwayVillain.h"
#include "GungeonCore.h"
#include "Random.h" 


RunAwayVillain::RunAwayVillain(float pX, float pY, Player* p) 
{
	player = p;
	sprite = new TileSet("Resources/player_sprite_sheet.png", 17, 27, 5, 18);
    animation = new Animation(sprite, 0.095f, true);

   
    uint Seq3[6] = { 12, 13, 14, 15, 16, 17 };

    animation->Add(VILLAIN_RUNNING_LEFT, Seq3, 6);
    animation->Select(VILLAIN_RUNNING_LEFT);

    speed.RotateTo(0.0f);
    speed.ScaleTo(0.0f);

    // mantém certa distância do jogador
    distance = 50;//Random{25,50 }.Rand();

    MoveTo(pX, pY);

    type = RUNAWAYVILLAIN;
}

RunAwayVillain::~RunAwayVillain()
{
    delete sprite;

}

void RunAwayVillain::Update()
{
    animation->NextFrame();
    // a magnitude do vetor target controla quão 
    // rápido o objeto converge para a direção do alvo
    Vector target = Vector(Line::Angle(Point(x, y), Point(player->X(), player->Y())), 100.0f * gameTime);

    // fugir se o player chegar muito perto
    if (Point::Distance(Point(x, y), Point(player->X(), player->Y())) < distance)
    {
        target.Rotate(180.0f);
        target.ScaleTo(50.0f * gameTime);
    }

    speed.Add(target);

    // limita a magnitude da velocidade para impedir 
    // seu crescimento indefinido na soma vetorial
    if (speed.Magnitude() > 8)
        speed.ScaleTo(8.0f);

  

    // move o objeto pelo seu vetor velocidade
    Translate(speed.XComponent() * 15.0f* gameTime, -speed.YComponent() * 15.0f * gameTime);
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

inline void RunAwayVillain::Draw()
{
    animation->Draw(x, y, Layer::MIDDLE, 1.0f, 0);
}
