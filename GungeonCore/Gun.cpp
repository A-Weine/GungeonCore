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

// ------------------------------------------------------------------------------

// ------------------------------------------------------------------------------

Gun::Gun(string filename, float cooldownToShot, int fullOfBullets, float timeReloading, int typeGun)
{
    sprite = new Sprite(filename);
    attackCooldownDuration = cooldownToShot;

    fullBullet = fullOfBullets;
    quantBullets = fullBullet;

    timeToReload = timeReloading;

    reloading = false;

    type = GUN;

    gunTypes = typeGun;

    if (gunTypes == SHOTGUN) {
        reloadSound = SHOTGUN_RELOAD;
        shootSound = SHOTGUN_FIRE;
    }
    if (gunTypes == MAGNUM) {
        reloadSound = MAGNUM_RELOAD;
        shootSound = MAGNUM_FIRE;
    }
}

// ------------------------------------------------------------------------------

Gun::~Gun()
{
    delete sprite;
}

// -------------------------------------------------------------------------------

void Gun::Update()
{


}

void Gun::Reset(){
    reloading = false;
    quantBullets = fullBullet;
}

void Gun::shoot(Object* shooter, int whoShot, Image * shotImage) {
    if ((timer.Elapsed() >= attackCooldownDuration) && !reloading) {

        Point shooterPos(shooter->X(), shooter->Y());

        Point mouseWorldPos = window->ScreenToWorld(GungeonCore::level);

        float firingAngle = Line::Angle(shooterPos, mouseWorldPos);
        quantBullets--;

        Scene* currentScene = GungeonCore::level->GetScene();
        if (currentScene)
        {
            GungeonCore::audio->Play(shootSound);
            currentScene->Add(new Fire(shooter, firingAngle, shotImage, FIRE), MOVING);
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
