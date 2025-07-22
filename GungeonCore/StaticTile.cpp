#include "StaticTile.h"
#include "Engine.h" // Para acessar o renderer

StaticTile::StaticTile(TileSet* ts, uint tileIndex)
{
    tileset = ts;

    // --- LÓGICA DE CÁLCULO FEITA UMA ÚNICA VEZ ---

    // Define o tamanho do sprite com base no tamanho do tile
    sprite.width = tileset->TileWidth();
    sprite.height = tileset->TileHeight();
    sprite.texture = tileset->View();

    // Calcula a posição (u,v) do tile dentro da imagem do tileset em pixels
    float u = (float) (tileIndex % tileset->Columns()) * tileset->TileWidth();
    float v = (float) (tileIndex / tileset->Columns()) * tileset->TileHeight();

    // Converte as coordenadas em pixels para o formato normalizado (0.0 a 1.0) que o renderer espera
    sprite.texCoord.x = u / tileset->Width();
    sprite.texCoord.y = v / tileset->Height();

    // Calcula o tamanho do recorte na textura, também normalizado
    sprite.texSize.x = float(sprite.width) / tileset->Width();
    sprite.texSize.y = float(sprite.height) / tileset->Height();
}

void StaticTile::Draw(float x, float y, float z, float scale, float rotation, Color color)
{
    // A função Draw agora é muito simples. Ela só precisa atualizar
    // as propriedades que mudam a cada quadro (posição, rotação, etc.)

    sprite.x = x;
    sprite.y = y;
    sprite.depth = z;
    sprite.scale = scale;
    sprite.rotation = rotation * XM_PI / 180.0f;
    sprite.color = color;

    // Envia o SpriteData já preparado para o renderer
    Engine::renderer->Draw(sprite);
}