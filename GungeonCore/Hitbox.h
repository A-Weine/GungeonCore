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

#ifndef _GungeonCore_HITBOX_H_
#define _GungeonCore_HITBOX_H_

// ---------------------------------------------------------------------------------

#include "Object.h"
#include "Vector.h"
#include "Timer.h"

// ---------------------------------------------------------------------------------
class Hitbox : public Object
{
private:
    Timer hitboxTimer;
    float lifeTime;
	float activationDelay;

    bool isActive, damageDealt;

public:
    Hitbox(float sizeX, float sizeY, float posX, float posY, float lifeTime, float activationDelay);                  // construtor
    ~Hitbox();                            // destrutor

    void Update();                      // atualização
    void Draw();                        // desenho
    bool IsActive() { return isActive; } // verifica se a hitbox está ativa
    bool IsDamageDealt() { return damageDealt; } // verifica se o dano foi causado

    void setDamageDealt(bool value) { damageDealt = value; } // define se o dano foi causado

    void OnCollision(Object* obj);     // resolução da colisão
};

// ---------------------------------------------------------------------------------


inline void Hitbox::Draw()
{
    
}
// ---------------------------------------------------------------------------------

#endif
