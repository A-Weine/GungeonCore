/**********************************************************************************
// WallHit (Arquivo de Cabeçalho)
//
// Criação:     03 Ago 2019
// Atualização: 05 Nov 2021
// Compilador:  Visual C++ 2022
//
// Descrição:   Efeito de explosão usando sistema de partículas
//
**********************************************************************************/

#ifndef __GungeonCore_WALLHIT_H_
#define _GungeonCore_WALLHIT_H_

// ---------------------------------------------------------------------------------

#include "Object.h"    
#include "Types.h"
#include "Particles.h"
#include "Fire.h"

// ---------------------------------------------------------------------------------

class WallHit : public Object
{
private:
    Particles* sparks;                 // sistema de partículas

public:
    WallHit(float pX, float pY, float angle);        // construtor
    ~WallHit();                         // destrutor

    void Update();                      // atualização
    void Draw();                        // desenho
};

// ---------------------------------------------------------------------------------
inline void WallHit::Draw()
{
    sparks->Draw(Layer::MIDDLE, 0.0f);
}

// ---------------------------------------------------------------------------------


#endif