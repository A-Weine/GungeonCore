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
#include "ShriekAttack.h"

// ------------------------------------------------------------------------------

// ------------------------------------------------------------------------------

ShriekAttack::ShriekAttack(ChaseVillain * zombier, boolean isConstants)
{
    sprite = new TileSet("Resources/shriek_sprite_sheet.png", 103, 102, 4, 16);
    animation = new Animation(sprite, 0.025f, false);
    animationConstant = new Animation(sprite, 0.025f, true);

    isConstant = isConstants;

    BBox(new Circle(attackSize));


    zombie = zombier;

    uint seqFast[13] = { 0, 1, 2 , 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    uint seqConstant[3] = { 13, 14, 15}; 

    type = ENEMYFIRE;

    if (!isConstant) {
        animation->Add(FASTATTACK, seqFast, 13);
        GungeonCore::audio->Play(ZOMBIE_SCREAM);
    }
    else {
        animationConstant->Add(CONSTANTATTACK, seqConstant, 3);
        GungeonCore::audio->Play(ZOMBIE_SCREAM);
    }

    
    attackTTL.Start();
    TTL = 0.6f;

    MoveTo(zombie->X(), zombie->Y()+7);
}

// ------------------------------------------------------------------------------

ShriekAttack::~ShriekAttack()
{
    delete sprite;

}

// -------------------------------------------------------------------------------

void ShriekAttack::Update()
{
    // move míssel com vetor resultante

    if (isConstant) {
        animationConstant->Select(FASTATTACK);
        animationConstant->NextFrame();
    }
    else {
        animation->Select(FASTATTACK);
        animation->NextFrame();
    }


    if (!isConstant && attackSize < 80)
        attackSize = attackSize + 160 * gameTime;



    // Lógica para o ataque constante em ondas (com pausa)
    if (isConstant)
    {
        switch (estadoAtualDaOnda)
        {
        case EstadoOnda::Crescendo:
            // Aumenta o tamanho
            attackSize = attackSize + 200 * gameTime;

            // Se atingiu o tamanho máximo...
            if (attackSize >= 100)
            {
                attackSize = 100; // Trava no máximo
                estadoAtualDaOnda = EstadoOnda::PausadoNoMaximo; // Muda para o estado de pausa
                timerPausa.Start(); // Inicia o timer da pausa
                GungeonCore::audio->Volume(ZOMBIE_SCREAM, 10.0f);
            }
            break;

        case EstadoOnda::PausadoNoMaximo:
            // Não faz nada com o tamanho, apenas espera o timer terminar
            if (timerPausa.Elapsed(0.1f))
            {
                estadoAtualDaOnda = EstadoOnda::Diminuindo; // Começa a diminuir
                GungeonCore::audio->Volume(ZOMBIE_SCREAM, 50.0f);
            }
            break;

        case EstadoOnda::Diminuindo:
            // Diminui o tamanho
            attackSize = attackSize - 250 * gameTime;

            // Se atingiu o tamanho mínimo...
            if (attackSize <= 0)
            {
                attackSize = 0; // Trava no mínimo
                estadoAtualDaOnda = EstadoOnda::PausadoNoMinimo; // Muda para o estado de pausa
                timerPausa.Start(); // Inicia o timer da pausa
            }
            break;

        case EstadoOnda::PausadoNoMinimo:
            // Não faz nada com o tamanho, apenas espera o timer terminar
            if (timerPausa.Elapsed(0.05f))
            {
                estadoAtualDaOnda = EstadoOnda::Crescendo; // Começa a crescer novamente
                GungeonCore::audio->Play(ZOMBIE_SCREAM);
            }
            break;
        }
    }

        BBox(new Circle(attackSize));

        Player* player = GungeonCore::player;
        if (!isConstant) {
            if (Point::Distance(Point(x, y), Point(player->X(), player->Y())) > zombie->distance || attackTTL.Elapsed(TTL))
            {
                GungeonCore::level->GetScene()->Delete(this, STATIC);
            }
        }
        else {
            if (Point::Distance(Point(x, y), Point(player->X(), player->Y())) > zombie->distance || attackTTL.Elapsed(3.2f))
            {
                GungeonCore::level->GetScene()->Delete(this, STATIC);
            }
        }
    
}
void ShriekAttack::OnCollision(Object* obj)
{

}
// -------------------------------------------------------------------------------

void ShriekAttack::Draw()
{
    animation->Draw(x, y, Layer::FRONT, 2.0f, 0);

    if (isConstant) {
        animationConstant->Draw(x, y, Layer::FRONT, 2.0f, 0);
    }
}