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
    handAngle = player->aimingAngle;
    RotateTo(handAngle);

    gun->Update();
    // 1. POSICIONAR A MÃO: Faça a mão seguir o jogador.
    // O pivô da mão deve estar sempre na posição do jogador (ou um pouco deslocado).
    // Supondo que a classe Hand tenha uma posição.
    switch (player->state)
    {
    case(PlayerState::IDLE_DOWN_RIGHT):
    case(PlayerState::IDLE_UP_RIGHT):
    case(PlayerState::RUNNING_DOWN_RIGHT):
    case(PlayerState::RUNNING_UP_RIGHT):
    case(PlayerState::IDLE_UP):
    case(PlayerState::RUNNING_UP):
        this->MoveTo(player->X() + 10, player->Y() - 3);
        break;
    case(PlayerState::IDLE_DOWN_LEFT):
    case(PlayerState::IDLE_UP_LEFT):
    case(PlayerState::RUNNING_DOWN_LEFT):
    case(PlayerState::RUNNING_UP_LEFT):
    case(PlayerState::IDLE_DOWN):
    case(PlayerState::RUNNING_DOWN):
        this->MoveTo(player->X() - 10, player->Y() - 3);
        break;
    }

    if (gun)
    {
        gun->MoveTo(this->X(), this->Y());
        gun->RotateTo(this->Rotation()); // A arma herda a rotação da mão
        gun->Update();
    }
}

// -------------------------------------------------------------------------------

void Hand::Draw()
{
    // O ângulo de rotação visual que a Hand nos deu (geralmente negativo)
    float visualAngle = -handAngle;

    // Se a animação selecionada for uma das da ESQUERDA...
    if (gun->currentState == GunState::NO_SHOOTING_L || gun->currentState == GunState::SHOOTING_L)
    {
        // ...nós compensamos os 180 graus que já estão na arte.
        // Isso "desvira" o sprite para a orientação padrão antes de aplicar a rotação final.
        visualAngle += 180.0f;
    }

    animations->Draw(x, y, Layer::MIDDLE+0.1, scale, visualAngle);
}