#include "StaticTile.h"
#include "Engine.h" // Para acessar o renderer
#include "sstream"

StaticTile::StaticTile(TileSet* ts, uint tileIndex)
{
    tileset = ts;

    sprite.width = tileset->TileWidth();
    sprite.height = tileset->TileHeight();
    sprite.texture = tileset->View();

    float u = (float) (tileIndex % tileset->Columns()) * tileset->TileWidth();
    float v = (float) (tileIndex / tileset->Columns()) * tileset->TileHeight();

    sprite.texCoord.x = u / tileset->Width();
    sprite.texCoord.y = v / tileset->Height();

    sprite.texSize.x = float(sprite.width) / tileset->Width();
    sprite.texSize.y = float(sprite.height) / tileset->Height();
}

void StaticTile::Draw(float x, float y, float z, float scale, float rotation, Color color)
{
    sprite.x = x;
    sprite.y = y;
    sprite.depth = z;
    sprite.scale = scale;
    sprite.rotation = rotation * XM_PI / 180.0f;
    sprite.color = color;

    Engine::renderer->Draw(sprite);
}