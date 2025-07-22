/**********************************************************************************
// Missile (Código Fonte)
//
// Criação:     23 Nov 2011
// Atualização: 11 Nov 2021
// Compilador:  Visual C++ 2019
//
// Descrição:   Define uma classe para um míssil
//
**********************************************************************************/

#include "Door.h"
#include "GungeonCore.h"
#include "Hud.h"
#include "Platform.h"
#include "Shadow.h"

// ------------------------------------------------------------------------------


// ------------------------------------------------------------------------------

Door::Door()
{

    // inicializa sprite
    spriteOpen = new Sprite("Resources/Door.png");
    spriteClosed = new Sprite("Resources/closed_door.png");

    BBox(new Rect(
        -(spriteClosed->Width() / 3.0f) + 3,
        -(spriteClosed->Height() / 4.0f),
        (spriteClosed->Width() / 3.0f) - 5,
        (spriteClosed->Height() / 2.0f)
    ));

    
    MoveTo(2464, 400);

    type = DOOR;

    // incrementa contagem
    //++Hud::missiles;
}

// ------------------------------------------------------------------------------

Door::~Door()
{
    delete spriteClosed;
    delete spriteOpen;
    // decrementa contagem
    //--Hud::missiles;
}

// -------------------------------------------------------------------------------

void Door::Update()
{

}

void Door::Draw()
{
    if (state == CLOSED)
        spriteClosed->Draw(x, y, Layer::MIDDLE, scale, rotation);
    else
        spriteOpen->Draw(x, y, Layer::MIDDLE, scale, rotation);
}
