/**********************************************************************************
// Level1 (C�digo Fonte)
//
// Cria��o:     14 Fev 2013
// Atualiza��o: 04 Set 2023
// Compilador:  Visual C++ 2022
//
// Descri��o:   N�vel 1 do jogo
//
**********************************************************************************/

#include "GungeonCore.h"
#include "Level1.h"
#include "Player.h"
#include "Platform.h"
#include "Background.h"
#include "Shadow.h"
#include "ScoreScreen.h"
#include "DroppedItem.h"
#include "Door.h"
#include "RandomMovementVillain.h"
#include "RunAwayVillain.h"
#include "ChaseVillain.h"

#include <string>
#include <fstream>
#include "Engine.h"

using std::ifstream;
using std::string;

#include <sstream>

Scene* Level1::scene = nullptr;
bool Level1::zoomingIn = false;
float Level1::zoom = 2.0f;

// ------------------------------------------------------------------------------

void Level1::Init()
{
    GungeonCore::completionTime->Start();

    GungeonCore::viewHUD = true;

    Size(2560, 864);

    zoom = 2.0f;

    scene = new Scene();

    cameraCurrentX = GungeonCore::player->X();
    cameraCurrentY = GungeonCore::player->Y();

    backg = new Background("Resources/background_t.png");

    Shadow* playerShadow = new Shadow(GungeonCore::player, scene);

	DroppedItem* revolver = new DroppedItem("Resources/magnum_dropped.png",200, 820, scene, MAGNUM);
    scene->Add(revolver,STATIC);

    Image* img = new Image("Resources/bat_sprite_sheet.png");

    RandomMovementVillain* randomMovementVillain = new RandomMovementVillain(img);
    scene->Add(randomMovementVillain, STATIC);

    RunAwayVillain* runAwayMovementVillain = new RunAwayVillain(200, 1500, GungeonCore::player);
    scene->Add(runAwayMovementVillain, STATIC);

    ChaseVillain* chaseMovementVillain = new ChaseVillain(500, 1500, GungeonCore::player);
    scene->Add(chaseMovementVillain, STATIC);
    

    scene->Add(GungeonCore::player, MOVING);
    scene->Add(playerShadow, STATIC);

    // MAP LOGIC

    /*Color white{ 1,1,1,1 };

    TileSet* tileset1 = new TileSet("Resources/tileset.png", 3, 3);
    TileSet* tileset3 = new TileSet("Resources/tileset3.png", 3, 3);
    TileSet* tilesetBorders = new TileSet("Resources/tilesetBorders.png", 2, 2);

    const int mapWidth = 80;
    const int TILE_SIZE = 32;

    std::ifstream file("Level1.tmx"); 
    if (!file.is_open()) {
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string fileContent = buffer.str();
    file.close();

    size_t startPos = fileContent.find("<data encoding=\"csv\">");
    if (startPos == std::string::npos) {
        return;
    }
    startPos = fileContent.find('>', startPos) + 1; 

    size_t endPos = fileContent.find("</data>", startPos);
    if (endPos == std::string::npos) {
        return;
    }

    std::string csvData = fileContent.substr(startPos, endPos - startPos);
    
    std::stringstream ss(csvData);
    std::string valueStr;
    int col = 0;
    int row = 0;

    while (std::getline(ss, valueStr, ','))
    {
        int tileID = 0;
        try {
            valueStr.erase(std::remove(valueStr.begin(), valueStr.end(), '\n'), valueStr.end());
            valueStr.erase(std::remove(valueStr.begin(), valueStr.end(), '\r'), valueStr.end());
            if (!valueStr.empty())
                tileID = std::stoi(valueStr);
        }
        catch (const std::invalid_argument&) {
            continue;
        }

        if (tileID != 0) {

            float positionX = (col * TILE_SIZE) + (TILE_SIZE / 2.0f);
            float positionY = (row * TILE_SIZE) + (TILE_SIZE / 2.0f);

            TileSet* selectedTileset = nullptr;
            int localTileIndex = 0;

            if (tileID >= 19)
            {
                selectedTileset = tilesetBorders;
                localTileIndex = tileID - 19;
            }
            else if (tileID >= 10)
            {
                selectedTileset = tileset3;
                localTileIndex = tileID - 10;
            }
            else if (tileID >= 1)
            {
                selectedTileset = tileset1;
                localTileIndex = tileID - 1;
            }

            if (selectedTileset)
            {
                if (tileID >= 10 && tileID <= 18) {
                scene->Add(new Platform(selectedTileset, localTileIndex, positionX, positionY, SOFTPLATFORM), STATIC);
                }
                else {
                    scene->Add(new Platform(selectedTileset, localTileIndex, positionX, positionY, SOLIDPLATFORM), STATIC);
                }
            }
        }

        col++;
        if (col >= mapWidth) {
            col = 0;
            row++;
        }
    }
    */

    float difx = (game->Width() - window->Width()) / 2.0f;
    float dify = (game->Height() - window->Height()) / 2.0f;

    GungeonCore::player->TakeDamage(true);
    //GungeonCore::audio->Play(SONG_LEVEL1, true);
}

// ------------------------------------------------------------------------------

void Level1::Update()
{
    if (window->KeyPress(VK_ESCAPE))
    {
        GungeonCore::player->Reset();
        GungeonCore::NextLevel<Level1>();
    }
    else if (window->KeyPress('N') || GungeonCore::levelNumber) {
        GungeonCore::completionTime->Stop();
        GungeonCore::NextLevel<ScoreScreen>();
    }
    else
    {
        scene->Update();
        scene->CollisionDetection();
    }

    if (window->KeyPress('L'))
    {
		zoomingIn = !zoomingIn;
    }

    if (zoomingIn)
    {
        zoom += 1.5f * gameTime;
        if (zoom > 4.0f)
            zoom = 4.0f;
    }
    else
    {
        zoom -= 1.5f * gameTime;
        if (zoom < 2.0f)
            zoom = 2.0f;
	}

    float cameraTargetX = GungeonCore::player->X();
    float cameraTargetY = GungeonCore::player->Y();

    float smoothing = 5.0f;

    cameraCurrentX += (cameraTargetX - cameraCurrentX) * smoothing * gameTime;
    cameraCurrentY += (cameraTargetY - cameraCurrentY) * smoothing * gameTime;
}

// ------------------------------------------------------------------------------

void Level1::Draw()
{
    float cameraX = cameraCurrentX;
    float cameraY = cameraCurrentY;

    float winWidth = window->Width();
    float winHeight = window->Height();

    float viewWidth = winWidth / zoom;
    float viewHeight = winHeight / zoom;

    this->viewLeft = cameraX - (viewWidth / 2.0f);
    this->viewRight = cameraX + (viewWidth / 2.0f);
    this->viewTop = cameraY - (viewHeight / 2.0f);
    this->viewBottom = cameraY + (viewHeight / 2.0f);

    if (this->viewLeft < 0.0f)
    {
        this->viewLeft = 0.0f;
        this->viewRight = viewWidth;
    }
    else if (this->viewRight > game->Width())
    {
        this->viewRight = game->Width();
        this->viewLeft = this->viewRight - viewWidth;
    }

    if (this->viewTop < 0.0f)
    {
        this->viewTop = 0.0f;
        this->viewBottom = viewHeight;
    }
    else if (this->viewBottom > game->Height())
    {
        this->viewBottom = game->Height();
        this->viewTop = this->viewBottom - viewHeight;
    }

    XMMATRIX finalMatrix = XMMatrixOrthographicOffCenterLH(this->viewLeft, this->viewRight, this->viewBottom, this->viewTop, 0.0f, 1.0f);

    Engine::renderer->SetTransform(finalMatrix);

    backg->Draw(cameraX, cameraY);

    scene->Draw();

    if (GungeonCore::viewBBox)
        scene->DrawBBox();

    if (GungeonCore::viewHUD)
    {
        GungeonCore* gc = static_cast<GungeonCore*>(Engine::game);
        if (gc && gc->GetHud())
        {
            gc->GetHud()->Draw(this->viewLeft, this->viewTop, zoom);
        }
    }
    
}

// ------------------------------------------------------------------------------

void Level1::Finalize()
{
    scene->Remove(GungeonCore::player, MOVING);
    //GungeonCore::audio->Stop(SONG_LEVEL1);
    delete scene;
    delete backg;
}

// ------------------------------------------------------------------------------
