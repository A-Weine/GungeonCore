/**********************************************************************************
// Player (Arquivo de Cabeçalho)
//
// Criação:     10 Out 2012
// Atualização: 11 Nov 2021
// Compilador:  Visual C++ 2022
//
// Descrição:   Define a classe jogador
//
**********************************************************************************/

#ifndef _GungeonCore_EXPLOSION_H_
#define _GungeonCore_EXPLOSION_H_

// ---------------------------------------------------------------------------------

#include "Object.h"                     // objetos do jogo
#include "Timer.h"                      // controle do tempo
#include "TileSet.h"
#include "Animation.h"
#include "ChaseVillain.h"

enum class Explosions
{
    EXPLOSION
};


// ---------------------------------------------------------------------------------
class Explosion : public Object
{
private:
    TileSet* sprite;                    // sprite da bala
    Animation* animation;

    Timer timer;
    float TTL;

    Explosions state = Explosions::EXPLOSION;
public:
    Explosion(float X, float Y);                  // construtor
    ~Explosion();                            // destrutor

    void Update();                      // atualização
    void Draw();                        // desenho

    void OnCollision(Object* obj);     // resolução da colisão
};

// ---------------------------------------------------------------------------------

#endif
