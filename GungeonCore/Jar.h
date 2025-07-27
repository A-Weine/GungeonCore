#pragma once
#pragma once

#ifndef _GungeonCore_GUN_H_
#define _GungeonCore_GUN_H_

// ---------------------------------------------------------------------------------

#include "Object.h"                     // objetos do jogo
#include "Sprite.h"                     // desenho de sprites
#include "Particles.h"                  // sistema de partículas

// ---------------------------------------------------------------------------------

class Jar : public Object
{
private:
    Sprite* sprite;                    // sprite da arma

public:

    Jar(string filename, float x, float y); // construtor
    ~Jar();                            // destrutor

    void Update();                      // atualização
    void Draw();                        // desenho

};

// ---------------------------------------------------------------------------------

inline void Jar::Draw()
{
    sprite->Draw(x, y, Layer::MIDDLE, scale, rotation);
}
// ---------------------------------------------------------------------------------

#endif