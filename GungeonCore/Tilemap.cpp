/**********************************************************************************
// Tilemap (Código Fonte)
//
// Criação:     22 Jul 2025
// Atualização: 22 Jul 2025
// Compilador:  Visual C++ 2022
//
// Descrição:   Representa um mapa em tiles, lido de um arquivo do Tiled,
//              desenhado de forma otimizada com culling.
//
**********************************************************************************/

#include "Tilemap.h"
#include "Engine.h"
#include "GungeonCore.h"
#include "tinyxml2.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace tinyxml2;

// -------------------------------------------------------------------------------

Tilemap::Tilemap()
{
    mapData = nullptr;
    mapWidth = 0;
    mapHeight = 0;
}

// -------------------------------------------------------------------------------

Tilemap::~Tilemap()
{
    if (mapData)
    {
        for (int i = 0; i < mapHeight; ++i)
        {
            delete[] mapData[i];
        }
        delete[] mapData;
    }

    for (auto tile : tileCache)
    {
        delete tile;
    }
    tileCache.clear();

    for (auto tileset : loadedTilesets)
        delete tileset;
    loadedTilesets.clear();
}

// -------------------------------------------------------------------------------

bool Tilemap::Load(const std::string& filename)
{
    for (auto tile : tileCache) delete tile;
    tileCache.clear();
    loadedTilesets.clear();
    firstGids.clear();

    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filename.c_str()) != XML_SUCCESS)
    {
        OutputDebugStringA(("ERRO: Falha ao carregar o arquivo do mapa: " + filename).c_str());
        return false;
    }

    XMLElement* pMap = doc.FirstChildElement("map");
    if (!pMap) return false;

    mapWidth = pMap->IntAttribute("width");
    mapHeight = pMap->IntAttribute("height");

    std::vector<TileSet*> tempTilesets;
    uint maxTileId = 0;

    for (XMLElement* pTilesetEl = pMap->FirstChildElement("tileset"); pTilesetEl != nullptr; pTilesetEl = pTilesetEl->NextSiblingElement("tileset"))
    {
        int gid = pTilesetEl->IntAttribute("firstgid");

        firstGids.push_back(gid);

        std::string tsxPath = std::string(pTilesetEl->Attribute("source"));

        tinyxml2::XMLDocument tsxDoc;
        if (tsxDoc.LoadFile(tsxPath.c_str()) == XML_SUCCESS)
        {
            XMLElement* ts = tsxDoc.FirstChildElement("tileset");
            uint tileCount = ts->UnsignedAttribute("tilecount");
            uint columns = ts->UnsignedAttribute("columns");

            std::string imgPath = "Resources/" + std::string(ts->FirstChildElement("image")->Attribute("source"));

            uint rows = (uint)ceil(float(tileCount) / float(columns));

            tempTilesets.push_back(new TileSet(imgPath, rows, columns));

            maxTileId = max(maxTileId, gid + tileCount - 1);
        
            loadedTilesets.push_back(new TileSet(imgPath, rows, columns));
        }
    }

    tileCache.resize(maxTileId, nullptr);

    for (size_t i = 0; i < loadedTilesets.size(); ++i)
    {
        int gid_start = firstGids[i];
        for (uint tile_idx = 0; tile_idx < loadedTilesets[i]->Size(); ++tile_idx)
        {
            int globalTileId = gid_start + tile_idx;

            if (globalTileId <= tileCache.size()) {
                tileCache[globalTileId-1] = new StaticTile(loadedTilesets[i], tile_idx);
            }
        }
    }

    mapData = new int* [mapHeight];
    for (int i = 0; i < mapHeight; ++i) mapData[i] = new int[mapWidth];

    XMLElement* pData = pMap->FirstChildElement("layer")->FirstChildElement("data");
    std::stringstream ss(pData->GetText());

    std::string valueStr;
    int r = 0, c = 0;

    while (std::getline(ss, valueStr, ','))
    {
        int tileID = 0;
        try {
            valueStr.erase(std::remove_if(valueStr.begin(), valueStr.end(), isspace), valueStr.end());
            if (!valueStr.empty()) tileID = std::stoi(valueStr);
        }
        catch (...) { continue; }

        if (c < mapWidth && r < mapHeight) mapData[r][c] = tileID;

        c++;
        if (c >= mapWidth) { c = 0; r++; }
    }

    return true;
}

// -------------------------------------------------------------------------------

void Tilemap::Update()
{

}

// -------------------------------------------------------------------------------

void Tilemap::Draw()
{
    if (!mapData || tileCache.empty()) return;

    const int TILE_SIZE = 32;
    Game* level = GungeonCore::level;

    float viewLeft = level->GetViewLeft();
    float viewTop = level->GetViewTop();
    float viewWidth = window->Width() / level->GetZoom();
    float viewHeight = window->Height() / level->GetZoom();
    float viewRight = viewLeft + viewWidth;
    float viewBottom = viewTop + viewHeight;

    int startCol = static_cast<int>(floor(viewLeft / TILE_SIZE));
    int endCol = static_cast<int>(ceil(viewRight / TILE_SIZE));
    int startRow = static_cast<int>(floor(viewTop / TILE_SIZE));
    int endRow = static_cast<int>(ceil(viewBottom / TILE_SIZE));

    startCol = max(0, startCol);
    endCol = min(mapWidth, endCol);
    startRow = max(0, startRow);
    endRow = min(mapHeight, endRow);

    for (int r = startRow; r < endRow; ++r)
    {
        for (int c = startCol; c < endCol; ++c)
        {
            int tileID = mapData[r][c];
            if (tileID == 0) continue;

            int cacheIndex = tileID - 1;

            if (cacheIndex >= 0 && cacheIndex < tileCache.size() && tileCache[cacheIndex] != nullptr)
            {
                StaticTile* tileToDraw = tileCache[cacheIndex];

                float worldX = (c * TILE_SIZE) + (TILE_SIZE / 2.0f);
                float worldY = (r * TILE_SIZE) + (TILE_SIZE / 2.0f);

                tileToDraw->Draw(worldX, worldY, Layer::LOWER);
            }
        }
    }
}