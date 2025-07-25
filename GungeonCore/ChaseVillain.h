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

#ifndef _GungeonCore_CHASE_VILLAIN_H_
#define _GungeonCore_CHASE_VILLAIN_H_

// ---------------------------------------------------------------------------------

#include "Object.h"    
#include "Types.h"
#include "Sprite.h"
#include "Vector.h"
#include "Player.h"
#include "TileSet.h"
#include "Animation.h"

// ---------------------------------------------------------------------------------

enum CHASEVILLAIN {
    VILLAIN_RUNNING
};

class ChaseVillain : public Object
{
private:
    TileSet* sprite;                            // sprite do objeto
    Animation* animation;
    Player* player;                            // ponteiro para jogador
    Vector speed;                               // velocidade e direção
    int distance;                               // mantém distância do jogador

public:
    ChaseVillain(float pX, float pY, Player* p);      // construtor
    ~ChaseVillain();                                   // destrutor

    void Update();                              // atualização
    void Draw();                                // desenho
};

// ---------------------------------------------------------------------------------


#endif