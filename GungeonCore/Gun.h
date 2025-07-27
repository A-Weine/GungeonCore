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
#include "Fire.h"
#include "GungeonCore.h"

// ---------------------------------------------------------------------------------

enum Guntype {
    SHOTGUN, MAGNUM
};

class Gun : public Object
{
private:
    Sprite* sprite;                    // sprite da arma
    
    
    // SOUNDS
    int reloadSound;
    int shootSound;



public:


    int gunTypes;

    float attackCooldownDuration;   
    Timer timer;                          
    
    int fullBullet;
    int quantBullets;

    float  timeToReload;
    Timer timerToReload;

    float firingAngle;                    // direção dos disparos
    boolean reloading = false;

    Gun(string filename, float cooldownToShot, int fullOfBullets, float timeReloading, int type); // construtor
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
    sprite->Draw(x, y, Layer::UPPER, scale, rotation);
}
// ---------------------------------------------------------------------------------

#endif