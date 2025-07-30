#pragma once

#ifndef _GungeonCore_GUN_H_
#define _GungeonCore_GUN_H_

// ---------------------------------------------------------------------------------

#include "Object.h"                     // objetos do jogo
#include "Sprite.h"                     // desenho de sprites
#include "Vector.h"                     // representação de vetores
#include "Particles.h"                  // sistema de partículas
#include "Controller.h"                 // entrada pelo controle
#include "Timer.h"                      // controle do tempo
#include "GungeonCore.h"
#include "Animation.h"

// ---------------------------------------------------------------------------------

enum class Guntype {
    BOMBGUN, DEFAULTGUN, UNFINISHEDGUN
};

enum class GunState
{
    NO_SHOOTING,
    SHOOTING
};


class Gun : public Object
{
private:
    
    
    // SOUNDS
    int reloadSound;
    int shootSound;



public:
    TileSet* sprite;                    // sprite da arma
    Animation* animation;
    
    Timer animationTime;

    GunState currentState;
    Guntype gunTypes;

    float attackCooldownDuration;   
    Timer timer;                          
    
    int fullBullet;
    int quantBullets;

    float  timeToReload;
    Timer timerToReload;

    float firingAngle;                    // direção dos disparos
    boolean reloading = false;

    float shootVelocity;
    float shootDamage;

    Gun(float cooldownToShot, int fullOfBullets, float timeReloading, 
        Guntype type, float bulletVelocity, float damage); // construtor
    ~Gun();                            // destrutor

    void shoot(Object* shooter, int whoShot, Image* shotImage, float angleDeg);
    void shoot(Object* shooter, int whoShot, Image* shotImage);

    void reload();
    void Reset();
    void Update();                      // atualização
    void Draw();                        // desenho

};

// ---------------------------------------------------------------------------------

inline void Gun::Draw()
{
    animation->Draw(x, y, Layer::UPPER, scale, rotation);
}
// ---------------------------------------------------------------------------------

#endif