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

    RandomMovementVillain* randomMovementVillain = new RandomMovementVillain();
    scene->Add(randomMovementVillain, STATIC);

    RunAwayVillain* runAwayMovementVillain = new RunAwayVillain(200, 1500, GungeonCore::player);
    scene->Add(runAwayMovementVillain, STATIC);

    ChaseVillain* chaseMovementVillain = new ChaseVillain(500, 1500, GungeonCore::player);
    scene->Add(chaseMovementVillain, STATIC);
    
    scene->Add(GungeonCore::player, MOVING);
    scene->Add(playerShadow, STATIC);

    tilemap = new Tilemap;
    
    tilemap->Load("Level1.tmx");
    
    scene->Add(tilemap, STATIC);

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
