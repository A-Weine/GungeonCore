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

#include "DroppedItem.h"
#include "GungeonCore.h"
#include "Hud.h"
#include "Platform.h"
#include "Shadow.h"

// ------------------------------------------------------------------------------


// ------------------------------------------------------------------------------

DroppedItem::DroppedItem(string filename, int X, int Y, Scene * scene, int itemTypes, Object * objeto)
{
    // inicializa sprite
    sprite = new Sprite(filename);

    obj = objeto;

    BBox(new Circle(7));

    initialX = X;
    initialY = Y;
    MoveTo((float) initialX, (float) initialY);

    type = DROPPEDITEM;
    itemType = itemTypes;
    topBoundary = y - 10;
    bottomBoundary = y;

    itemShadow = new Shadow(this, scene, 5);
    scene->Add(itemShadow, STATIC);
    // incrementa contagem
    //++Hud::missiles;
}

// ------------------------------------------------------------------------------

void DroppedItem::Reset()
{
    // volta ao estado inicial
    MoveTo((float) initialX, (float) initialY);
}

DroppedItem::~DroppedItem()
{
    delete sprite;
    // decrementa contagem
    //--Hud::missiles;
}

// -------------------------------------------------------------------------------

void DroppedItem::Update()
{
    // Put this in your object's Update function
    // 1. Check if the object has reached a boundary and needs to turn around
    if (Y() >= bottomBoundary) {
        movingUp = true; // Start moving up
    }
    else if (Y() <= topBoundary) {
        movingUp = false; // Start moving down
    }

    // 2. Move the object in the current direction
    if (movingUp) {
        // Move up (negative Y direction in many 2D engines)
        Translate(0.0f, -verticalSpeed * gameTime);
    }
    else {
        // Move down (positive Y direction)
        Translate(0.0f, verticalSpeed * gameTime);
    }
}


// -------------------------------------------------------------------------------
