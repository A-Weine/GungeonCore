/**********************************************************************************
// Item (Código Fonte)
//
// Criação:     14 Mai 2012
// Atualização: 22 Set 2021
// Compilador:  Visual C++ 2019
//
// Descrição:   Define um item de menu
//
**********************************************************************************/

#include "MenuItem.h"

// ---------------------------------------------------------------------------------

MenuItem::MenuItem(float posX, float posY, uint menuId, string imgFile)
{
    // cria tileset e animação
    tileset = new TileSet(imgFile, 200, 35, 1, 3);
    animation = new Animation(tileset, 0.200f, true);

    // define sequências de animação
    uint selected[2] = { 1,2 };
    uint notselected[1] = { 0 };

    // adiciona sequências de animação
    animation->Add(SELECTED, selected, 2);
    animation->Add(NOTSELECTED, notselected, 1);

    // cria bounding box
    BBox(new Rect(-1.0f * tileset->TileWidth() / 2.0f,
        -1.0f * tileset->TileHeight() / 2.0f,
        tileset->TileWidth() / 2.0f,
        tileset->TileHeight() / 2.0f));

    // ajusta posição do item
    MoveTo(posX, posY, Layer::FRONT);

    // define o identificador do item
    type = menuId;
}

// ---------------------------------------------------------------------------------

MenuItem::~MenuItem()
{
    delete animation;
    delete tileset;
}

// -------------------------------------------------------------------------------

void MenuItem::Select()
{
    animation->Select(SELECTED);
}

// -------------------------------------------------------------------------------

void MenuItem::UnSelect()
{
    animation->Select(NOTSELECTED);
}

// -------------------------------------------------------------------------------

void MenuItem::Update()
{
    animation->NextFrame();
}

// -------------------------------------------------------------------------------

void MenuItem::Draw()
{
    animation->Draw(x, y, z);
}

// -------------------------------------------------------------------------------