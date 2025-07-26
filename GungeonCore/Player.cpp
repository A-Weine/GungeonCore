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
    MoveTo((float) initialX, (float) initialY);
    type = PLAYER;

    jumpStrength = 7.5f;
    previousY = y;
    sinkAmount = 6.0f;


    // ITENS
    itemEquiped = 0;
    Empty* empty = new Empty();
    for (int i = 0; i < 10; i++) {
        inventory[i] = empty;
    }
    
    //hasMagnum = false;
    //hasGUN= false;
}

// -------------------------------------------------------------------------------

Player::~Player()
{
    delete sprite;
    delete animation;
    //delete gamepad;
}

// -------------------------------------------------------------------------------

void Player::Update()
{
    animation->NextFrame();
    
    if (inventory[itemEquiped]->type == GUN) {
        Gun* gun = dynamic_cast<Gun*>(inventory[itemEquiped]);
        gun->reload();
    }

    previousY = y;
    float accel = 30.0f * gameTime;
    float maxSpeed = 3.0f;
    float friction = 0.95f;

    Vector moveDirection;

    if (window->KeyDown('W')) { moveDirection.Add(Vector(90.0f, 1.0f)); }
    if (window->KeyDown('S')) { moveDirection.Add(Vector(270.0f, 1.0f)); }
    if (window->KeyDown('A')) { moveDirection.Add(Vector(180.0f, 1.0f)); }
    if (window->KeyDown('D')) { moveDirection.Add(Vector(0.0f, 1.0f)); }

    if (moveDirection.Magnitude() > 0.1f)
    {
        moveDirection.ScaleTo(1.0f);

        moveDirection.Scale(accel);

        speed.Add(moveDirection);
    }
    else
    {
        speed.Scale(friction);
    }

    if (speed.Magnitude() > maxSpeed)
    {
        speed.ScaleTo(maxSpeed);
    }

    float angle = speed.Angle();

    bool isMovingUp = window->KeyDown('W');
    bool isMovingDown = window->KeyDown('S');
    bool isMovingLeft = window->KeyDown('A');
    bool isMovingRight = window->KeyDown('D');
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

    /*
    // NOVO: Assumimos que o jogador est� no ar no in�cio de cada frame.
    // A fun��o OnCollision ir� corrigir isso se ele estiver em uma plataforma.

    // -----------------
    // Controle
    // -----------------

    //if (gamepadOn)
    //{
    //    // atualiza estado das teclas e eixos do controle
    //    gamepad->UpdateState();

    //    // constr�i vetor com base na posi��o do anal�gico esquerdo
    //    float ang = Line::Angle(Point(0, 0), Point(gamepad->Axis(AxisX) / 25.0f, gamepad->Axis(AxisY) / 25.0f));
    //    float mag = Point::Distance(Point(0, 0), Point(gamepad->Axis(AxisX) / 25.0f, gamepad->Axis(AxisY) / 25.0f));

    //    // nenhuma dire��o selecionada
    //    if (mag == 0)
    //    {
    //        // se a velocidade estiver muita baixa
    //        if (speed.Magnitude() < 0.1)
    //        {
    //            // pare de se movimentar imediatamente
    //            speed.ScaleTo(0.0f);
    //        }
    //        else
    //        {
    //            // some um vetor no sentido contr�rio para frear
    //            Move(Vector(speed.Angle() + 180.0f, 5.0f * gameTime));
    //        }
    //    }
    //    else
    //    {
    //        // movimente-se para a nova dire��o
    //        Move(Vector(ang, mag * gameTime));
    //    }

    //    // dispara m�ssil com o anal�gico direito
    //    if (AxisTimed(AxisRX, AxisRY, 0.150f))
    //    {
    //        float ang = Line::Angle(Point(0,0), Point(float(gamepad->Axis(AxisRX)), float(gamepad->Axis(AxisRY))));
    //        GungeonCore::audio->Play(FIRE);
    //        GungeonCore::scene->Add(new Missile(ang), STATIC);
    //    }
    //}
    */

    // -----------------
    // Teclado
    // -----------------
    //

    

    if (window->KeyPress('I')) {
        health = 10000;

        Gun* gun = new Gun("Resources/revolver_picked.png", 0.3f, 6, 1.5, MAGNUM);
        itemEquiped = 1;
        inventory[itemEquiped] = gun;

        Gun* gun2 = new Gun("Resources/GUN.png", 0.5f, 3, 1.65, GUN);
        itemEquiped = 2;
        inventory[itemEquiped] = gun2;

        itemEquiped = 1;
    }

    // Momentâneo enquanto não há armas específicas do jogo
    if (window->KeyPress('1')) {
        if (inventory[1]->type == MAGNUM) {
            Gun* gun = dynamic_cast<Gun*>(inventory[itemEquiped]);
            if (!gun->reloading)
                itemEquiped = 1;
        }
    }
    // Momentâneo enquanto não há armas específicas do jogo
    if (window->KeyPress('2')) {
        if (inventory[2]->type == GUN) {
            Gun* gun = dynamic_cast<Gun*>(inventory[itemEquiped]);
            if (!gun->reloading)
                itemEquiped = 2;
        }
    }

	// (clique esquerdo do mouse)
    if (window->KeyPress(VK_LBUTTON)) {
        
        // Momentâneo enquanto não há armas específicas do jogo
        if (inventory[itemEquiped]->type == GUN) {
            Gun* gun = dynamic_cast<Gun*>(inventory[itemEquiped]);
            gun->shoot(this,GUN,new Image("Resources/Explo.png"));
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

    Empty* empty = new Empty();

    for (int i = 0; i < 10; i++) {
        Gun* gun = dynamic_cast<Gun*>(inventory[i]);
        if (gun != nullptr) {
            gun->Reset();
        }    
        inventory[i] = empty;
    }
    
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

        GungeonCore::level->GetScene()->Delete(villain, STATIC);
    }

    if (obj->Type() == DROPPEDITEM) {
        DroppedItem* droppedItem = dynamic_cast<DroppedItem*>(obj);


        if (droppedItem->itemType == MAGNUM) {
            GungeonCore::audio->Play(GRAB_ITEM);
            Gun* gun = new Gun("Resources/revolver_picked.png", 0.3f, 6, 1.5, MAGNUM);
            itemEquiped = 1;
            inventory[itemEquiped] = gun;
        }

        if (droppedItem->itemType == GUN) {
            GungeonCore::audio->Play(GRAB_ITEM);
            Gun* gun = new Gun("Resources/GUN.png", 0.5f, 3, 1.65, GUN);
            itemEquiped = 2;
            inventory[itemEquiped] = gun;
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
