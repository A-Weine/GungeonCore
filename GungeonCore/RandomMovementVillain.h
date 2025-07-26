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
	FLYING
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

public:
	boolean isAttacking;
	RandomMovementVillain(Player * p);
	~RandomMovementVillain();

	void NewDirection();
	void Update();
	void Draw();

};
// ---------------------------------------------------------------------------------


#endif
