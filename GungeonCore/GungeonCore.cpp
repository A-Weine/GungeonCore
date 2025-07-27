/**********************************************************************************
// GungeonCore (Código Fonte)
// 
// Criação:     23 Out 2012
// Atualização: 11 Nov 2021
// Compilador:  Visual C++ 2022
//
// Descrição:   Demonstração de todas as IAs

//
**********************************************************************************/

#include "Resources.h"
#include "GungeonCore.h"
#include "Engine.h"
#include "Platform.h"
#include "Shadow.h"
#include "Home.h"
#include "GameOver.h"

// ------------------------------------------------------------------------------

Game   * GungeonCore::level   = nullptr;
Player * GungeonCore::player  = nullptr;
Audio  * GungeonCore::audio   = nullptr;
Scene  * GungeonCore::scene   = nullptr;
bool GungeonCore::viewBBox = false;
Timer* GungeonCore::completionTime = nullptr;
uint GungeonCore::levelNumber = 0;
bool GungeonCore::viewHUD = false;

// ------------------------------------------------------------------------------

void GungeonCore::Init() 
{
    audio = new Audio();
    audio->Add(RUNNING, "Resources/passos.wav", 1);
    audio->Add(BOSS_DASH, "Resources/boss_dash.wav");
    audio->Add(GRAB_ITEM, "Resources/grab_item.wav");
    audio->Add(MAGNUM_RELOAD, "Resources/magnum_reload.wav");
    audio->Add(SHORTRANGE_ATTACK, "Resources/short_range_attack.wav");
    audio->Add(FIRE_HIT_VILLAIN, "Resources/fire_hit_villain.wav");
    audio->Add(MAGNUM_FIRE, "Resources/magnum_fire.wav");
    audio->Add(TAKE_DAMAGE, "Resources/take_damage.wav");
    audio->Add(MENU_SELECT, "Resources/Select.wav");
    audio->Add(SHOTGUN_RELOAD, "Resources/shotgunReload.wav");
    audio->Add(SHOTGUN_FIRE, "Resources/shotgun_fire.wav");
    audio->Add(LONGRANGE_ATTACK, "Resources/skeleton_throws_bone.wav");
    audio->Add(VILLAIN_DIES, "Resources/villain_dies.wav");
    audio->Add(BOSS_DYING, "Resources/boss_dying.wav");
    audio->Add(BOSS_SPEECH, "Resources/boss_speech.wav");
    audio->Add(GRAB_KEY, "Resources/grab_key.wav");
    audio->Add(FIRE_HIT_STONE, "Resources/fire_hit_stone.wav");
    //audio->Add(SONG_LEVEL1, "Resources/song2.wav");
    //audio->Add(SONG_LEVEL2, "Resources/song3_part1.wav");
    //audio->Add(LOOP_THEME, "Resources/song3_part2.wav");
    audio->Add(START_SCREEN, "Resources/start_screen_song.wav");
    audio->Add(THEME_SONG, "Resources/theme_song.wav");
    audio->Add(GAME_OVER_SONG, "Resources/game_over_song.wav");
    audio->Add(VICTORY_SONG, "Resources/victory_song.wav");
    audio->Add(BOSS_FIGHT, "Resources/boss_fight_song.wav");
    audio->Add(EXPLORATION_SONG, "Resources/exploration_song.wav");

    audio->Volume(MAGNUM_FIRE, 0.3f);
    audio->Volume(THEME_SONG, 0.3f);

    player  = new Player();
    hud     = new Hud();

    level = new Home();
    level->Init();

    completionTime = new Timer();
}

// ------------------------------------------------------------------------------

void GungeonCore::Update()
{
    if (window->KeyPress('B'))
        viewBBox = !viewBBox;
    
    level->Update();

    if (player->health <= 0) {
        level->Finalize();
        delete level;
        level = new GameOver();
        delete player;
		player = new Player();
		level->Init();
    }
} 

// ------------------------------------------------------------------------------

void GungeonCore::Draw()
{
    level->Draw();
}

// ------------------------------------------------------------------------------

void GungeonCore::Finalize()
{
    level->Finalize();
    completionTime->Reset();

    delete hud;
    delete player;
    delete audio;
    delete level;
}

// ------------------------------------------------------------------------------

ViewPort& GungeonCore::GetViewport()
{
    if (level)
    {
        return level->GetViewport();
    }
    else
    {
        return this->viewport;
    }
}

// ------------------------------------------------------------------------------
//                                  WinMain                                      
// ------------------------------------------------------------------------------

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, 
                     _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    Engine * engine = new Engine();

    // configura janela
    engine->window->Mode(WINDOWED);
    engine->window->Size(1024, 576);
    engine->window->Color(0, 0, 0);
    engine->window->Title("Mortal Cells Redemption");
    engine->window->Icon(IDI_ICON);
    engine->window->Cursor(IDC_CURSOR);
    engine->window->HideCursor(false);
    //engine->graphics->VSync(true);

    Game * game = new GungeonCore();

    game->Size(2560, 864);
    
    int status = engine->Start(game);

    delete engine;

    return status;
}

// ----------------------------------------------------------------------------