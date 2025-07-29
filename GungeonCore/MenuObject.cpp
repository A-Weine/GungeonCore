#include "GungeonCore.h"
#include "MenuObject.h"


// ---------------------------------------------------------------------------------

MenuObject::MenuObject(string fileName, float px, float py, float size)
{
    
    sprite = new Sprite(fileName);
    MoveTo(px, py);
    sizer = size;
}

// ---------------------------------------------------------------------------------

MenuObject::~MenuObject()
{
    delete sprite;
}

// -------------------------------------------------------------------------------

void MenuObject::Update()
{

}

// -------------------------------------------------------------------------------

void MenuObject::Draw()
{
    sprite->Draw(x, y, Layer::FRONT, sizer, rotation);
}