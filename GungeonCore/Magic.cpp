// Em Magic.cpp
#include "Magic.h"
#include "Engine.h"
#include "MortalCellsRedemption.h"

// Sequências de animação (podem ser definidas aqui ou como membros estáticos)
uint summonSeq[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
uint movingSeq[6] = { 16, 17, 18, 19, 20, 21 };
uint explosionSeq[17] = { 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38 };


Magic::Magic(float x, float y, Player* playerTarget)
{
    type = ENEMYFIRE;
    layer = CollisionLayer::ENEMY; // Ou uma camada específica para projéteis

    target = playerTarget;
    state = SUMMONING; // Começa no estado de invocação

    tileset = new TileSet("Resources/magic_sprite_sheet.png", 58, 58, 10, 39); // Ajuste colunas/tiles

    // Animação NÃO dá loop, pois cada estado controla seu fim
    anim = new Animation(tileset, 0.08f, false);
    anim->Add(SUMMONING, summonSeq, 16);
    anim->Add(MOVING, movingSeq, 6);
    anim->Add(EXPLODING, explosionSeq, 17);

    anim->Select(SUMMONING); // Seleciona a primeira animação

    // BBox começa pequena e vai crescer
    BBox(new Circle(5.0f));

    MoveTo(x, y);
    stateTimer.Start();
}

Magic::~Magic()
{
    delete tileset;
    delete anim;
}

void Magic::Update()
{
    // Chama o handler do estado apropriado
    switch (state)
    {
    case MagicState::SUMMONING:  HandleSummoning();  break;
    case MagicState::MOVING:     HandleMoving();     break;
    case MagicState::EXPLODING:  HandleExploding();  break;
    }
}

void Magic::HandleSummoning()
{
    anim->Select(SUMMONING);
    anim->NextFrame();

    // Aumenta a escala e a BBox junto com a animação
    float progress = float(anim->Frame()) / 15.0f; // Progresso de 0.0 a 1.0
    if (progress > 1.0f) progress = 1.0f;

    ScaleTo(progress);
    BBox()->ScaleTo(progress);

    // Quando a animação de invocação terminar...
    if (anim->Inactive())
    {
        state = MagicState::MOVING;
        anim->Select(MOVING);
        anim->Loop(true); // A animação de movimento PODE ter loop
        anim->Restart();
        stateTimer.Start(); // Inicia o contador de 3 segundos

        // Define o tamanho final da BBox de perseguição
        BBox(new Circle(33.0f / 2.0f));
    }
}

void Magic::HandleMoving()
{
    anim->NextFrame();

    // Lógica de perseguição (homing)
    if (target)
    {
        float angleToPlayer = Line::Angle(Point(x, y), Point(target->X(), target->Y()));
        speed.RotateTo(angleToPlayer);
        speed.ScaleTo(movingSpeed);
    }

    Translate(speed.XComponent() * gameTime, -speed.YComponent() * gameTime);

    // Se o tempo de perseguição acabou, explode
    if (stateTimer.Elapsed(movingDuration))
    {
        state = MagicState::EXPLODING;
        anim->Select(EXPLODING);
        anim->Loop(false); // Animação de explosão não tem loop
        anim->Restart();
        speed.ScaleTo(0.0f); // Para de se mover

        // Expande a BBox para o dano em área
        BBox(new Circle(58.0f / 2.0f));
    }
}

void Magic::HandleExploding()
{
    anim->NextFrame();

    // Quando a animação de explosão terminar...
    if (anim->Inactive())
    {
        // ...o objeto se marca para ser deletado pela cena.
        MortalCellsRedemption::level->GetScene()->Delete(this, MOVING);
    }
}

void Magic::OnCollision(Object* obj)
{
    // Se estiver se movendo e colidir com o jogador, explode
    if (state == MagicState::MOVING && obj->Type() == PLAYER)
    {
        state = MagicState::EXPLODING;
        anim->Select(EXPLODING);
        anim->animLoop(false);
        anim->Restart();
        speed.ScaleTo(0.0f);
        BBox(new Circle(58.0f / 2.0f));
    }

    // A lógica de dano pode acontecer aqui ou na colisão do Player
}

inline void Magic::Draw()
{
    // O Draw é simples, ele apenas desenha o quadro atual da animação selecionada.
    // A escala é controlada pelo `ScaleTo` no Update.
    anim->Draw(x, y, Layer::UPPER, scale, rotation);
}