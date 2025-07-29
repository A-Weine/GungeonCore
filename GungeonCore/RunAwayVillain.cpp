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
#include "EggProjectile.h"
#include <sstream>


RunAwayVillain::RunAwayVillain(float pX, float pY, Player* p) 
{
	player = p;
	sprite = new TileSet("Resources/gigi_sprite_sheet.png", 38, 38, 8, 50);
    animation = new Animation(sprite, 0.125f, true);

    uint flyRSeq[4] = { 0, 1, 2, 3 };
    uint flyLSeq[4] = { 4, 5, 6, 7 };
    uint shootRSeq[12] = { 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
    uint shootLSeq[12] = { 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };
    uint hitRSeq[4] = { 32, 33, 34, 35 };
    uint hitLSeq[4] = { 36, 37, 38, 39 };
    uint deathRSeq[5] = { 40, 41, 42, 43, 44 };
    uint deathLSeq[5] = { 45, 46, 47, 48, 49 };

    animation->Add(static_cast<uint>(RunAwayVillainState::FLYINGRIGHT), flyRSeq, 4);
    animation->Add(static_cast<uint>(RunAwayVillainState::FLYINGLEFT), flyLSeq, 4);
    animation->Add(static_cast<uint>(RunAwayVillainState::SHOOTINGRIGHT), shootRSeq, 12);
    animation->Add(static_cast<uint>(RunAwayVillainState::SHOOTINGLEFT), shootLSeq, 12);
    animation->Add(static_cast<uint>(RunAwayVillainState::HITRIGHT), hitRSeq, 4);
    animation->Add(static_cast<uint>(RunAwayVillainState::HITLEFT), hitLSeq, 4);
    animation->Add(static_cast<uint>(RunAwayVillainState::DYINGRIGHT), deathRSeq, 5);
    animation->Add(static_cast<uint>(RunAwayVillainState::DYINGLEFT), deathLSeq, 5);

    currentState = RunAwayVillainState::FLYINGRIGHT;
    animation->Select(static_cast<uint>(currentState));

    speed.RotateTo(0.0f);
    speed.ScaleTo(0.0f);

    MoveTo(pX, pY);

    type = RUNAWAYVILLAIN;

    BBox(new Rect(-(sprite->TileWidth() / 2.0f) + 9, -(sprite->TileHeight() / 2.0f) + 10, (sprite->TileWidth() / 2.0f) - 9, (sprite->TileHeight() / 2.0f) - 5));
}

RunAwayVillain::~RunAwayVillain()
{
    delete sprite;

}

void RunAwayVillain::Update()
{
    animation->NextFrame();

    switch (currentState)
    {
    case RunAwayVillainState::FLYINGRIGHT:
    case RunAwayVillainState::FLYINGLEFT:
    {
        float distance = Point::Distance(Point(x, y), Point(player->X(), player->Y()));

        Vector target = Vector(Line::Angle(Point(x, y), Point(player->X(), player->Y())), 100.0f * gameTime);

        if (distance < fleeDistance)
        {
            target.Rotate(180.0f);
            target.ScaleTo(150.0f * gameTime);
        }
        else if (distance > shootDistance)
        {
            target.ScaleTo(100.0f * gameTime);
        }
        else
        {
            target.ScaleTo(0.0f);
        }

        speed.Add(target);

        if (speed.Magnitude() > 4.0f)
            speed.ScaleTo(4.0f);

        if (player->X() < x) {
            currentState = RunAwayVillainState::FLYINGLEFT;
        }
        else {
            currentState = RunAwayVillainState::FLYINGRIGHT;
        }

        animation->Select(static_cast<uint>(currentState));


        if (attackTimer.Elapsed(attackCooldown) && distance >= fleeDistance && distance <= shootDistance)
        {
            speed.ScaleTo(0.0f);
            currentState = (player->X() < x) ? RunAwayVillainState::SHOOTINGLEFT : RunAwayVillainState::SHOOTINGRIGHT;
            animation->Select(static_cast<uint>(currentState));
            animation->Loop(false);
            animation->Restart();
            hasFiredInAttack = false;
        }
        break;
    }

    case RunAwayVillainState::SHOOTINGLEFT:
    case RunAwayVillainState::SHOOTINGRIGHT:
    {
        if (!hasFiredInAttack && (animation->Frame() == 17 || animation->Frame() == 30))
        {
            Vector eggSpeed(currentState == RunAwayVillainState::SHOOTINGRIGHT ? 45.0f : 135.0f, 10.0f);
            float xEgg = currentState == RunAwayVillainState::SHOOTINGRIGHT ? (x + 5.0f) : (x - 5.0f);

            GungeonCore::level->GetScene()->Add(new EggProjectile(this, player, eggSpeed), MOVING);

            hasFiredInAttack = true;
        }

        if (animation->Inactive())
        {
            currentState = (player->X() < x) ? RunAwayVillainState::FLYINGLEFT : RunAwayVillainState::FLYINGRIGHT;
            animation->Loop(true);
            attackTimer.Reset();
        }
        break;
    }

    case RunAwayVillainState::HITLEFT:
    case RunAwayVillainState::HITRIGHT:
    {
        // Fica parado por um curto período (stun)
        if (stunTimer.Elapsed(0.3f)) // Stun de 0.3 segundos
        {
            //currentState = (player->X() < x) ? RunAwayVillainState::FLYINGLEFT : RunAwayVillainState::FLYINGRIGHT;
            //animation->Loop(true);
        }
        break;
    }

    case RunAwayVillainState::DYINGLEFT:
    case RunAwayVillainState::DYINGRIGHT:
    {
        // Quando a animação de morte terminar, deleta o objeto
        if (animation->Inactive())
        {
            GungeonCore::level->GetScene()->Delete(this, MOVING);
        }
        break;
    }
    }

    // Aplica o movimento final
    Translate(speed.XComponent() * 15.0f * gameTime, -speed.YComponent() * 15.0f * gameTime);


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

void  RunAwayVillain::OnCollision(Object* obj) {
    if (obj->Type() == PLATFORM) {
        // Gets the bounding boxes for the player and the platform
        Rect* playerRect = static_cast<Rect*>(this->BBox());
        Rect* platformRect = static_cast<Rect*>(obj->BBox());

        // Ensure the bounding boxes are valid
        if (!playerRect || !platformRect)
            return;

        // --- 1. Calculate Dimensions and Centers ---

        float playerWidth = playerRect->Right() - playerRect->Left();
        float playerHeight = playerRect->Bottom() - playerRect->Top();
        float platformWidth = platformRect->Right() - platformRect->Left();
        float platformHeight = platformRect->Bottom() - platformRect->Top();

        float playerCenterX = playerRect->Left() + playerWidth / 2.0f;
        float playerCenterY = (playerRect->Top()) + playerHeight / 2.0f;
        float platformCenterX = platformRect->Left() + platformWidth / 2.0f;
        float platformCenterY = platformRect->Top() + platformHeight / 2.0f;

        // --- 2. Calculate Overlap ---

        float diffX = playerCenterX - platformCenterX;
        float diffY = playerCenterY - platformCenterY;

        float combinedHalfWidths = playerWidth / 2.0f + platformWidth / 2.0f;
        float combinedHalfHeights = playerHeight / 2.0f + platformHeight / 2.0f;

        float overlapX = combinedHalfWidths - abs(diffX);
        float overlapY = combinedHalfHeights - abs(diffY);

        // --- 3. Resolve Collision ---

        if (overlapX > 0 && overlapY > 0)
        {
            if (overlapX < overlapY)
            {
                // HORIZONTAL COLLISION (No sinking)
                if (diffX > 0)
                {
                    MoveTo(x + overlapX, y);
                }
                else
                {
                    MoveTo(x - overlapX, y);
                }
                speed.ScaleTo(0.0f);
            }
            else
            {
                // VERTICAL COLLISION
                if (diffY > 0)
                {
                    // Hitting from below (No sinking)
                    MoveTo(x, y + overlapY);

                    // Stop vertical movement
                    speed.ScaleTo(0.0f);
                }
                else
                {
                    // MODIFIED PART: Landing on top (Apply sinking)
                    MoveTo(x, y - overlapY);

                    // Stop vertical movement
                    speed.ScaleTo(0.0f);
                }

            }
        }

    }
}

inline void RunAwayVillain::Draw()
{
    animation->Draw(x, y, Layer::MIDDLE, 1.0f, 0);
}

void RunAwayVillain::TakeDamage(int damage) {
    health -= damage;

    if (health <= 0)
    {
        GungeonCore::level->GetScene()->Delete(this, MOVING);
    }
}