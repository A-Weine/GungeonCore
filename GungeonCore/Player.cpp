/**********************************************************************************
// Player (C�digo Fonte)
// 
// Cria��o:     10 Out 2012
// Atualiza��o: 11 Nov 2021
// Compilador:  Visual C++ 2022
//
// Descri��o:   Define a classe jogador
//
**********************************************************************************/

#include "Player.h" 
#include "GungeonCore.h"
#include "Hud.h"
#include "Platform.h"
#include "Level1.h"
#include "Fire.h"
#include "DroppedItem.h"
#include "ScoreScreen.h"
#include "Gun.h"
#include "Empty.h"
#include "RandomMovementVillain.h"
#include "Explosion.h"
#include "Hand.h"
#include <cmath>

// -------------------------------------------------------------------------------

Player::Player()
{
    // inicializa controle
    //gamepad = new Controller();
    //gamepadOn = gamepad->Initialize();

    layer = CollisionLayer::PLAYER;

    sprite = new TileSet("Resources/player_sprite_sheet.png", 17, 27, 6, 54);
    animation = new Animation(sprite, 0.125f, true);

    uint seqRunningDown[6] = { 0, 1, 2, 3, 4, 5 }; // running down
    uint seqRunningDownR[6] = { 6, 7, 8, 9, 10, 11 }; // running down right
    uint seqRunningDownL[6] = { 12, 13, 14, 15, 16, 17 }; // running down left
    uint seqRunningUp[6] = { 18, 19, 20, 21, 22, 23 }; // running up
    uint seqRunningUpR[6] = { 24, 25, 26, 27, 28, 29 }; // running up right
    uint seqRunningUpL[6] = { 30, 31, 32, 33, 34, 35 }; // running up left
    uint seqIdleDown[3] = { 36, 37, 38 }; // idle down
    uint seqIdleDownR[3] = { 39, 40, 41 }; // idle down right
    uint seqIdleDownL[3] = { 42, 43, 44 }; // idle down left
    uint seqIdleUp[3] = { 45, 46, 47 }; // idle up
    uint seqIdleUpR[3] = { 48, 49, 50 }; // idle up right
    uint seqIdleUpL[3] = { 51, 52, 53 }; // idle up left

    animation->Add(static_cast<uint>(PlayerState::IDLE_DOWN), seqIdleDown, 3);
    animation->Add(static_cast<uint>(PlayerState::IDLE_DOWN_RIGHT), seqIdleDownR, 3);
    animation->Add(static_cast<uint>(PlayerState::IDLE_DOWN_LEFT), seqIdleDownL, 3);
    animation->Add(static_cast<uint>(PlayerState::IDLE_UP), seqIdleUp, 3);
    animation->Add(static_cast<uint>(PlayerState::IDLE_UP_RIGHT), seqIdleUpR, 3);
    animation->Add(static_cast<uint>(PlayerState::IDLE_UP_LEFT), seqIdleUpL, 3);
    animation->Add(static_cast<uint>(PlayerState::RUNNING_DOWN), seqRunningDown, 6);
    animation->Add(static_cast<uint>(PlayerState::RUNNING_DOWN_RIGHT), seqRunningDownR, 6);
    animation->Add(static_cast<uint>(PlayerState::RUNNING_DOWN_LEFT), seqRunningDownL, 6);
    animation->Add(static_cast<uint>(PlayerState::RUNNING_UP), seqRunningUp, 6);
    animation->Add(static_cast<uint>(PlayerState::RUNNING_UP_RIGHT), seqRunningUpR, 6);
    animation->Add(static_cast<uint>(PlayerState::RUNNING_UP_LEFT), seqRunningUpL, 6);

	/*spriteAiming = new TileSet("Resources/player_revolver_sprite_sheet.png", 40, 32, 7, 40);
	animationAiming = new Animation(spriteAiming, 0.095f, true);

	uint Seq9[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };
	uint Seq10[8] = { 8, 9, 10, 11, 12, 13, 14, 15 };

	animationAiming->Add(AIMING_RIGHT, Seq9, 8);
	animationAiming->Add(AIMING_LEFT, Seq10, 8);*/

    BBox(new Rect(-(sprite->TileWidth() / 2.0f), -(sprite->TileHeight() / 2.0f), (sprite->TileWidth() / 2.0f), (sprite->TileHeight() / 2.0f)));
    
    initialX = 96;
    initialY = 760;
    MoveTo((float)initialX, (float)initialY);
    type = PLAYER;

    jumpStrength = 7.5f;
    previousY = y;

    // ITENS
    itemEquiped = 0;
    Empty* empty = new Empty();
    for (int i = 0; i < NWEAPONS; i++) {
        inventory[i] = empty;
    }
    
    //hasMagnum = false;
    //hasGUN= false;

    gamepad = new Controller();
    gamepadOn = gamepad->XboxInitialize();

    
}

// -------------------------------------------------------------------------------

Player::~Player()
{
    delete sprite;
    delete animation;
    delete gamepad;
}

// -------------------------------------------------------------------------------

void Player::Update()
{
    inventarioAnterior = itemEquiped;

    animation->NextFrame();
    
    if (inventory[itemEquiped]->type == HAND) {
        Hand* hand = dynamic_cast<Hand*>(inventory[itemEquiped]);
        hand->gun->reload();
    }

    previousY = y;
    float accel = 30.0f * gameTime;
    float maxSpeed = 3.0f;
    float friction = 0.95f;

    Vector moveDirection;

    bool isMovingUp = false;
    bool isMovingDown = false;
    bool isMovingLeft = false;
    bool isMovingRight = false;

    // Keyboard movement
    if (window->KeyDown('W')) { moveDirection.Add(Vector(90.0f, 1.0f)); isMovingUp = true; }
    if (window->KeyDown('S')) { moveDirection.Add(Vector(270.0f, 1.0f)); isMovingDown = true; }
    if (window->KeyDown('A')) { moveDirection.Add(Vector(180.0f, 1.0f)); isMovingLeft = true; }
    if (window->KeyDown('D')) { moveDirection.Add(Vector(0.0f, 1.0f)); isMovingRight = true; }

    // Controller movement
    static bool rightStickWasActive = false;

    if (gamepadOn && gamepad->XboxUpdateState()) {
        const float deadzone = 8000.0f;
        float lx = static_cast<float>(gamepad->XboxAnalog(ThumbLX));
        float ly = static_cast<float>(gamepad->XboxAnalog(ThumbLY));

        // Normalize for XInput range
        float normLX = lx / 32768.0f;
        float normLY = -ly / 32768.0f;

        // Apply deadzone and use normalized values
        if (fabs(lx) > deadzone || fabs(ly) > deadzone) {
            if (normLX > 1.0f) normLX = 1.0f;
            if (normLX < -1.0f) normLX = -1.0f;
            if (normLY > 1.0f) normLY = 1.0f;
            if (normLY < -1.0f) normLY = -1.0f;
            moveDirection.Add(Vector(0.0f, normLX));
            moveDirection.Add(Vector(90.0f, -normLY));

            if (normLY < -0.2f) isMovingUp = true;
            if (normLY > 0.2f)  isMovingDown = true;
            if (normLX < -0.2f) isMovingLeft = true;
            if (normLX > 0.2f)  isMovingRight = true;
        }

        // --- Right stick shooting
        float rx = static_cast<float>(gamepad->XboxAnalog(ThumbRX));
        float ry = static_cast<float>(gamepad->XboxAnalog(ThumbRY));
        const float shootDeadzone = 8000.0f;

        float normRX = rx / 32768.0f;
        float normRY = -ry / 32768.0f;

        if ((fabs(rx) > shootDeadzone || fabs(ry) > shootDeadzone)) {
            if (!rightStickWasActive) {
                rightStickWasActive = true;
                float angleRad = atan2f(-normRY, normRX);
                float angleDeg = angleRad * 180.0f / 3.14159265f;
                if (inventory[itemEquiped]->type == HAND) {
                    Hand* hand = dynamic_cast<Hand*>(inventory[itemEquiped]);
                    if (hand->gun && !hand->gun->reloading) {
                        hand->gun->shoot(this, GUN, new Image("Resources/Explo.png"), angleDeg);
                    }
                }
            }
        } else {
            rightStickWasActive = false;
        }

        // Gun swap
        if (gamepad->XboxButton(LeftBumper)) {
            if (!gunSwapMade) {
                gunSwapMade = true;
                int originalGun = itemEquiped;
                if (inventory[itemEquiped]->type == HAND) {
                    Hand* hand = dynamic_cast<Hand*>(inventory[itemEquiped]);
                    if (!hand->gun->reloading) {
                        // Swap guns
                        do {
                            itemEquiped += 1;
                            itemEquiped %= NWEAPONS;

                            if (itemEquiped == originalGun) {
                                // Looped though all guns, didnt find any
                                break;
                            }
                        } while (inventory[itemEquiped]->type != HAND);

                    }
                }
            }
		} else {
            // Button released
            gunSwapMade = false;
        }
    }

    if (moveDirection.Magnitude() > 0.1f)
    {
        moveDirection.ScaleTo(1.0f);
        moveDirection.Scale(accel);
        speed.Add(moveDirection);
    }
    else
    {
        speed.Scale(friction * gameTime);
    }

    if (speed.Magnitude() > maxSpeed)
    {
        speed.ScaleTo(maxSpeed);
    }

    float angle = speed.Angle();
    bool isMoving = isMovingUp || isMovingDown || isMovingLeft || isMovingRight;

    if (isMovingUp) {
        lastVFacing = VerticalFacing::UP;
    }
    else if (isMovingDown) {
        lastVFacing = VerticalFacing::DOWN;
    }

    if (isMoving)
    {
        if (isMovingUp) {
            if (isMovingLeft)       state = PlayerState::RUNNING_UP_LEFT;
            else if (isMovingRight) state = PlayerState::RUNNING_UP_RIGHT;
            else                    state = PlayerState::RUNNING_UP;
        }
        else if (isMovingDown) {
            if (isMovingLeft)       state = PlayerState::RUNNING_DOWN_LEFT;
            else if (isMovingRight) state = PlayerState::RUNNING_DOWN_RIGHT;
            else                    state = PlayerState::RUNNING_DOWN;
        }
        else {
            if (isMovingLeft) {
                state = (lastVFacing == VerticalFacing::UP) ? PlayerState::RUNNING_UP_LEFT : PlayerState::RUNNING_DOWN_LEFT;
            }
            else if (isMovingRight) {
                state = (lastVFacing == VerticalFacing::UP) ? PlayerState::RUNNING_UP_RIGHT : PlayerState::RUNNING_DOWN_RIGHT;
            }
        }
    }
    else
    {
        if (state == PlayerState::RUNNING_UP)             state = PlayerState::IDLE_UP;
        else if (state == PlayerState::RUNNING_UP_LEFT)   state = PlayerState::IDLE_UP_LEFT;
        else if (state == PlayerState::RUNNING_UP_RIGHT)  state = PlayerState::IDLE_UP_RIGHT;
        else if (state == PlayerState::RUNNING_DOWN)      state = PlayerState::IDLE_DOWN;
        else if (state == PlayerState::RUNNING_DOWN_LEFT) state = PlayerState::IDLE_DOWN_LEFT;
        else if (state == PlayerState::RUNNING_DOWN_RIGHT)state = PlayerState::IDLE_DOWN_RIGHT;
    }

    animation->Select(static_cast<uint>(state));
    
    // -----------------
    // Controle
    // -----------------

    

    // -----------------
    // Teclado
    // -----------------
    //

    

    if (window->KeyPress('I')) {
        health = 10000;

        /*Gun* revolver = new Gun(0.3f, 6, 1.5f, Guntype::DEFAULTGUN, 320.0f, 12);
        itemEquiped = 0;
        inventory[itemEquiped] = revolver;

        Gun* metralhadora = new Gun(0.1f, 20, 1.5f, Guntype::UNFINISHEDGUN, 200.0f, 8);
        itemEquiped = 1;
        inventory[itemEquiped] = metralhadora;

        Gun* armaBomba = new Gun(1.5f, 3, 2.0f, Guntype::BOMBGUN, 80.0f, 60.0f);
        itemEquiped = 2;
        inventory[itemEquiped] = armaBomba;*/

        itemEquiped = 0;
    }

    if (window->KeyPress('1')) {
        if (inventory[0]->type == HAND) {
            Hand* hand = dynamic_cast<Hand*>(inventory[itemEquiped]);
            if (!hand->gun->reloading) {
                itemEquiped = 0;
                GungeonCore::level->GetScene()->Add(hand, STATIC);
            }

        }
    }
    if (window->KeyPress('2')) {
        if (inventory[1]->type == HAND) {
            Hand* hand = dynamic_cast<Hand*>(inventory[itemEquiped]);
            if (!hand->gun->reloading) {
                itemEquiped = 1;
                GungeonCore::level->GetScene()->Add(hand, STATIC);
            }
        }
    }
    if (window->KeyPress('3')) {
        if (inventory[2]->type == HAND) {
            Hand* hand = dynamic_cast<Hand*>(inventory[itemEquiped]);
            if (!hand->gun->reloading) {
                itemEquiped = 2;
                GungeonCore::level->GetScene()->Add(hand, STATIC);
        }
        }
    }

	// (clique esquerdo do mouse)
    if (window->KeyDown(VK_LBUTTON)) {
        
        // Momentâneo enquanto não há armas específicas do jogo
        if (inventory[itemEquiped]->type == HAND) {
            Hand* hand = dynamic_cast<Hand*>(inventory[itemEquiped]);
            hand->gun->shoot(this,GUN,new Image("Resources/Explo.png"));
        }
    }       

    Translate(speed.XComponent() * 50 * gameTime, -speed.YComponent() * 50 * gameTime);

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

void Player::Reset()
{
    health = MAX_HEALTH;
    MoveTo((float) initialX, (float) initialY);

    bombGunOwner = false;
    metralhadoraOwner = false;

    Empty* empty = new Empty();

    for (int i = 0; i < NWEAPONS; i++) {
        Hand* hand = dynamic_cast<Hand*>(inventory[i]);
        if (hand != nullptr) {
            hand->gun->Reset();
        }    
        inventory[i] = empty;
    }
    itemEquiped = 0;
}

// ---------------------------------------------------------------------------------

void Player::Draw()
{
    animation->Draw(x, y, Layer::MIDDLE, 1.0f, 0);

    //if (hasMagnum)
    //{
    //    // Pega a posição do ombro para "ancorar" o braço
    //    float shoulderX = x;      // Ajuste fino se necessário
    //    float shoulderY = y - 5;  // Ajuste fino para alinhar com o ombro

    //    // Desenha o sprite do braço, passando a posição do ombro e o ângulo calculado
    //    // A rotação é passada em graus. A função Draw do Sprite/Animation converte para radianos.
    //    animationAiming->Draw(shoulderX, shoulderY, Layer::UPPER, 1.0f, armAngle);
    //    // ^^^^ Uma camada na frente do corpo
    //}
}

void Player::OnCollision(Object* obj)
{
    
    if (obj->Type() == RANDOMMOVEMENTVILLAIN) {
        RandomMovementVillain* villain = dynamic_cast<RandomMovementVillain*>(obj);

        Explosion* explosao = new Explosion(villain->X(), villain->Y());
        GungeonCore::level->GetScene()->Add(explosao, STATIC);
        GungeonCore::level->GetScene()->Delete(villain->batShadow, STATIC);
        GungeonCore::level->GetScene()->Delete(villain, MOVING);
    }
    else if (obj->Type() == CHASEVILLAIN) {
        ChaseVillain* villain = dynamic_cast<ChaseVillain*>(obj);
        
        if (villain->state != ChaseVillainState::EXPLODING) {
        TakeDamage(10);
        }
    }
    else if (obj->Type() == RUNAWAYVILLAIN) {
        TakeDamage(10);
    }
    else if (obj->Type() == DROPPEDITEM) {
        DroppedItem* droppedItem = dynamic_cast<DroppedItem*>(obj);


        if (droppedItem->itemType == GUN) {
            GungeonCore::audio->Play(GRAB_ITEM);
            Gun* gun = dynamic_cast<Gun*>(droppedItem->obj);
            Hand* hand = new Hand(GungeonCore::player, gun->sprite, gun->animation, gun);
            if (gun->gunTypes == Guntype::DEFAULTGUN) {
                itemEquiped = 0;
            }
            
            if (gun->gunTypes == Guntype::UNFINISHEDGUN) {
                itemEquiped = 1;
            }

            if (gun->gunTypes == Guntype::BOMBGUN) {
                itemEquiped = 2;
            }
            GungeonCore::level->GetScene()->Add(hand, STATIC);

            inventory[itemEquiped] = hand;
        }

        /*if (droppedItem->itemType == KEY) {
            GungeonCore::audio->Play(GRAB_KEY);
            if (hasKey1) {
                hasKey2 = true;
            }
            hasKey1 = true;
        }*/

        if (droppedItem && droppedItem->itemShadow)
        {
            GungeonCore::level->GetScene()->Delete(droppedItem->itemShadow, STATIC);
        }

        GungeonCore::level->GetScene()->Delete(obj, STATIC);
    }
    else if (obj->Type() == EXPLOSION) {
        TakeDamage(20);
    }
    else if (obj->Type() == ENEMYFIRE) {
        Fire* fire = dynamic_cast<Fire*>(obj);

        if (fire != nullptr) {
        TakeDamage((int)fire->damage);    
        }
        else {
        TakeDamage(10);    
        }
    }
    else if (obj->Type() == PLATFORM) {
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

    /*if (obj->Type() == HITBOX) {
        Hitbox* hitbox = dynamic_cast<Hitbox*>(obj);

        if (hitbox)
        {
            if (hitbox->IsActive() && !hitbox->IsDamageDealt())
            {
                this->TakeDamage(10);

                hitbox->setDamageDealt(true);

                GungeonCore::level->GetScene()->Delete(hitbox, MOVING);
            }
        }
    }*/
}

// -------------------------------------------------------------------------------

//bool Player::KeysTimed(bool pressed, float time)
//{
//    if (keysCtrl) {
//        if (pressed) {
//            keysCtrl = false;
//            start = timerMagnum.Stamp();
//            return true;
//        }
//    }
//    else if (timerMagnum.Elapsed(start, time)) {
//        keysCtrl = true;
//    }
//
//    return false;
//}

// -------------------------------------------------------------------------------

//bool Player::AxisTimed(int axisX, int axisY, float time)
//{
//    // implementar se for usar controle
//    return false;
//}

// -------------------------------------------------------------------------------

void Player::TakeDamage(int damage)
{
    if (takeDamage && invicibilityTimer.Elapsed() >= invicibilityTime) {
        GungeonCore::audio->Play(TAKE_DAMAGE);

        health -= damage;
        if (health <= 0)
        {
            health = 0;
        }

		invicibilityTimer.Reset();
    }
}

void Player::TakeDamage(bool take) {
    takeDamage = (take) ? true : false;
}
