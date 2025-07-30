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
        sprite = new TileSet("Resources/unfinished_gun_sprite_sheet.png", 25, 18,3, 8);
        animation = new Animation(sprite, 0.125f, true);
        
        uint seqNoShoot[5] = { 0,1,2,3,4 };
        uint seqShoot[3] = { 5,6,7 };

        animation->Add(static_cast<uint>(GunState::NO_SHOOTING), seqNoShoot, 5);
        animation->Add(static_cast<uint>(GunState::SHOOTING), seqShoot, 3);

    }
    else if (gunTypes == Guntype::DEFAULTGUN) {
        reloadSound = MAGNUM_RELOAD;
        shootSound = MAGNUM_FIRE;
        sprite = new TileSet("Resources/default_gun_sprite_sheet.png", 19, 13, 3, 5);
        animation = new Animation(sprite, 0.125f, true);

        uint seqNoShoot[1] = { 0 }; 
        uint seqShoot[4] = { 1, 2, 3, 4}; 

        animation->Add(static_cast<uint>(GunState::NO_SHOOTING), seqNoShoot, 1);
        animation->Add(static_cast<uint>(GunState::SHOOTING), seqShoot, 4);
    }
    else if (gunTypes == Guntype::BOMBGUN) {
        reloadSound = MAGNUM_RELOAD;// MUDAR SOM AQUI
        shootSound = MAGNUM_FIRE;// MUDAR SOM AQUI
        sprite = new TileSet("Resources/bomb_gun_sprite_sheet.png", 29, 23, 4, 10);
        animation = new Animation(sprite, 0.125f, true);

        uint seqNoShoot[3] = { 0,1,2};
        uint seqShoot[6] = { 3, 4, 5, 6, 7, 8};

        animation->Add(static_cast<uint>(GunState::NO_SHOOTING), seqNoShoot, 3);
        animation->Add(static_cast<uint>(GunState::SHOOTING), seqShoot, 6);
    }

        currentState = GunState::NO_SHOOTING;

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

    
    if (currentState == GunState::SHOOTING) {
        if (gunTypes == Guntype::UNFINISHEDGUN && animationTime.Elapsed(0.1f)) {
           currentState = GunState::NO_SHOOTING;
           animationTime.Reset();
        }else if (gunTypes == Guntype::DEFAULTGUN && animationTime.Elapsed(0.4f)) {
            currentState = GunState::NO_SHOOTING;
            animationTime.Reset();
        }
        else if (gunTypes == Guntype::BOMBGUN && animationTime.Elapsed(0.7f)) {
            currentState = GunState::NO_SHOOTING;
            animationTime.Reset();
        }
    }
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

        currentState = GunState::SHOOTING;

        animationTime.Start();

        Scene* currentScene = GungeonCore::level->GetScene();
        if (currentScene)
        {
            GungeonCore::audio->Play(shootSound);
            currentScene->Add(new Fire(shooter, this, angleDeg, FIRE), MOVING);
        }
        timer.Reset();
    }
}

// Mouse shooting
void Gun::shoot(Object* shooter, int whoShot, Image* shotImage) {
    if ((timer.Elapsed() >= attackCooldownDuration) && !reloading) {
        
        currentState = GunState::SHOOTING;

        animationTime.Start();

        Point shooterPos(shooter->X(), shooter->Y());
        Point mouseWorldPos = window->ScreenToWorld(GungeonCore::level);

        float firingAngle = Line::Angle(shooterPos, mouseWorldPos);
        quantBullets--;

        Scene* currentScene = GungeonCore::level->GetScene();
        if (currentScene)
        {
            GungeonCore::audio->Play(shootSound);
            currentScene->Add(new Fire(shooter, this, firingAngle, FIRE), MOVING);
        }
        timer.Reset();
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
