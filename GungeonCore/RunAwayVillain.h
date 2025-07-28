/**********************************************************************************
// Green (Arquivo de Cabeçalho)
//
// Criação:     15 Out 2012
// Atualização: 05 Nov 2021
// Compilador:  Visual C++ 2022
//
// Descrição:   Objeto faz uma fuga suavizada
//
**********************************************************************************/

#ifndef _GungeonCore_RUN_AWAY_VILLAIN_H_
#define _GungeonCore_RUN_AWAY_VILLAIN_H_

// ---------------------------------------------------------------------------------

#include "Object.h"    
#include "Types.h"
#include "Sprite.h"
#include "Vector.h"
#include "Player.h"
#include "TileSet.h"
#include "Animation.h"

// ---------------------------------------------------------------------------------

enum RUNAWAYVILLAINMOVEMENT{
    VILLAIN_RUNNING_LEFT
};

class RunAwayVillain : public Object
{
private:
    TileSet* sprite;                            // sprite do objeto
    Animation* animation;
    Player* player;                            // ponteiro para jogador
    Vector speed;                               // velocidade e direção
    int distance;                               // mantém distância do jogador
	int health = 100;					 // vida do vilão

public:
    RunAwayVillain(float pX, float pY, Player* p);      // construtor
    ~RunAwayVillain();                                   // destrutor

    void OnCollision(Object* obj);
    void Update();                              // atualização
    void Draw();                                // desenho
    void TakeDamage(int damage);      // recebe dano
};

// ---------------------------------------------------------------------------------


#endif