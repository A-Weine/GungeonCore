/**********************************************************************************
// Tilemap (Arquivo de Cabeçalho)
//
// Criação:     22 Jul 2025
// Atualização: 22 Jul 2025
// Compilador:  Visual C++ 2022
//
// Descrição:   Representa um mapa em tiles, lido de um arquivo do Tiled,
//              desenhado de forma otimizada com culling.
//
**********************************************************************************/

#ifndef _PROGJOGOS_TILEMAP_H_
#define _PROGJOGOS_TILEMAP_H_

#include "Object.h"
#include "TileSet.h"
#include "StaticTile.h"
#include <string>
#include <vector>

enum class TileCollisionType { NONE, SOLID };

class Tilemap : public Object
{
private:
    int** mapData;
    int mapWidth;
    int mapHeight;

    
    std::vector<StaticTile*> tileCache;

    std::vector<TileSet*> loadedTilesets;
    std::vector<int> firstGids;

public:
    Tilemap();
    ~Tilemap();

    bool Load(const std::string& filename);

    void Update() override;
    void Draw() override;

    TileCollisionType GetTileCollisionType(int col, int row);
};

#endif