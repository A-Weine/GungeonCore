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

#include "Fire.h"
#include "GungeonCore.h"
#include "Gun.h"
#include "Hand.h"
#include <sstream>
// ------------------------------------------------------------------------------

// ------------------------------------------------------------------------------

Gun::Gun(float cooldownToShot, int fullOfBullets, float timeReloading, Guntype typeGun, float bulletVelocity, float damage)
{
    attackCooldownDuration = cooldownToShot;

    fullBullet = fullOfBullets;
    quantBullets = fullBullet;

    timeToReload = timeReloading;

    reloading = false;

    type = GUN;

    gunTypes = typeGun;

    shootVelocity = bulletVelocity;
    shootDamage = damage;

    if (gunTypes == Guntype::UNFINISHEDGUN) {
        reloadSound = SHOTGUN_RELOAD;
        shootSound = SHOTGUN_FIRE;
        sprite = new TileSet("Resources/unfinished_gun_sprite_sheet.png", 25, 18, 3, 16);
        animation = new Animation(sprite, 0.125f, true);
        
        uint seqNoShootR[5] = { 0,1,2,3,4 };
        uint seqShootR[3] = { 5,6,7 };
        uint seqNoShootL[5] = { 8, 9, 10, 11, 12 };
        uint seqShootL[3] = { 13, 14, 15 };

        animation->Add(static_cast<uint>(GunState::NO_SHOOTING_R), seqNoShootR, 5);
        animation->Add(static_cast<uint>(GunState::NO_SHOOTING_L), seqNoShootL, 5);
        animation->Add(static_cast<uint>(GunState::SHOOTING_R), seqShootR, 3);
        animation->Add(static_cast<uint>(GunState::SHOOTING_L), seqShootL, 3);

    }
    else if (gunTypes == Guntype::DEFAULTGUN) {
        reloadSound = MAGNUM_RELOAD;
        shootSound = MAGNUM_FIRE;
        sprite = new TileSet("Resources/default_gun_sprite_sheet.png", 19, 13, 3, 10);
        animation = new Animation(sprite, 0.125f, true);

        uint seqNoShootR[1] = { 0 }; 
        uint seqShootR[4] = { 1, 2, 3, 4}; 
        uint seqNoShootL[1]{ 5 };
        uint seqShootL[4] = { 6, 7, 8, 9 };

        animation->Add(static_cast<uint>(GunState::NO_SHOOTING_R), seqNoShootR, 1);
        animation->Add(static_cast<uint>(GunState::NO_SHOOTING_L), seqNoShootL, 1);
        animation->Add(static_cast<uint>(GunState::SHOOTING_R), seqShootR, 4);
        animation->Add(static_cast<uint>(GunState::SHOOTING_L), seqShootL, 4);
    }
    else if (gunTypes == Guntype::BOMBGUN) {
        reloadSound = MAGNUM_RELOAD;// MUDAR SOM AQUI
        shootSound = MAGNUM_FIRE;// MUDAR SOM AQUI
        sprite = new TileSet("Resources/bomb_gun_sprite_sheet.png", 29, 23, 4, 20);
        animation = new Animation(sprite, 0.125f, true);

        uint seqNoShootR[3] = { 0,1,2};
        uint seqShootR[6] = { 3, 4, 5, 6, 7, 8};
        uint seqNoShootL[3] = { 10, 11, 12 };
        uint seqShootL[6] = { 13, 14, 15, 16, 17, 18 };

        animation->Add(static_cast<uint>(GunState::NO_SHOOTING_R), seqNoShootR, 3);
        animation->Add(static_cast<uint>(GunState::NO_SHOOTING_L), seqNoShootL, 3);
        animation->Add(static_cast<uint>(GunState::SHOOTING_R), seqShootR, 6);
        animation->Add(static_cast<uint>(GunState::SHOOTING_L), seqShootL, 6);
    }

        currentState = GunState::NO_SHOOTING_R;

        animation->Select(static_cast<uint>(currentState));
}

// ------------------------------------------------------------------------------

Gun::~Gun()
{
    delete sprite;
}

// -------------------------------------------------------------------------------

void Gun::Update()
{
    animation->NextFrame();

    // 1. Converte a rotação para um ângulo lógico (0-360) se ela for negativa.
    float logicAngle = this->rotation;
    if (logicAngle < 0.0f) {
        logicAngle += 360.0f;
    }

    // 2. Decide a direção (esquerda ou direita)
    bool isFacingLeft = (logicAngle > 90.0f && logicAngle < 270.0f);

    // 3. Gerencia os estados da animação
    if (currentState == GunState::SHOOTING_R || currentState == GunState::SHOOTING_L)
    {
        // Se está no meio de um tiro, espera o timer acabar
        if (animationTime.Elapsed(0.3f))
        {
            // E então volta para o estado parado CORRETO
            currentState = isFacingLeft ? GunState::NO_SHOOTING_L : GunState::NO_SHOOTING_R;
        }
    }
    else // Se NÃO está atirando (está em NO_SHOOTING_R ou NO_SHOOTING_L)
    {
        // Atualiza constantemente a direção da animação parada
        currentState = isFacingLeft ? GunState::NO_SHOOTING_L : GunState::NO_SHOOTING_R;
    }

    // 4. Seleciona a animação final
    animation->Select(static_cast<uint>(currentState));
}

void Gun::Reset() {
    reloading = false;
    quantBullets = fullBullet;
}

// Right stick shooting
void Gun::shoot(Object* shooter, int whoShot, Image* shotImage, float angleDeg) {
    if ((timer.Elapsed() >= attackCooldownDuration) && !reloading) {
        quantBullets--;

        // Pega a rotação atual para decidir a direção do tiro
        float logicAngle = this->rotation;
        if (logicAngle < 0.0f) {
            logicAngle += 360.0f;
        }

        // Inicia o estado de tiro na direção correta
        currentState = (logicAngle > 90.0f && logicAngle < 270.0f) ? GunState::SHOOTING_L : GunState::SHOOTING_R;
        animationTime.Start();

        // O ângulo do projétil é a rotação atual da arma!
        float firingAngle = this->rotation;

        Scene* currentScene = GungeonCore::level->GetScene();
        if (currentScene)
        {
            GungeonCore::audio->Play(shootSound);
            currentScene->Add(new Fire(shooter, this, angleDeg, FIRE), MOVING);
        }
        timer.Reset();
    }

    if (reloading) {
        GungeonCore::audio->Play(EMPTY_GUN);
    }
}

// Mouse shooting
void Gun::shoot(Object* shooter, int whoShot, Image* shotImage) {
    if ((timer.Elapsed() >= attackCooldownDuration) && !reloading) {
        
        float logicAngle = this->rotation;
        if (logicAngle < 0.0f) {
            logicAngle += 360.0f;
        }

        // Inicia o estado de tiro na direção correta
        currentState = (logicAngle > 90.0f && logicAngle < 270.0f) ? GunState::SHOOTING_L : GunState::SHOOTING_R;
        animationTime.Start();

        // O ângulo do projétil é a rotação atual da arma!
        float firingAngle = this->rotation;

        Point shooterPos(shooter->X(), shooter->Y());
        Point mouseWorldPos = window->ScreenToWorld(GungeonCore::level);

        quantBullets--;

        Scene* currentScene = GungeonCore::level->GetScene();
        if (currentScene)
        {
            GungeonCore::audio->Play(shootSound);
            currentScene->Add(new Fire(shooter, this, firingAngle, FIRE), MOVING);
        }
        timer.Reset();
    }

    if (reloading) {
        GungeonCore::audio->Play(EMPTY_GUN);
    }
}

void Gun::reload() {
    if ((quantBullets == 0 && !reloading) || (window->KeyPress('R') && quantBullets != fullBullet)) {
        GungeonCore::audio->Play(reloadSound);
        reloading = true;
        timerToReload.Start();
    }
    if (timerToReload.Elapsed() >= timeToReload && reloading) {
        reloading = false;
        quantBullets = fullBullet;
        timerToReload.Reset();
        timerToReload.Stop();
    }
}

// -------------------------------------------------------------------------------
