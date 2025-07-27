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

#include "Jar.h"
#include "GungeonCore.h"
#include "JarShrap.h"
// ------------------------------------------------------------------------------

// ------------------------------------------------------------------------------

Jar::Jar(string filename, float x, float y)
{
    sprite = new Sprite(filename);
    
    BBox(new Rect(-(sprite->Width() / 2.0f), -(sprite->Height() / 2.0f), (sprite->Width() / 2.0f), (sprite->Height() / 2.0f)));

    type = JAR;

    MoveTo(x, y);
}

// ------------------------------------------------------------------------------

Jar::~Jar()
{
    delete sprite;
}

// -------------------------------------------------------------------------------

void Jar::Update()
{


}

void Jar::OnCollision(Object* obj) {
    
    if (obj->Type() == FIRE) {
      GungeonCore::level->GetScene()->Add(new JarShrap(this), STATIC);
      GungeonCore::level->GetScene()->Delete(this, STATIC);
    }

}

// -------------------------------------------------------------------------------
