/**********************************************************************************
// GungeonCore (Arquivo de Cabeçalho)
// 
// Criação:     23 Out 2012
// Atualização: 11 Nov 2021
// Compilador:  Visual C++ 2022
//
// Descrição:   Demonstração de todas as IAs
//
**********************************************************************************/

#ifndef _GungeonCore_H_
#define _GungeonCore_H_

// ------------------------------------------------------------------------------

#include "Game.h"
#include "Audio.h"
#include "Scene.h"
#include "Background.h"
#include "Hud.h"
#include "Player.h"
#include "Timer.h"

// ------------------------------------------------------------------------------

enum Ids 
{ 
    SHADOW, PLATFORM, PLAYER,  RANDOMMOVEMENTVILLAIN, CHASEVILLAIN , RUNAWAYVILLAIN, FIRE,
    ENEMYFIRE, DROPPEDITEM, HITBOX, GUN, VAZIO, JARSHRAP, JAR, EXPLOSION, WALLHIT
};

enum Soundtrack
{
    START_SCREEN, THEME_SONG, GAME_OVER_SONG, VICTORY_SONG, BOSS_FIGHT, EXPLORATION_SONG, MENU_HOVER, MENU_CLICK, ZOMBIE_SCREAM,
    BAT_NOISE,
    
    START, THEME, RUNNING, BOSS_DASH, GRAB_ITEM, MAGNUM_RELOAD, SHORTRANGE_ATTACK, FIRE_HIT_VILLAIN, MAGNUM_FIRE, TAKE_DAMAGE,
    MENU_SELECT, SHOTGUN_RELOAD, SHOTGUN_FIRE, LONGRANGE_ATTACK, VILLAIN_DIES, BOSS_DYING, BOSS_SPEECH, GRAB_KEY, FIRE_HIT_STONE,
    SONG_LEVEL1, SONG_LEVEL2, LOOP_THEME
};

// ------------------------------------------------------------------------------

class GungeonCore : public Game
{
private:
    Background * backg = nullptr;   // pano de fundo
    //Hud * hud = nullptr;            // heads up display

public:
    static Game * level;
    static bool viewBBox;          // visualização das bouding boxes
    static Player * player;         // ponteiro para o jogador
    static Audio * audio;           // sitema de áudio
    static Scene* scene;            // cena do nível
    static Timer* completionTime;
    float zoom;
	float sinkAmount;               // quantidade de afundamento dos objetos
    float viewLeft, viewRight, viewTop, viewBottom;
	Hud* hud;          // heads up display
    static bool viewHUD;

    //static bool viewHUD;            // hud visível

    void Init();                    // inicialização
    void Update();                  // atualização
    void Draw();                    // desenho
    void Finalize();                // finalização
    ViewPort& GetViewport() override;

    static uint levelNumber;

    template<class T>
    static void NextLevel()         // muda para próximo nível do jogo
    {
        if (level)
        {
            level->Finalize();
            delete level;
            level = new T();
            level->Init();
        }
    };
    Scene* GetScene() const override;
    float GetZoom() const override;
    float GetViewLeft() const override;
    float GetViewTop() const override;
    float GetSinkAmount() const override;
    Hud* GetHud() const { return hud; }
};

inline Scene* GungeonCore::GetScene() const { return scene; }
inline float GungeonCore::GetZoom() const { return zoom; }
inline float GungeonCore::GetViewLeft() const { return viewLeft; }
inline float GungeonCore::GetViewTop() const { return viewTop; }
inline float GungeonCore::GetSinkAmount() const { return sinkAmount; }

// ---------------------------------------------------------------------------------

#endif