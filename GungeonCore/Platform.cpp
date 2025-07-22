#include "GungeonCore.h"
#include "Platform.h"
#include "Engine.h"

// ---------------------------------------------------------------------------------

Platform::Platform(TileSet* ts, uint tile, float x, float y, int platformType)
{
    type = platformType;

    // Cria o nosso novo objeto StaticTile com o tileset e o tile corretos
    tileSprite = new StaticTile(ts, tile);

    if (platformType == SOLIDPLATFORM) {

        layer = CollisionLayer::SOLIDPLATFORM;
        BBox(new Rect(
            -(tileSprite->Width() / 2.0f),
            -(tileSprite->Height() / 2.0f),
            (tileSprite->Width() / 2.0f),
            (tileSprite->Height() / 2.0f)
        ));
        MoveTo(x, y);
    }
    else if(platformType == SOFTPLATFORM) {
        layer = CollisionLayer::SOFTPLATFORM;
        BBox(new Rect(
            -(tileSprite->Width()/2.0f),
            -((tileSprite->Height() / 4.0f)-1),
            (tileSprite->Width() / 2.0f),
            (tileSprite->Height() / 2.0f)
        ));
        MoveTo(x, y);
    }
    else if (platformType == NOBBOXPLATFORM) {
        layer = CollisionLayer::NONE;
        MoveTo(x, y);
    }
    // A BBox usa o tamanho do tile que o nosso novo objeto conhece
}

// ---------------------------------------------------------------------------------

Platform::~Platform()
{
}

// ---------------------------------------------------------------------------------

void Platform::Update()
{
}

// ---------------------------------------------------------------------------------

void Platform::OnCollision(Object* obj)
{
}

// ---------------------------------------------------------------------------------

void Platform::Draw()
{
    // A lógica de desenho é uma única linha: delegue para o objeto tileSprite!
    tileSprite->Draw(x, y, Layer::LOWER);
}

// ---------------------------------------------------------------------------------
