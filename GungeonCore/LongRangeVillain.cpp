#pragma once
#include "LongRangeVillain.h"
#include "GungeonCore.h"
#include "Hud.h" // If used for logging/display
#include "Platform.h" // If collision with platforms is still relevant
#include "Level1.h"
#include "BoneProjectile.h"
#include "Scene.h"
#include <windows.h>
#include <sstream>

// --- LongRangeVillain Constructor & Destructor ---

Player*& LongRangeVillain::player = GungeonCore::player;          // referência para o player
bool LongRangeVillain::toAttack = true;

LongRangeVillain::LongRangeVillain(float x, float y, Scene * scene)
{
    layer = CollisionLayer::ENEMY;
    // The base Villain constructor runs first implicitly.
    // Assume Villain's constructor initializes 'sprite' and 'animation' to nullptr
    // or a default. If Villain *allocates* its own, you might need to delete them here
    // before re-allocating for LongRangeVillain, as discussed in previous responses.
    // For simplicity, we'll assume the base class is properly managing its members,
    // and we're just re-assigning the inherited pointers.

    sprite = new TileSet("Resources/long_range_sprite_sheet.png", 30, 45, 6, 36);
    animation = new Animation(sprite, 0.095f, false);
    previousY = y;

	uint Seq1[1] = { 0 }; // STOPPED_RIGHT
    uint Seq2[18] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 }; // ATTACKING_RIGHT
	uint Seq3[1] = { 18 }; // STOPPED_LEFT
	uint Seq4[18] = { 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35 }; // ATTACKING_LEFT
   
    animation->Add(STOPPED_RIGHT, Seq1, 1);
    animation->Add(STOPPED_LEFT, Seq3, 1);
	animation->Add(ATTACKING_RIGHT, Seq2, 18);
	animation->Add(ATTACKING_LEFT, Seq4, 18);
    animation->Select(STOPPED_RIGHT); // Set initial animation state

    initialX = x;
    initialY = y;
    BBox(new Rect(-(sprite->TileWidth() / 2.0f) + 4, -(sprite->TileHeight() / 2.0f) + 5, (sprite->TileWidth() / 2.0f) - 4, (sprite->TileHeight() / 2.0f)));
	this->MoveTo(x, y); // Set initial position

    sinkAmount = 6.0f; // Still relevant if it stands on platforms
    attackCooldownTimer.Reset();

    // Initialize state machine
    state = STOPPED_RIGHT;
    currentMachineState = IDLE_OR_PATROLLING;

    // Movement vectors are no longer managed by this class directly if stationary.
    // If they exist in Villain, they'll be there, but won't be actively used for movement.
    // speedX.ScaleTo(0.0f);
    // speedY.ScaleTo(0.0f);
     previousY = y; // Still needed for platform collision logic (from OnCollision)

     villainShadow = new Shadow(this, scene);
     scene->Add(villainShadow, STATIC);
}

LongRangeVillain::~LongRangeVillain()
{
    delete sprite;
    delete animation;
}

void LongRangeVillain::OnCollision(Object* obj)
{
    if (obj->Type() == SOFTPLATFORM)
    {
        // trata geometria como rect
        Rect* villainRect = static_cast<Rect*>(this->BBox());
        Rect* platformRect = static_cast<Rect*>(obj->BBox());

        // verifica validade
        if (!villainRect || !platformRect)
            return;

        // posição da base do player no frame anterior
        float previousVillainBottom = previousY + villainRect->bottom;

        // topo da plataforma
        float platformTop = platformRect->Top();

        // verifica se jogador está caindo
        bool isFalling = speedY.YComponent() < 0;

        // verifica se o jogador estava acima da plataforma no frame anterior
        bool wasAbove = previousVillainBottom <= platformTop + sinkAmount;

        // só reposiciona o jogador se ele estava em uma posição acima da plataforma e está caindo
        if (isFalling && wasAbove)
        {
            // cálcula posição exata para reposicionamento do jogador
            float correctY = platformTop - villainRect->bottom;
            float finalY = correctY + sinkAmount;

            MoveTo(x, finalY);

            // zera velocidade vertical (para aceleração da gravidade)
            speedY.ScaleTo(0.0f);

            // pula somente se estiver colidindo com plataforma
            /*if (window->KeyDown('W'))
            {
                speedY.Add(Vector(90.0f, jumpStrength));
            }*/
        }
    }

    if (obj->Type() == SOLIDPLATFORM) {
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
        float playerCenterY = (playerRect->Top() - sinkAmount) + playerHeight / 2.0f;
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
                speedX.ScaleTo(0.0f);
            }
            else
            {
                // VERTICAL COLLISION
                if (diffY > 0)
                {
                    // Hitting from below (No sinking)
                    MoveTo(x, y + overlapY);

                    // Stop vertical movement
                    speedY.ScaleTo(0.0f);
                }
                else
                {
                    // MODIFIED PART: Landing on top (Apply sinking)
                    MoveTo(x, y - overlapY);

                    // Stop vertical movement
                    speedY.ScaleTo(0.0f);

                    // pula somente se estiver colidindo com plataforma
                    /*if (window->KeyDown('W'))
                    {
                        speedY.Add(Vector(90.0f, jumpStrength));
                    }*/
                }

            }
        }
    }

    if (obj->Type() == ENEMYFIRE) {
        return;
    }

    if (obj->Type() == FIRE) {
        GungeonCore::audio->Play(FIRE_HIT_VILLAIN);
        TakeDamage(10);
        GungeonCore::level->GetScene()->Delete(obj, MOVING);
    }
}

void LongRangeVillain::Reset()
{
    MoveTo((float) initialX, (float) initialY); // Reset position
    currentMachineState = IDLE_OR_PATROLLING;
    state = STOPPED_RIGHT;
    attackCooldownTimer.Reset();
    // Re-initialize any LongRangeVillain specific states here if needed
}

void LongRangeVillain::Update()
{
    previousY = y; // Keep previousY for collision detection (if OnCollision is still processing vertical)

    animation->NextFrame(); // Update animation frame

    // The animation state is solely controlled by MachineState for facing and attack
    // Remove complex 'if' blocks for walking, as it doesn't walk.
    if (state == STOPPED_RIGHT) {
        animation->Select(STOPPED_RIGHT);
    }
    else if (state == STOPPED_LEFT) {
        animation->Select(STOPPED_LEFT);
    }
    else if (state == ATTACKING_RIGHT) {
        animation->Select(ATTACKING_RIGHT);
    }
    else if (state == ATTACKING_LEFT) {
        animation->Select(ATTACKING_LEFT);
	}
    // Add logic for ATTACKING animation if you implement it:
    // else if (state == ATTACKING) {
    //     animation->Select(ATTACKING);
    // }


    this->MachineState(); // Run the AI state machine

    // Apply gravity (LongRangeVillains still fall if no platform)
    // Assuming 'gameTime' is accessible globally or passed.
    // speedY member (from Villain) can still be used for falling.
    float gravityForce = 19.6f * gameTime;
    // Add gravity vector to speedY, assuming Villain::Translate handles final position.
    if (speedY.Magnitude() < 15.0f) // Prevent infinite acceleration
        speedY.Add(Vector(270.0f, gravityForce));

    // Use Villain's Translate to update position based on its speed vectors.
    // If speedX is always 0, it won't move horizontally from this.
    Translate(speedX.XComponent() * gameTime, -speedY.YComponent() * 50 * gameTime); // Assuming gameTime factor

    // Restrict to game area (common for all villains)
    if (x < 0)
        MoveTo(0, y);
    if (y < 0)
        MoveTo(x, 0);
    if (x > game->Width())
        MoveTo(game->Width(), y);
    if (y > game->Height())
        MoveTo(x, game->Height());
}

void LongRangeVillain::Draw()
{
    // Delegate drawing to the animation component.
    if (animation) {
        animation->Draw(x, y, Layer::MIDDLE, 1.0f, 0);
    }
}

// --- AI State Machine and Handlers (Modified for stationary behavior) ---

bool LongRangeVillain::CanSeePlayer(Vector direction, float range)
{
    RaycastHit hit;
    Point origin(x, y);

    unsigned int mask = static_cast<unsigned int>(CollisionLayer::PLAYER) |
        static_cast<unsigned int>(CollisionLayer::SOLIDPLATFORM);

    if (GungeonCore::level->GetScene()->Raycast(origin, direction, range, hit, mask, this))
    {
        return (hit.objectHit->layer == CollisionLayer::PLAYER);
    }

    return false;
}

void LongRangeVillain::MachineState()
{
    if (toAttack) {
        float playerX = GungeonCore::player->X();
        float playerY = GungeonCore::player->Y();
        float distanceToPlayer = Point::Distance(Point(playerX, playerY), Point(x, y));

        playerSpotted = false; // começa assumindo que não viu o jogador

        if (distanceToPlayer <= LONG_VISION_RANGE)
        {
            float angleToPlayer = Line::Angle(Point(x, y), Point(playerX, playerY));
            angleToPlayer = -angleToPlayer;

            Vector directionToPlayer(angleToPlayer, 1.0f);

            if (CanSeePlayer(directionToPlayer, distanceToPlayer))
            {
                playerSpotted = true;
            }
        }

        // Determine facing direction based on player's relative X position
        if (playerX < x) {
            state = STOPPED_LEFT; // Face left
        } else {
            state = STOPPED_RIGHT; // Face right
        }

        std::stringstream text;

        switch (currentMachineState) {
        case IDLE_OR_PATROLLING:
            // In IDLE_OR_PATROLLING, a LongRangeVillain just stands still, facing the player
            // or perhaps a default direction if player isn't in range.
            // No actual "patrolling" movement for a stationary villain.
            HandlePatrolling(); // This will simply set velocity to zero.

            if (playerSpotted) {
                currentMachineState = PURSUING; // Transition to pursuing (even if just standing still and aiming)
                text << "LongRangeVillain: Player detected! Pursuing!\n";
                OutputDebugString(text.str().c_str());
            }
            break;

        case PURSUING:
            // In PURSUING, a LongRangeVillain simply faces the player. No actual movement.
            HandlePursuing(); // This will simply set velocity to zero.

            if (playerSpotted) {
                currentMachineState = ATTACKING;
                text << "LongRangeVillain: Target in range! Attacking!\n";
                OutputDebugString(text.str().c_str());
            } else if (distanceToPlayer > LONG_VISION_RANGE * 1.5f) { // Player got too far
                currentMachineState = IDLE_OR_PATROLLING;
                text << "LongRangeVillain: Player too far. Returning to idle.\n";
                OutputDebugString(text.str().c_str());
            }
            break;

        case ATTACKING:
            // In ATTACKING, the villain fires its projectile.
            HandleAttacking();

            if (animation->Inactive()) {
                if (distanceToPlayer > LONG_ATTACK_RANGE) {
                    currentMachineState = PURSUING;
                    text << "LongRangeVillain: Player moved away. Resuming pursuit!\n";
                    OutputDebugString(text.str().c_str());
                }
            }
            break;
        }
    }
}

void LongRangeVillain::HandlePatrolling() {
    // A stationary long-range villain simply stands still.
    // The facing direction is already handled by MachineState.
    // We explicitly ensure no horizontal movement.
    speedX.ScaleTo(0.0f);
}

void LongRangeVillain::HandlePursuing() {
    // A stationary long-range villain simply faces the player.
    // The facing direction is already handled by MachineState.
    // We explicitly ensure no horizontal movement.
    speedX.ScaleTo(0.0f);
}

void LongRangeVillain::HandleAttacking() {
    // Ensure the villain is not moving while attacking.
    speedX.ScaleTo(0.0f);

    // The 'state' (STOPPED_RIGHT/LEFT) for facing is set by MachineState.
    // If you have a specific ATTACKING animation, set it here for the duration of the attack:
    // animation->Select(ATTACKING);
    if (GungeonCore::player->X() < x)
    {
        state = ATTACKING_LEFT;
    }
    else
    {
        state = ATTACKING_RIGHT;
    }

    if (attackCooldownTimer.Elapsed() >= attackCooldownDuration && playerSpotted) {
        animation->Restart();
        // Implement projectile spawning logic here
        // Example: GungeonCore::SpawnProjectile(x, y, targetAngle);

        // 1. Pega a posi��o do vilão no mundo
        Point villainPos(this->X(), this->Y());

        // 2. USA NOSSA NOVA FUN��O para pegar a posi��o do mouse NO MUNDO
        Point playerWorldPos = Point(GungeonCore::player->X(), GungeonCore::player->Y());

        // 3. Calcula o �ngulo entre dois pontos no MESMO sistema de coordenadas (Mundo)
        float firingAngle = Line::Angle(villainPos, playerWorldPos);

        GungeonCore::audio->Play(LONGRANGE_ATTACK);
        Level1::scene->Add(new BoneProjectile(this, firingAngle), MOVING);
        //GungeonCore::player->TakeDamage(10); // Placeholder: direct damage for now

        std::stringstream text;
        text << "LongRangeVillain: ATTACK EXECUTED! Player took 10 damage!\n";
        OutputDebugString(text.str().c_str());
        attackCooldownTimer.Reset();
    }
}

void LongRangeVillain::TakeDamage(int damage)
{
    // reduz vida do jogador
    health -= damage;
    // se a vida for menor ou igual a zero, o jogador morre
    if (health <= 0)
    {
        health = 0;
        GungeonCore::audio->Play(VILLAIN_DIES);
        GungeonCore::level->GetScene()->Delete(villainShadow, STATIC);
        GungeonCore::level->GetScene()->Delete(this, MOVING);
        /*GungeonCore::hud->SetGameOver(true);
        GungeonCore::hud->SetGameWon(false);*/
    }
}
