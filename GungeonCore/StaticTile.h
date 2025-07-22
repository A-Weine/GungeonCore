#ifndef _PROGJOGOS_STATICTILE_H_
#define _PROGJOGOS_STATICTILE_H_

#include "TileSet.h"
#include "Sprite.h" // Para usar SpriteData
#include "Types.h"

class StaticTile
{
private:
    SpriteData sprite;  // Guarda todos os dados do sprite pré-calculados
    TileSet* tileset;   // Ponteiro para o tileset de origem

public:
    // Construtor que recebe o tileset e o índice do tile a ser usado
    StaticTile(TileSet* ts, uint tileIndex);

    // Função Draw que recebe as coordenadas de mundo para o desenho
    void Draw(float x, float y, float z = Layer::MIDDLE, float scale = 1.0f, float rotation = 0.0f, Color color = { 1,1,1,1 });

    // Funções para obter altura e largura, se necessário
    uint Width();
    uint Height();
};

// Funções inline
inline uint StaticTile::Width() { return sprite.width; }
inline uint StaticTile::Height() { return sprite.height; }

#endif