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

#ifndef _GungeonCore_DROPPEDITEM_H_
#define _GungeonCore_DROPPEDITEM_H_

// ---------------------------------------------------------------------------------

#include "Object.h"                     // objetos do jogo
#include "Sprite.h"                     // desenho de sprites
#include "Vector.h"                     // representação de vetores
#include "Particles.h"                  // sistema de partículas
#include "Controller.h"                 // entrada pelo controle
#include "TileSet.h"
#include "Animation.h"
#include "Shadow.h"


// ---------------------------------------------------------------------------------
class DroppedItem : public Object
{
private:
    Sprite* sprite;                    // sprite da bala
public:
    Shadow* itemShadow;
    bool movingUp = false;
    float topBoundary;
    float bottomBoundary;
    float sinkAmount = 6.0f;
    float const verticalSpeed = 5.0f;
    int itemType;

    Object* obj;

    int initialX;
    int initialY;

    DroppedItem(string filename, int x, int y, Scene * scene, int itemTypes, Object * objeto);               // construtor
    ~DroppedItem();                 // destrutor

    void Update();                      // atualização
    void Draw();                        // desenho
    void Reset();

    void MoveY(Vector&& v);
};

// ---------------------------------------------------------------------------------

inline void DroppedItem::Draw()
{
    sprite->Draw(x, y, Layer::UPPER, scale, rotation);
}
// ---------------------------------------------------------------------------------

#endif
