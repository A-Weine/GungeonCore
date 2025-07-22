#pragma once
#ifndef _GungeonCore_PLATFORM_H_
#define _GungeonCore_PLATFORM_H_

// ---------------------------------------------------------------------------------
// Inclusões

#include "Types.h"                      // tipos específicos da engine
#include "Object.h"                     // interface de Object
#include "Sprite.h"                     // interface de Sprites
#include "TileSet.h"                    // interface de TileSet
#include "StaticTile.h"                // interface de StaticTile

// ---------------------------------------------------------------------------------
// Constantes Globais
// ---------------------------------------------------------------------------------

enum Platforms
{
    SOLIDPLATFORM, SOFTPLATFORM, NOBBOXPLATFORM
};

class Platform : public Object
{
private:
    //Sprite* platform = nullptr;
    StaticTile* tileSprite;

    const float maxLife = 50.0f;
    float life = maxLife;

public:
    uint state;

    Platform(TileSet* ts, uint tile, float x, float y, int platformType);
    ~Platform();                          // destrutor

    void OnCollision(Object* obj);     // resolução da colisão
    //void PivotCollision(Object * obj);  // revolve colisão com pivô

    void Update();                      // atualização do objeto
    void Draw();                        // desenho do objeto
};

// ---------------------------------------------------------------------------------

#endif
