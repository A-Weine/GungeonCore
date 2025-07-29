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

enum class ShriekState {
    IDLE,
    WINDUP,
    PULSE_SHRIEK,
    CONTINUOUS_SHRIEK,
    COOLDOWN,
    BULLET
};

enum class ChaseVillainState
{
    SPAWNING,
    WALKING_DOWN,
    WALKING_RIGHT,
    WALKING_LEFT,
    HIT,
    QUICK_POWER,
    CONSTANT_POWER,
    EXPLODING,
    MISSILE
};

class ChaseVillain : public Object
{
private:
    TileSet* sprite;                            // sprite do objeto
    Animation* animation;
    Player* player;                            // ponteiro para jogador
    Vector speed;                               // velocidade e direção
	int health = 100;                          // vida do vilão


    Timer explosion;

    Timer stateTimer;
    int shriekPulseCount = 0;    // Para contar os 3 gritos iniciais
    bool playerIsInRange = false; // Para saber se o player está na área de alcance

    // Constantes para facilitar o ajuste
    const float WINDUP_TIME = 0.2f;       // Tempo de preparação antes do 1º grito
    const float PAUSE_BETWEEN_SHRIEKS = 0.7f; // A pausa de 1.5s

public:
    ChaseVillainState state = ChaseVillainState::WALKING_DOWN;
    ShriekState currentShriekState = ShriekState::IDLE;
    int distance;                               // mantém distância do jogador
    ChaseVillain(float pX, float pY, Player* p);      // construtor
    ~ChaseVillain();                                   // destrutor

    void OnCollision(Object* obj);
    void ResetShriekAttack();
    void Update();                              // atualização
    void Draw();                                // desenho
	void TakeDamage(int damage);      // recebe dano
};

// ---------------------------------------------------------------------------------


#endif