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

#ifndef _GungeonCore_SHRIEK_ATTACK_H_
#define _GungeonCore_SHRIEK_ATTACK_H_

// ---------------------------------------------------------------------------------

#include "Object.h"                     // objetos do jogo
#include "Timer.h"                      // controle do tempo
#include "TileSet.h"
#include "Animation.h"
#include "ChaseVillain.h"

enum SHRIEK
{
    FASTATTACK, CONSTANTATTACK
};

enum class EstadoOnda
{
    Crescendo,
    PausadoNoMaximo,
    Diminuindo,
    PausadoNoMinimo
};

// ---------------------------------------------------------------------------------
class ShriekAttack : public Object
{
private:
    TileSet* sprite;                    // sprite da bala
    Animation* animation;
    Animation* animationConstant;
    ChaseVillain* zombie;

    Timer diminuindoTTL;
    Timer attackTTL;
    float TTL;

    float attackSize = 10;
    boolean isConstant;


    EstadoOnda estadoAtualDaOnda = EstadoOnda::Crescendo;

    Timer timerPausa;



public:
    ShriekAttack(ChaseVillain * zombier, boolean isConstants);                  // construtor
    ~ShriekAttack();                            // destrutor

    void Update();                      // atualização
    void Draw();                        // desenho

    void OnCollision(Object* obj);     // resolução da colisão
};

// ---------------------------------------------------------------------------------

#endif
