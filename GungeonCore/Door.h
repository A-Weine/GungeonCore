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

#ifndef _GungeonCore_DOOR_H_
#define _GungeonCore_DOOR_H_

// ---------------------------------------------------------------------------------

#include "Object.h"                     // objetos do jogo
#include "Sprite.h"                     // desenho de sprites
#include "Vector.h"                     // representação de vetores
#include "Particles.h"                  // sistema de partículas
#include "Controller.h"                 // entrada pelo controle

enum { CLOSED, OPEN };


// ---------------------------------------------------------------------------------
class Door : public Object
{
private:
    Sprite* spriteOpen;
    Sprite* spriteClosed;                    // sprite da bala
public:
    uint state = CLOSED;

    Door();               // construtor
    ~Door();                 // destrutor

    void Update();                      // atualização
    void Draw();                        // desenho

};

// ---------------------------------------------------------------------------------

#endif
