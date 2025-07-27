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

#ifndef _GungeonCore_RANDOM_MOVEMENT_VILLAIN_H_
#define _GungeonCore_RANDOM_MOVEMENT_VILLAIN_H_

// ---------------------------------------------------------------------------------

#include "Object.h"    
#include "Types.h"
#include "Sprite.h"
#include "Vector.h"
#include "Timer.h"
#include "Random.h"
#include "TileSet.h"
#include "Animation.h"
#include "Player.h"
// ---------------------------------------------------------------------------------

enum RANDOMMOVEMENTVILLAINSTATE
{
	FLYING, CHARGING, ATTACKING
};


class RandomMovementVillain : public Object
{
private:
	TileSet * sprite;
	Animation * animation;
	Vector speed;
	Timer timer;    
	Random<float> magnitude;       
	Random<float> angle;           
	Random<float> secs;            
	float delay;
	Player* player;                            // ponteiro para jogador

	int distance;
	int const distanceToSee = 100;
	int const visionMultiplierWhenAttacking = 2;

	Timer timerToAttack;
	float preparingToAttack;

	int health = 100;

public:
	boolean isAttacking;
	RandomMovementVillain(float x, float y, Player * p);
	~RandomMovementVillain();

	void OnCollision(Object* obj);     // resolu��o da colis�o
	void NewDirection();
	void Update();
	void Draw();

	void TakeDamage(int damage);      // recebe dano
};
// ---------------------------------------------------------------------------------


#endif
