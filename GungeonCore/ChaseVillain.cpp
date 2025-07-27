

#include "ChaseVillain.h"
#include "GungeonCore.h"
#include "Random.h" 
#include "ShriekAttack.h"
#include "Explosion.h"


ChaseVillain::ChaseVillain(float pX, float pY, Player* p)
{
    player = p;

    sprite = new TileSet("Resources/bombshee_sprite_sheet.png", 34, 34, 6, 34);
    animation = new Animation(sprite, 0.150f, true);

    // Animation sequences based on the provided sprite sheet
    uint seqSpawning[3] = { 0, 1, 2 };          // de 0 a 2 é ele spawnando
    uint seqWalkDown[4] = { 3, 4, 5, 6 };       // de 3 a 6 é ele andando pra baixo (idle tb)
    uint seqWalkRight[4] = { 7, 8, 9, 10 };     // de 7 a 10 é ele andando pra direita (idle tb)
    uint seqWalkLeft[4] = { 11,12,13,14};   // de 11 a 14 é ele andando pra esquerda (idle tb)
    uint seqHit[4] = { 15,15, 15, 16 };                // de 15 a 16 é ele tomando hit
    uint seqQuickPower[5] = { 17, 18, 19, 20, 21 }; // de 17 a 21 é ele usando o poder rapido
    uint seqConstantPower[5] = { 22, 23, 24, 25, 26 }; // de 22 a 26 é ele usando o poder constantemente
    uint seqExploding[2] = { 27, 28 };          // de 27 a 28 é ele explodindo
    uint seqMissile[5] = { 29, 30, 31, 32, 33 }; // de 29 a 33 é ele em forma de missel

    // Assuming an enum 'ChaseVillain' exists for clarity
    // enum ChaseVillain { SPAWNING, WALKING_DOWN, WALKING_RIGHT, WALKING_LEFT,
    //                      HIT, QUICK_POWER, CONSTANT_POWER, EXPLODING, MISSILE };

    animation->Add(static_cast<uint>(ChaseVillainState::SPAWNING), seqSpawning, 3);
    animation->Add(static_cast<uint>(ChaseVillainState::WALKING_DOWN), seqWalkDown, 4);
    animation->Add(static_cast<uint>(ChaseVillainState::WALKING_RIGHT), seqWalkRight, 4);
    animation->Add(static_cast<uint>(ChaseVillainState::WALKING_LEFT), seqWalkLeft, 4);
    animation->Add(static_cast<uint>(ChaseVillainState::HIT), seqHit, 4);
    animation->Add(static_cast<uint>(ChaseVillainState::QUICK_POWER), seqQuickPower, 5);
    animation->Add(static_cast<uint>(ChaseVillainState::CONSTANT_POWER), seqConstantPower, 5);
    animation->Add(static_cast<uint>(ChaseVillainState::EXPLODING), seqExploding, 2);
    animation->Add(static_cast<uint>(ChaseVillainState::MISSILE), seqMissile, 5);


    speed.RotateTo(0.0f);
    speed.ScaleTo(0.0f);

    
    distance = 80;

    MoveTo(pX, pY);

    type = CHASEVILLAIN;

    BBox(new Rect(-9.0f, -13.5f, 9.0f, 18.5f));
}

ChaseVillain::~ChaseVillain()
{
    delete sprite;

}

void ChaseVillain::Update()
{
    animation->NextFrame();

    if (state ==  ChaseVillainState::EXPLODING) {
        if (explosion.Elapsed(0.0165f)) {
            Explosion* explosion = new Explosion(this->X(),this->Y());
            GungeonCore::level->GetScene()->Add(explosion, STATIC);
            GungeonCore::level->GetScene()->Delete(this, MOVING);
        }
    }
    else {
        // a magnitude do vetor target controla quão 
        // rápido o objeto converge para a direção do alvo
        Vector target = Vector(Line::Angle(Point(x, y), Point(player->X(), player->Y())), 100.0f * gameTime);

        speed.Add(target);

        // limita a magnitude da velocidade para impedir 
        // seu crescimento indefinido na soma vetorial
        if (speed.Magnitude() > 8)
            speed.ScaleTo(8.0f);


        playerIsInRange = (Point::Distance(Point(x, y), Point(player->X(), player->Y())) < distance);

        if (currentShriekState != ShriekState::IDLE && currentShriekState != ShriekState::BULLET) {
            speed.ScaleTo(0);
        }

        // 2. A Máquina de Estados
        switch (currentShriekState)
        {
        case ShriekState::IDLE:
            if (playerIsInRange) {
                currentShriekState = ShriekState::WINDUP;
                stateTimer.Reset(); // Inicia a contagem para a preparação
                // Tocar som de preparação, etc.
            }
            break;

        case ShriekState::WINDUP:
            if (!playerIsInRange) {
                ResetShriekAttack();
                break;
            }

            // A mágica acontece aqui: uma verificação limpa e direta.
            if (stateTimer.Elapsed(WINDUP_TIME)) {
                shriekPulseCount = 1;
                ShriekAttack* shriek = new ShriekAttack(this, false);
                GungeonCore::level->GetScene()->Add(shriek, STATIC);
                state = ChaseVillainState::QUICK_POWER;
                currentShriekState = ShriekState::PULSE_SHRIEK;
                stateTimer.Reset();
            }
            break;

        case ShriekState::PULSE_SHRIEK:
            if (!playerIsInRange) {
                ResetShriekAttack();
                break;
            }

            // Verifica se a pausa de 1.5s terminou
            if (stateTimer.Elapsed(PAUSE_BETWEEN_SHRIEKS)) {
                shriekPulseCount++;
                if (shriekPulseCount <= 3) {
                    ShriekAttack* shriek = new ShriekAttack(this, false);
                    GungeonCore::level->GetScene()->Add(shriek, STATIC);
                    state = ChaseVillainState::QUICK_POWER;
                    stateTimer.Reset(); // Reinicia o timer para a próxima pausa
                }
                else {
                    currentShriekState = ShriekState::CONTINUOUS_SHRIEK;
                    ShriekAttack* shriek = new ShriekAttack(this, true);
                    state = ChaseVillainState::CONSTANT_POWER;
                    GungeonCore::level->GetScene()->Add(shriek, STATIC);
                    stateTimer.Reset();
                    
                }
            }
            break;

        case ShriekState::CONTINUOUS_SHRIEK:
            if (!playerIsInRange) {
                ResetShriekAttack();
                break;
            }
            if (stateTimer.Elapsed(3.0f)) {
                // New state
                currentShriekState = ShriekState::BULLET;
            }
            // O timer não é usado aqui, apenas o dano contínuo
            //ApplyContinuousDamage(deltaTime);
            break;

        case ShriekState::BULLET:
            
            state = ChaseVillainState::MISSILE;
            RotateTo(Line::Angle(Point(x, y),Point(GungeonCore::player->X(), GungeonCore::player->Y())));
            // O timer não é usado aqui, apenas o dano contínuo
            //ApplyContinuousDamage(deltaTime);
            break;

        case ShriekState::COOLDOWN:
            // Verifica se o tempo de recarga terminou

            currentShriekState = ShriekState::IDLE;

            break;
        }

        /*animation->Add(static_cast<uint>(ChaseVillainState::SPAWNING), seqSpawning, 3);
        animation->Add(static_cast<uint>(ChaseVillainState::MISSILE), seqMissile, 5);*/

        if (speed.Magnitude() > 0.0f && currentShriekState != ShriekState::BULLET) {
            if (GungeonCore::player->X() > X()) {
                state = ChaseVillainState::WALKING_RIGHT;
            }
            else {
                state = ChaseVillainState::WALKING_LEFT;
            }
            if (GungeonCore::player->Y() > Y() + 20) {
                state = ChaseVillainState::WALKING_DOWN;
            }
        }

        animation->Select(static_cast<uint>(state));

        // move o objeto pelo seu vetor velocidade
        Translate(speed.XComponent() * 15.0f * gameTime, -speed.YComponent() * 15.0f * gameTime);
        Rotate(50 * gameTime);
    }
    

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

void ChaseVillain::OnCollision(Object* obj) {

    if (obj->Type() == PLAYER) {
        state = ChaseVillainState::EXPLODING;
        animation->Select(static_cast<uint>(state));
        explosion.Start();
    }
}

inline void ChaseVillain::Draw()
{
    if (state == ChaseVillainState::MISSILE) {
    animation->Draw(x, y, Layer::MIDDLE, 1.0f, -rotation);
    }
    else {
     animation->Draw(x, y, Layer::MIDDLE, 1.0f, 0);
    }
}

void ChaseVillain::TakeDamage(int damage) {
    health -= damage;

    state = ChaseVillainState::HIT;
    animation->Select(static_cast<uint>(state));

    if (health <= 0)
    {
        // Optionally play a sound or animation here
        GungeonCore::level->GetScene()->Delete(this, MOVING);
    }
}

void ChaseVillain::ResetShriekAttack() {
    shriekPulseCount = 0;
    currentShriekState = ShriekState::COOLDOWN;
    stateTimer.Reset(); 
}