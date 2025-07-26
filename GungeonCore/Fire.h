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

#ifndef _GungeonCore_FIRE_H_
#define _GungeonCore_FIRE_H_

// ---------------------------------------------------------------------------------

#include "Object.h"                     // objetos do jogo
#include "Sprite.h"                     // desenho de sprites
#include "Vector.h"                     // representação de vetores
#include "Particles.h"                  // sistema de partículas
#include "Controller.h"                 // entrada pelo controle
#include "Timer.h"                      // controle do tempo
#include "TileSet.h"
#include "Animation.h"
#include "Player.h"

// ---------------------------------------------------------------------------------
class Fire : public Object
{
private:
    Sprite * sprite;                    // sprite da bala
    Vector speed;                       // velocidade da bala  

public:
    Fire(Object * shooter,float angle, Image * img, int typeShot);                  // construtor
    ~Fire();                            // destrutor

    Vector& Speed();                    // retona vetor velocidade
    void Update();                      // atualização
    void Draw();                        // desenho

    void OnCollision(Object* obj);     // resolução da colisão
};

// ---------------------------------------------------------------------------------

inline Vector& Fire::Speed()
{
    return speed;
}

inline void Fire::Draw()
{
    sprite->Draw(x, y, Layer::UPPER, scale, rotation);
}
// ---------------------------------------------------------------------------------

#endif
