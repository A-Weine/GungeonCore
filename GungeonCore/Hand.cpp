#include "Hand.h"
#include "GungeonCore.h"
#include "Gun.h"

// ---------------------------------------------------------------------------------

Hand::Hand(Player * players, TileSet* tileSet, Animation * animation, Gun* guns)
{
    gun = guns;
    sprite = tileSet;
    animations = animation;
    player = players;

    type = HAND;
}

// ---------------------------------------------------------------------------------

Hand::~Hand()
{
    delete sprite;
}

// -------------------------------------------------------------------------------

void Hand::Update()
{
    // 1. POSICIONAR A MÃO: Faça a mão seguir o jogador.
    // O pivô da mão deve estar sempre na posição do jogador (ou um pouco deslocado).
    // Supondo que a classe Hand tenha uma posição.
    this->MoveTo(player->X(), player->Y()+10);

    // 2. OBTER POSIÇÕES ATUAIS: Faça isso a cada frame.
    Point playerPos(this->X(), this->Y()); // Pega a posição atual da mão/jogador
    Point mouseWorldPos = window->ScreenToWorld(GungeonCore::level); // Pega a posição ATUAL do mouse

    // 3. CALCULAR O ÂNGULO:
    handAngle = Line::Angle(playerPos, mouseWorldPos);

    
}

// -------------------------------------------------------------------------------

void Hand::Draw()
{
    animations->Draw(x, y, Layer::UPPER+0.1, scale, -handAngle);
}