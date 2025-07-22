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

#ifndef _GungeonCore_BONE_PROJECTILE_H_
#define _GungeonCore_BONE_PROJECTILE_H_

// ---------------------------------------------------------------------------------

#include "Object.h"                     // objetos do jogo
#include "Vector.h"                     // representação de vetores
#include "Particles.h"                  // sistema de partículas
#include "Controller.h"                 // entrada pelo controle
#include "Timer.h"                      // controle do tempo
#include "TileSet.h"
#include "Animation.h"
#include "Player.h"

// ---------------------------------------------------------------------------------
class BoneProjectile : public Object
{
private:
	Animation* animation;            // animação da bala
    TileSet* sprite;                    // sprite da bala
    Vector speed;                       // velocidade da bala  

public:
    BoneProjectile(Object* shooter, float angle);                  // construtor
    ~BoneProjectile();                            // destrutor

    Vector& Speed();                    // retona vetor velocidade
    void Update();                      // atualização
    void Draw();                        // desenho

    void OnCollision(Object* obj);     // resolução da colisão
};

// ---------------------------------------------------------------------------------

inline Vector& BoneProjectile::Speed()
{
    return speed;
}

inline void BoneProjectile::Draw()
{
    animation->Draw(x, y, Layer::UPPER, scale, rotation);
}
// ---------------------------------------------------------------------------------

#endif
